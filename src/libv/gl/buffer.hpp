// File: vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/buffer_object.hpp>
#include <libv/gl/check.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct AccessBuffer {
protected:
	Buffer& object;

public:
	AccessBuffer(Buffer& object) :
		object(object) { }

public:
	inline void create() {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		glGenBuffers(1, &object.id);
		checkGL();
	}

	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteBuffers(1, &object.id);
		object.id = 0;
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
