// File: Log.hpp, Created on 2015. február 2. 5:11, Author: Vader

#pragma once

//vl
#include <vl/log.hpp>

#define checkGL() ::vl::gl::detail::logOGLError(__FUNCTION__, __VFILENAME__, __LINE__);

namespace vl {
namespace gl {

inline Log& log() {
	static Log log("gl", vl::log());
	return log;
}

namespace detail {
void logOGLError(const char* func, const char* file, int line);
} //namespace detail

} //namespace gl
} //namespace vl