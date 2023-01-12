// Project: libv.gl, File: src/libv/gl/check.hpp

#pragma once


// #define LIBV_ENABLE_GL_CHECK

#ifdef LIBV_ENABLE_GL_CHECK

// libv
#include <libv/utility/source_location.hpp>
// ext
#include <GL/glew.h>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

void logGLError(libv::source_location loc, GLenum err) noexcept;

inline void checkGL(libv::source_location loc = libv::source_location::current()) noexcept {
	while (GLenum err = glGetError())
		logGLError(loc, err);
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv

#else // ===========================================================================================

// libv
#include <libv/meta/force_inline.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

LIBV_FORCE_INLINE void checkGL() {
	// Noop
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv

#endif
