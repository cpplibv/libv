// Project: libv, File: app/space/glsl_include_engine.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/rev/glsl_compose.hpp>
// ext
#include <ctre.hpp>
// libv
#include <libv/algo/linear_contains.hpp>
#include <libv/range/view_lines_string_view.hpp>
#include <libv/utility/guard.hpp>
// std
#include <algorithm>
#include <optional>
#include <ranges>
#include <span>
#include <sstream>
#include <vector>
// pro
#include <libv/rev/log.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class LoadShaderSourceContext {
public:
	const IncludeLoadFunc& loader;
	std::span<std::span<const std::string>> injected_defines_list;
	std::span<std::span<const std::string>> injected_includes_list;

	std::stringstream output;
	std::vector<std::pair<std::string, int>> include_stack;
	std::vector<std::string> includePragmaOnce;

private:
	static std::optional<std::string_view> extract_include(std::string_view line) noexcept;
	static bool is_pragma_once(std::string_view line) noexcept;
	static bool is_version(std::string_view line) noexcept;
	static bool is_mixed_indentation(std::string_view line) noexcept;

private:
	bool process_include(const std::string_view filename);
	void process_source(const std::string_view source, const std::string_view filename);

public:
	std::string load_from_include(const std::string_view filename);
	std::string load_from_source(const std::string_view source);
};

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

bool LoadShaderSourceContext::process_include(const std::string_view filename) {
	if (libv::linear_contains(include_stack | std::views::keys, filename)) {
		log_rev.error("Cyclic source inclusion detected for {}. Skipping inclusion.", filename);
		for (const auto& entry : include_stack | std::views::reverse)
			log_rev.error("    Included from {}:{}", entry.first, entry.second);
		return false;
	}

	if (libv::linear_contains(includePragmaOnce, filename))
		return false;

	auto included_source = loader(filename);
	if (included_source.ec) {
		auto e = glsl_failed_include_exception(
				included_source.result, // Contains the file_path
				included_source.ec,
				std::string(filename),
				include_stack
		);
		std::ranges::reverse(e.include_stack);

		throw e;
	}

	const auto stack_guard = libv::guard{[this] { include_stack.pop_back(); }};
	include_stack.emplace_back(filename, 0);

	const auto is_main_source = include_stack.size() == 1;
	if (!is_main_source)
		output << "#line " << 1 << " \"" << filename << "\"\n";
	process_source(included_source.result, filename);

	return true;
}

void LoadShaderSourceContext::process_source(const std::string_view source, const std::string_view filename) {
	const auto line_number = [this]() -> auto& { return include_stack.back().second; };

	for (std::string_view line : source | libv::view::lines_string_view()) {
		line_number()++;

		if (const auto include_opt = extract_include(line)) {
			if (process_include(*include_opt))
				output << "#line " << line_number() + 1 << " \"" << filename << "\"\n";

		} else if (is_pragma_once(line)) {
			includePragmaOnce.emplace_back(filename);

		} else {
			const auto crlf = line.ends_with("\r\n");
			const auto cr = line.ends_with("\r");
			const auto lf = !crlf && line.ends_with("\n");

			const auto mixed = is_mixed_indentation(line);

			if (crlf || cr) {
				log_rev.warn("Non generic line ending {} detected at {}:{}:{}. Converting to generic linefeed.", crlf ? "crlf" : "cr", filename, line_number(), line.size() - (crlf ? 2 : 1) + 1);
			}

			if (!cr && !lf && !crlf) {
				log_rev.warn("Missing line ending detected at {}:{}:{}. Appending generic linefeed.", filename, line_number(), line.size());
			}

			if (mixed) {
				log_rev.warn("Mixed indentation detected at {}:{}.", filename, line_number(), 0);
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
				const auto is_main_source = include_stack.size() == 1;

				if (is_main_source && has_injected_define) {
					// If this is the main file inject defines
					output << "// --- Source code injected defines start ---\n";
					output << "#line 1 \"injected_defines\"\n";
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
				output << "#line " << line_number() + 1 << " \"" << filename << "\"\n";
			}
		}
	}
}

std::string LoadShaderSourceContext::load_from_include(const std::string_view filename) {
	process_include(filename);
	return std::move(output).str();
}

std::string LoadShaderSourceContext::load_from_source(const std::string_view source) {
	include_stack.emplace_back("main", 0);
	process_source(source, "main");
	return std::move(output).str();
}

// -------------------------------------------------------------------------------------------------

std::string glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<const std::string> defines, std::span<const std::string> includes) {
	std::span<std::span<const std::string>> defines_list(&defines, 1);
	std::span<std::span<const std::string>> includes_list(&includes, 1);
	return glsl_compose_from_include(loader, path_main, defines_list, includes_list);
}

std::string glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<std::span<const std::string>> defines_list, std::span<const std::string> includes) {
	std::span<std::span<const std::string>> includes_list(&includes, 1);
	return glsl_compose_from_include(loader, path_main, defines_list, includes_list);
}

std::string glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<const std::string> defines, std::span<std::span<const std::string>> includes_list) {
	std::span<std::span<const std::string>> defines_list(&defines, 1);
	return glsl_compose_from_include(loader, path_main, defines_list, includes_list);
}

std::string glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<std::span<const std::string>> defines_list, std::span<std::span<const std::string>> includes_list) {
	LoadShaderSourceContext context{loader, defines_list, includes_list, {}, {}, {}};
	return context.load_from_include(path_main);
}

std::string glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<const std::string> defines, std::span<const std::string> includes) {
	std::span<std::span<const std::string>> defines_list(&defines, 1);
	std::span<std::span<const std::string>> includes_list(&includes, 1);
	return glsl_compose_from_source(loader, path_main, defines_list, includes_list);
}

std::string glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<std::span<const std::string>> defines_list, std::span<const std::string> includes) {
	std::span<std::span<const std::string>> includes_list(&includes, 1);
	return glsl_compose_from_source(loader, path_main, defines_list, includes_list);
}

std::string glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<const std::string> defines, std::span<std::span<const std::string>> includes_list) {
	std::span<std::span<const std::string>> defines_list(&defines, 1);
	return glsl_compose_from_source(loader, path_main, defines_list, includes_list);
}

std::string glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view path_main, std::span<std::span<const std::string>> defines_list, std::span<std::span<const std::string>> includes_list) {
	LoadShaderSourceContext context{loader, defines_list, includes_list, {}, {}, {}};
	return context.load_from_source(path_main);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
