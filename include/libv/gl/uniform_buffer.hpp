// File: vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/basic_buffer.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/uniform_buffer_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct AccessUniformBuffer : BasicAccessBuffer<UniformBuffer, GL_UNIFORM_BUFFER> {
public:
	using BasicAccessBuffer<UniformBuffer, GL_UNIFORM_BUFFER>::BasicAccessBuffer;

public:
	inline void bindBase(GLuint index) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, index, object.id);
		checkGL();
	}

	inline void bindRange(GLuint index, GLintptr offset, GLsizeiptr size) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, index, object.id, offset, size);
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
