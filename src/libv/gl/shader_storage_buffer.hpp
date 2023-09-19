// Project: libv.gl, File: src/libv/gl/uniform_buffer.hpp

#pragma once

// ext
#include <GL/glew.h>
// pro
// #include <libv/gl/assert.hpp>
#include <libv/gl/basic_buffer.hpp>
// #include <libv/gl/check.hpp>
#include <libv/gl/shader_storage_buffer_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

// Could have a common base with uniform buffer (and atomic counter buffer)
struct AccessShaderStorageBuffer : BasicAccessBuffer<ShaderStorageBuffer, GL_SHADER_STORAGE_BUFFER> {
public:
	using BasicAccessBuffer<ShaderStorageBuffer, GL_SHADER_STORAGE_BUFFER>::BasicAccessBuffer;

public:
	inline void bindBase(GLuint index) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, object.id);
		checkGL();
	}

	inline void bindRange(GLuint index, GLintptr offset, GLsizeiptr size) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, object.id, offset, size);
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
