// File: shader.hpp, Created on 2014. december 7. 12:56, Author: Vader

#pragma once

// std
#include <functional>
#include <string>
#include <string_view>


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
	using IncludeLoader = std::function<IncludeResult(const std::string_view)>;

private:
	IncludeLoader loader;

public:
	GLSLCompiler(IncludeLoader file_loader);
	std::string compile(const std::string_view source, const std::string_view filename = "main");
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
