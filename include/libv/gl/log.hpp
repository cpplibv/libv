// File: Log.hpp, Created on 2015.02.02. 5:11, Author: Vader

#pragma once

// libv
#include <libv/log/log.hpp>
// ext
#include <GL/glew.h>


//#ifdef NDEBUG
#define LIBV_GL_DEBUG_CHECK() ::libv::gl::checkGL()
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

inline bool checkGL(source_location loc = source_location::current()) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		log_gl.error({"OpenGL: {}: {}", loc}, err, gluErrorString(err));
	return err != GL_NO_ERROR;
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
