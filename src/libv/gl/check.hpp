// Project: libv.gl, File: src/libv/gl/check.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/source_location.hpp>
// ext
#include <GL/glew.h>
// pro
#include <libv/gl/log.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

inline void checkGL(libv::source_location loc = libv::source_location::current()) noexcept {
	while (GLenum err = glGetError())
		log_gl.error({"OpenGL: {}: {}", loc}, err, glewGetErrorString(err));
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
