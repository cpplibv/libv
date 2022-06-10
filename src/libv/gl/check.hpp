// Project: libv.gl, File: src/libv/gl/check.hpp

#pragma once

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
