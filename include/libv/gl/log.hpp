// File: Log.hpp, Created on 2015.02.02. 5:11, Author: Vader

#pragma once

// libv
#include <libv/log/log.hpp>
// ext
#include <GL/glew.h>


//#ifdef NDEBUG
#define LIBV_GL_DEBUG_CHECK() LIBV_GL_CHECK()
#define LIBV_GL_DEBUG_ASSERT(X) assert((X))
#define LIBV_GL_DEBUG_ASSERT_STATIC(X, M) static_assert((X), M)//#else
//#    define LIBV_GL_DEBUG_CHECK() {}
//#    define LIBV_GL_DEBUG_ASSERT(X) {}
//#    define LIBV_GL_DEBUG_ASSERT_STATIC(X) {}
//#endif

namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

inline LoggerModule log_gl{libv::logger, "libv.gl"};

#define LIBV_LOG_GL_TRACE(...) ::libv::gl::log_gl.trace(__VA_ARGS__)
#define LIBV_LOG_GL_DEBUG(...) ::libv::gl::log_gl.debug(__VA_ARGS__)
#define LIBV_LOG_GL_INFO( ...) ::libv::gl::log_gl.info (__VA_ARGS__)
#define LIBV_LOG_GL_WARN( ...) ::libv::gl::log_gl.warn (__VA_ARGS__)
#define LIBV_LOG_GL_ERROR(...) ::libv::gl::log_gl.error(__VA_ARGS__)
#define LIBV_LOG_GL_FATAL(...) ::libv::gl::log_gl.fatal(__VA_ARGS__)

#define LIBV_GL_CHECK() ::libv::gl::detail::logOGLError()

namespace detail {

inline bool logOGLError(source_location loc = source_location::current()) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		log_gl.error({"OpenGL: {}: {}", loc}, gluErrorString(err));
	return err != GL_NO_ERROR;
}

} // namespace detail

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
