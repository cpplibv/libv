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

	void preprocess_includes(const std::string_view source, const std::string_view filename, int include_line);
};

// -------------------------------------------------------------------------------------------------

std::optional<std::string_view> ImplGLSLCompiler::extract_include(const std::string_view line) const noexcept {
	if (auto m = ctre::match<R"qq(^[ \t]*#[ \t]*include[ \t]+(?:"(.*)"|<(.*)>).*)qq">(line)) {
		return m.get<1>().to_view().empty() ? m.get<2>().to_view() : m.get<1>().to_view();
	} else {
		return std::nullopt;
	}
}

bool ImplGLSLCompiler::is_pragma_once(const std::string_view line) const noexcept {
	return ctre::match<R"qq(^[ \t]*#[ \t]*pragma[ \t]+once[ \t]*.*)qq">(line);
}

void ImplGLSLCompiler::preprocess_includes(const std::string_view source, const std::string_view filename, const int include_line) {
	int line_number = 1;

	includeStack.emplace_back(filename, include_line);
	libv::Guard stack_guard{[this] { includeStack.pop_back(); }};

	for (const std::string_view line : source | libv::view::lines_string_view()) {
		const auto include = extract_include(line);

		if (!include) {
			if (is_pragma_once(line)) {
				includePragmaOnce.emplace_back(filename);
			} else {
				output << line;
				if (line.back() != '\n')
					// Append newline on last line if its missing
					output << '\n';
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
				output << "#line "<< 1 << " \"" << *include << "\"\n";
				preprocess_includes(included_source.result, *include, line_number);
				output << "#line "<< line_number + 1 << " \"" << filename << "\"\n";
			}
		}

		++line_number;
	}
}

// -------------------------------------------------------------------------------------------------

GLSLCompiler::GLSLCompiler(IncludeLoader file_loader) : loader(std::move(file_loader)) {}

std::string GLSLCompiler::compile(const std::string_view source, const std::string_view filename) {
	ImplGLSLCompiler session{loader};

	session.preprocess_includes(source, filename, 1);

	return std::move(session.output).str();
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
