// File: shader.hpp, Created on 2014. december 7. 12:56, Author: Vader

#pragma once

// std
#include <string>
#include <string_view>
// libv
#include <libv/utility/unique_function.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct IncludeResult {
	bool success;
	std::string result; /// Error reason or include payload
};

class GLSLCompiler {
public:
	using IncludeResult = libv::gl::IncludeResult;
	using IncludeLoader = libv::unique_function<IncludeResult(const std::string_view)>;

private:
	IncludeLoader loader;

public:
	GLSLCompiler(IncludeLoader file_loader);
	/// Processes the provided source
	std::string compile(const std::string_view source, const std::string_view filename = "main");
	/// Uses the file loader to load the given file and process it
	std::string load(const std::string_view filepath);
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
