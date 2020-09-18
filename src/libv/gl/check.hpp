// File: Log.hpp, Created on 2015.02.02. 5:11, Author: Vader

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
	GLenum err = glGetError();
	while (err != GL_NO_ERROR) {
		log_gl.error({"OpenGL: {}: {}", loc}, err, glewGetErrorString(err));
		err = glGetError();
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
