// File: Log.hpp, Created on 2015. február 2. 5:11, Author: Vader

#pragma once

//libv
#include <libv/log/log.hpp>

#define checkGL() ::libv::gl::detail::logOGLError(__FUNCTION__, __VFILENAME__, __LINE__);

#define LIBV_GL_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv.gl", __VA_ARGS__);
#define LIBV_GL_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.gl", __VA_ARGS__);
#define LIBV_GL_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info , "libv.gl", __VA_ARGS__);
#define LIBV_GL_WARN( ...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.gl", __VA_ARGS__);
#define LIBV_GL_ERROR(...) ::libv::log(LIBV_POC, ::libv::Warn , "libv.gl", __VA_ARGS__);
#define LIBV_GL_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv.gl", __VA_ARGS__);

namespace libv {
namespace gl {

namespace detail {
void logOGLError(const char* func, const char* file, int line);
} //namespace detail

} //namespace gl
} //namespace libv