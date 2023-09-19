// Project: libv.re, File: src/libv/re/core/glsl/glsl_compose.cpp

#include <libv/re/core/glsl/glsl_compose.hpp>

#include <ctre.hpp>

#include <libv/algo/linear_contains.hpp>
#include <libv/container/flat_map.hpp>
#include <libv/range/view_lines_string_view.hpp>
#include <libv/re/log.hpp>
#include <libv/utility/guard.hpp>

#include <algorithm>
#include <optional>
#include <ranges>
#include <span>
#include <sstream>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class LoadShaderSourceContext {
public:
	const IncludeLoadFunc& loader;
	std::span<std::span<const std::string>> injected_defines_list;
	std::span<std::span<const std::string>> injected_includes_list;

	std::stringstream output;
	std::vector<std::pair<std::string, int>> includeStack;
	std::vector<std::string> includePragmaOnce;

	int mappingFilenameNextIndex = 2090909000; // High index start to avoid random collisions in error strings
	libv::flat_map<int, std::string> mappingIndexToFilename;
	libv::flat_map<std::string, int, std::less<>> mappingFilenameToIndex;

public:
	LoadShaderSourceContext(const IncludeLoadFunc& loader, std::span<std::span<const std::string>> injectedDefinesList, std::span<std::span<const std::string>> injectedIncludesList);

private:
	static std::optional<std::string_view> extract_include(std::string_view line) noexcept;
	static bool is_pragma_once(std::string_view line) noexcept;
	static bool is_version(std::string_view line) noexcept;
	static bool is_mixed_indentation(std::string_view line) noexcept;

private:
	int filenameToIndex(std::string_view name);

private:
	bool process_include(const std::string_view filename);
	void process_source(const std::string_view source, const std::string_view filename);

public:
	std::string load_from_include(const std::string_view filename);
	std::string load_from_source(const std::string_view source);
};

// -------------------------------------------------------------------------------------------------

LoadShaderSourceContext::LoadShaderSourceContext(const IncludeLoadFunc& loader, std::span<std::span<const std::string>> injectedDefinesList, std::span<std::span<const std::string>> injectedIncludesList) :
		loader(loader),
		injected_defines_list(injectedDefinesList),
		injected_includes_list(injectedIncludesList) {}

// -------------------------------------------------------------------------------------------------

std::optional<std::string_view> LoadShaderSourceContext::extract_include(const std::string_view line) noexcept {
	// Accepted: #include <path> // comment
	// Accepted: #include "path" // comment

	if (auto m = ctre::match<R"qq(^[ \t]*#[ \t]*include[ \t]+(?:"(.*)"|<(.*)>).*)qq">(line)) {
		return m.get<1>().to_view().empty() ? m.get<2>().to_view() : m.get<1>().to_view();
	} else {
		return std::nullopt;
	}
}

bool LoadShaderSourceContext::is_pragma_once(const std::string_view line) noexcept {
	return ctre::match<R"qq(^[ \t]*#[ \t]*pragma[ \t]+once[ \t]*.*)qq">(line);
}

bool LoadShaderSourceContext::is_version(const std::string_view line) noexcept {
	return ctre::match<R"qq(^[ \t]*#[ \t]*version[ \t]+.*)qq">(line);
}

bool LoadShaderSourceContext::is_mixed_indentation(const std::string_view line) noexcept {
	return ctre::match<R"qq(^(\t+ +| +\t+).*)qq">(line);
}

int LoadShaderSourceContext::filenameToIndex(std::string_view name) {
	if (auto it = mappingFilenameToIndex.find(name); it != mappingFilenameToIndex.end())
		return it->second;

	const auto index = mappingFilenameNextIndex++;
	mappingFilenameToIndex.emplace(name, index);
	mappingIndexToFilename.emplace(index, name);
	return index;
}

bool LoadShaderSourceContext::process_include(const std::string_view filename) {
	if (libv::linear_contains(includeStack | std::views::keys, filename)) {
		log_re.error("Cyclic source inclusion detected for {}. Skipping inclusion.", filename);
		for (const auto& entry : includeStack | std::views::reverse)
			log_re.error("    Included from {}:{}", entry.first, entry.second);
		return false;
	}

	if (libv::linear_contains(includePragmaOnce, filename))
		return false;

	auto included_source = loader(filename, includeStack);

	const auto stack_guard = libv::guard{[this] { includeStack.pop_back(); }};
	includeStack.emplace_back(filename, 0);

	const auto is_main_source = includeStack.size() == 1;
	if (!is_main_source)
		output << "#line " << 1 << " " << filenameToIndex(filename) << " // " << filename << "\n";
	process_source(included_source, filename);

	return true;
}

