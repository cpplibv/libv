// File: Log.hpp, Created on 2015. február 2. 5:11, Author: Vader

#pragma once

//libv
#include <libv/log/log.hpp>

#define checkGL() ::libv::gl::detail::logOGLError(__FUNCTION__, __VFILENAME__, __LINE__);

#define LIBV_LOG_GL_TRACE(...) LIBV_TRACE("libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_DEBUG(...) LIBV_DEBUG("libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_INFO( ...) LIBV_INFO( "libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_WARN( ...) LIBV_WARN( "libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_ERROR(...) LIBV_ERROR("libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_FATAL(...) LIBV_FATAL("libv.gl", __VA_ARGS__);

namespace libv {
namespace gl {

namespace detail {
void logOGLError(const char* func, const char* file, int line);
} //namespace detail

} //namespace gl
} //namespace libv