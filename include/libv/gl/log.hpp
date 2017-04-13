// File: Log.hpp, Created on 2015. február 2. 5:11, Author: Vader

#pragma once

//libv
#include <libv/log/log.hpp>
// ext
#include <GL/glew.h>

#define LIBV_GL_CHECK() ::libv::gl::detail::logOGLError(__FUNCTION__, __LIBV_SHORT_FILE__, __LINE__);

// -------------------------------------------------------------------------------------------------

#ifdef NDEBUG
#    define LIBV_GL_DEBUG_CHECK() LIBV_GL_CHECK()
#    define LIBV_GL_DEBUG_ASSERT(X) assert((X))
#    define LIBV_GL_DEBUG_ASSERT_STATIC(X, M) static_assert((X), M)
#else
#    define LIBV_GL_DEBUG_CHECK() {}
#    define LIBV_GL_DEBUG_ASSERT(X) {}
#    define LIBV_GL_DEBUG_ASSERT_STATIC(X) {}
#endif

// -------------------------------------------------------------------------------------------------

#define LIBV_LOG_GL_TRACE(...) LIBV_TRACE("libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_DEBUG(...) LIBV_DEBUG("libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_INFO( ...) LIBV_INFO( "libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_WARN( ...) LIBV_WARN( "libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_ERROR(...) LIBV_ERROR("libv.gl", __VA_ARGS__);
#define LIBV_LOG_GL_FATAL(...) LIBV_FATAL("libv.gl", __VA_ARGS__);

namespace libv {
namespace gl {
namespace detail {

template <typename = void>
bool logOGLError(const char* func, const char* file, int line) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		::libv::log(CodePosition{file, func, line}, ::libv::Error, "libv.gl",
				"OpenGL: %s: %s", func, gluErrorString(err));
	return err != GL_NO_ERROR;
}

} // namespace detail
} // namespace gl
} // namespace libv