void LoadShaderSourceContext::process_source(const std::string_view source, const std::string_view filename) {
	const auto line_number = [this]() -> auto& { return includeStack.back().second; };

	for (std::string_view line : source | libv::view::lines_string_view()) {
		line_number()++;

		if (const auto include_opt = extract_include(line)) {
			if (process_include(*include_opt))
				output << "#line " << line_number() + 1 << " " << filenameToIndex(filename) << " // " << filename << "\n";
			else
				output << "\n"; // Place an empty line instead of the 'include' directive to keep line numbers intact

		} else if (is_pragma_once(line)) {
			includePragmaOnce.emplace_back(filename);
			output << "\n"; // Place an empty line instead of the 'pragma once' directive to keep line numbers intact

		} else {
			const auto crlf = line.ends_with("\r\n");
			const auto cr = line.ends_with("\r");
			const auto lf = !crlf && line.ends_with("\n");

			const auto mixed = is_mixed_indentation(line);

			if (crlf || cr) {
				log_re.warn("Non generic line ending {} detected at {}:{}:{}. Converting to generic linefeed.", crlf ? "crlf" : "cr", filename, line_number(), line.size() - (crlf ? 2 : 1) + 1);
			}

			if (!cr && !lf && !crlf) {
				log_re.warn("Missing line ending detected at {}:{}:{}. Appending generic linefeed.", filename, line_number(), line.size());
			}

			if (mixed) {
				log_re.warn("Mixed indentation detected at {}:{}.", filename, line_number(), 0);
			}

			if (cr || lf)
				line.remove_suffix(1);

			if (crlf)
				line.remove_suffix(2);

			// NOTE: This step also auto-appends newline on last line if it is missing
			output << line << '\n';

			if (is_version(line)) {
				const auto has_injected_define = std::ranges::any_of(injected_defines_list, [](const auto& r){ return !r.empty(); });
				const auto has_injected_include = std::ranges::any_of(injected_includes_list, [](const auto& r){ return !r.empty(); });
				const auto is_main_source = includeStack.size() == 1;

				if (is_main_source && has_injected_define) {
					// If this is the main file inject defines
					output << "// --- Source code injected defines start ---\n";
					output << "#line 1 " << filenameToIndex("injected_defines") << " // injected_defines\n";
					for (const auto& defines : injected_defines_list)
						for (const auto& define : defines)
							output << "#define " << define << "\n";
					output << "// --- Source code injected defines end ---\n";
				}

				if (is_main_source && has_injected_include) {
					output << "// --- Source code injected includes start ---\n";
					for (const auto& injected_includes : injected_includes_list) {
						for (const auto& injected_include : injected_includes) {
							process_include(injected_include);
						}
					}
					output << "// --- Source code injected includes end ---\n";
				}

				// NOTE: Inserting top level source information after version (version has to come first)
				output << "#line " << line_number() + 1 << " " << filenameToIndex(filename) << " // " << filename << "\n";
			}
		}
	}
}

std::string LoadShaderSourceContext::load_from_include(const std::string_view filename) {
	process_include(filename);
	return std::move(output).str();
}

std::string LoadShaderSourceContext::load_from_source(const std::string_view source) {
	includeStack.emplace_back("main", 0);
	process_source(source, "main");
	return std::move(output).str();
}

// -------------------------------------------------------------------------------------------------

GLSLSourceCode glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<const std::string> defines, std::span<const std::string> includes) {
	std::span<std::span<const std::string>> defines_list(&defines, 1);
	std::span<std::span<const std::string>> includes_list(&includes, 1);
	return glsl_compose_from_include(loader, path_main, defines_list, includes_list);
}

GLSLSourceCode glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<std::span<const std::string>> defines_list, std::span<const std::string> includes) {
	std::span<std::span<const std::string>> includes_list(&includes, 1);
	return glsl_compose_from_include(loader, path_main, defines_list, includes_list);
}

GLSLSourceCode glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<const std::string> defines, std::span<std::span<const std::string>> includes_list) {
	std::span<std::span<const std::string>> defines_list(&defines, 1);
	return glsl_compose_from_include(loader, path_main, defines_list, includes_list);
}

GLSLSourceCode glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<std::span<const std::string>> defines_list, std::span<std::span<const std::string>> includes_list) {
	LoadShaderSourceContext context{loader, defines_list, includes_list};
	return {context.load_from_include(path_main), std::move(context.mappingIndexToFilename)};
}

GLSLSourceCode glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<const std::string> defines, std::span<const std::string> includes) {
	std::span<std::span<const std::string>> defines_list(&defines, 1);
	std::span<std::span<const std::string>> includes_list(&includes, 1);
	return glsl_compose_from_source(loader, path_main, defines_list, includes_list);
}

GLSLSourceCode glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<std::span<const std::string>> defines_list, std::span<const std::string> includes) {
	std::span<std::span<const std::string>> includes_list(&includes, 1);
	return glsl_compose_from_source(loader, path_main, defines_list, includes_list);
}

GLSLSourceCode glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<const std::string> defines, std::span<std::span<const std::string>> includes_list) {
	std::span<std::span<const std::string>> defines_list(&defines, 1);
	return glsl_compose_from_source(loader, path_main, defines_list, includes_list);
}

GLSLSourceCode glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<std::span<const std::string>> defines_list, std::span<std::span<const std::string>> includes_list) {
	LoadShaderSourceContext context{loader, defines_list, includes_list};
	return {context.load_from_source(path_main), std::move(context.mappingIndexToFilename)};
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
