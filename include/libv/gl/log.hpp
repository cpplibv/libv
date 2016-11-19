// File: Log.hpp, Created on 2015. február 2. 5:11, Author: Vader

#pragma once

//libv
#include <libv/log/log.hpp>
// ext
#include <GL/glew.h>

#define checkGL() ::libv::gl::detail::logOGLError(__FUNCTION__, __LIBV_SHORT_FILE__, __LINE__);

#define LIBV_LOG_GL_TRACE(...) LIBV_TRACE("libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_DEBUG(...) LIBV_DEBUG("libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_INFO( ...) LIBV_INFO( "libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_WARN( ...) LIBV_WARN( "libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_ERROR(...) LIBV_ERROR("libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_FATAL(...) LIBV_FATAL("libv.gl", __VA_ARGS__);

namespace libv {
namespace gl {
namespace detail {

inline bool logOGLError(const char* func, const char* file, int line) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		::libv::log(CodePosition{file, func, line}, ::libv::Error, "libv.gl",
			"OpenGL: %s", reinterpret_cast<const char*> (gluErrorString(err)));
	return err != GL_NO_ERROR;
}

} //namespace detail
} //namespace gl
} //namespace libv