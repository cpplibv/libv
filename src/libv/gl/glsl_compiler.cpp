// File: image.cpp - Created on 2017.11.06. 12:06 - Author: Vader

// hpp
#include <libv/gl/glsl_compiler.hpp>
// ext
#include <ctre.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/reverse.hpp>
// libv
#include <libv/algorithm/linear_contains.hpp>
#include <libv/range/view_lines_string_view.hpp>
#include <libv/utility/guard.hpp>
// std
#include <optional>
#include <sstream>
#include <stack>
#include <vector>
// pro
#include <libv/gl/log.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct ImplGLSLCompiler {
public:
	GLSLCompiler::IncludeLoader& loader;
	std::stringstream output;
	std::vector<std::pair<std::string_view, int>> includeStack;
	std::vector<std::string> includePragmaOnce;

public:
	std::optional<std::string_view> extract_include(std::string_view line) const noexcept;
	bool is_pragma_once(std::string_view line) const noexcept;
	bool is_version(std::string_view line) const noexcept;
	bool is_mixed_indentation(const std::string_view line) const noexcept;

	void preprocess_includes(const std::string_view source, const std::string_view filename, int include_line);
};

// -------------------------------------------------------------------------------------------------

//auto ImplGLSLCompiler::extract_inspect(const std::string_view line) const noexcept {
//	struct Result {
//		std::string_view name;
//		std::string_view type;
//		std::string_view min;
//		std::string_view max;
//		std::string_view step;
//		std::string_view init;
//	};
//
//	std::optional<Result> result;
//
//	// Accepted: #pragma inspect                 uniform vec4 color;
//	// Accepted: #pragma inspect                 uniform vec4 color = vec4(0, 0, 0, 0);
//	// Accepted: #pragma inspect(min, max)       uniform vec4 color = vec4(0, 0, 0, 0);
//	// Accepted: #pragma inspect(min, max, step) uniform vec4 color = vec4(0, 0, 0, 0);
//	if (auto m = ctre::match<R"qq(^[ \t]*#[ \t]*pragma[ \t]*inspect[ \t]+(.*);)qq">(line)) {
//	if (auto m = ctre::match<R"qq(^[ \t]*#[ \t]*pragma[ \t]*inspect[ \t]+(.*);)qq">(line)) {
//		m.get<1>().to_view();
//	}
//	} else {
//		return std::nullopt;
//	}
//}

std::optional<std::string_view> ImplGLSLCompiler::extract_include(const std::string_view line) const noexcept {
	// Accepted: #include <path> // comment
	// Accepted: #include "path" // comment

	if (auto m = ctre::match<R"qq(^[ \t]*#[ \t]*include[ \t]+(?:"(.*)"|<(.*)>).*)qq">(line)) {
		return m.get<1>().to_view().empty() ? m.get<2>().to_view() : m.get<1>().to_view();
	} else {
		return std::nullopt;
	}
}

bool ImplGLSLCompiler::is_pragma_once(const std::string_view line) const noexcept {
	return ctre::match<R"qq(^[ \t]*#[ \t]*pragma[ \t]+once[ \t]*.*)qq">(line);
}

bool ImplGLSLCompiler::is_version(const std::string_view line) const noexcept {
	return ctre::match<R"qq(^[ \t]*#[ \t]*version[ \t]+.*)qq">(line);
}

bool ImplGLSLCompiler::is_mixed_indentation(const std::string_view line) const noexcept {
	return ctre::match<R"qq(^(\t+ +| +\t+).*)qq">(line);
}

void ImplGLSLCompiler::preprocess_includes(const std::string_view source, const std::string_view filename, const int include_line) {
	int line_number = 1;

	includeStack.emplace_back(filename, include_line);
	libv::Guard stack_guard{[this] { includeStack.pop_back(); }};

	for (std::string_view line : source | libv::view::lines_string_view()) {
		const auto include = extract_include(line);

		if (!include) {
			if (is_pragma_once(line)) {
				includePragmaOnce.emplace_back(filename);
			} else {
				const auto crlf = line.ends_with("\r\n");
				const auto cr = line.ends_with("\r");
				const auto lf = !crlf && line.ends_with("\n");

				const auto mixed = is_mixed_indentation(line);

				if (crlf || cr) {
					log_gl.warn("Non generic line ending {} detected at {}:{}:{}. Converting to generic linefeed.", crlf ? "crlf" : "cr", filename, line_number, line.size() - (crlf ? 2 : 1) + 1);
				}

				if (!cr && !lf && !crlf) {
					log_gl.warn("Missing line ending detected at {}:{}:{}. Appending generic linefeed.", filename, line_number, line.size());
				}

				if (mixed) {
					log_gl.warn("Mixed indentation detected at {}:{}.", filename, line_number, 0);
				}

				if (cr || lf)
					line.remove_suffix(1);

				if (crlf)
					line.remove_suffix(2);

				// NOTE: This step also auto-appends newline on last line if it is missing
				output << line << '\n';

				if (is_version(line))
					// NOTE: Inserting top level source information after version
					output << "#line " << line_number + 1 << " \"" << filename << "\"\n";
			}
		} else {
			if (libv::linear_contains(includeStack | ranges::view::keys, *include)) {
				log_gl.error("Cyclic source inclusion detected for {} from {}:{}. Skipping inclusion.", *include, filename, line_number);
				for (const auto& entry : includeStack | ranges::view::reverse)
					log_gl.error("\tincluded from {}:{}", entry.first, entry.second);

				++line_number;
				continue;
			}

			if (libv::linear_contains(includePragmaOnce, *include)) {
				++line_number;
				continue;
			}

			auto included_source = loader(*include);
			if (!included_source.success) {
				log_gl.error("Failed to include source: {} reason: {}", *include, included_source.result);
			} else {
				output << "#line " << 1 << " \"" << *include << "\"\n";
				preprocess_includes(included_source.result, *include, line_number);
				output << "#line " << line_number + 1 << " \"" << filename << "\"\n";
			}
		}

		++line_number;
	}
}

// -------------------------------------------------------------------------------------------------

GLSLCompiler::GLSLCompiler(IncludeLoader file_loader) : loader(std::move(file_loader)) {}

std::string GLSLCompiler::compile(const std::string_view source, const std::string_view filename) {
	ImplGLSLCompiler session{loader, {}, {}, {}};

	session.preprocess_includes(source, filename, 1);

	return std::move(session.output).str();
}

std::string GLSLCompiler::load(const std::string_view filepath) {
	auto main_source = loader(filepath);

	if (!main_source.success) {
		log_gl.error("Failed to load source: {} reason: {}", filepath, main_source.result);
		return "";
	}

	return compile(main_source.result, filepath);
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
