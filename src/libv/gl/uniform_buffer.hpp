// Project: libv.gl, File: src/libv/gl/uniform_buffer.hpp

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
	inline void bindBase(uint32_t binding) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.bindUniformBufferBase(object, binding);
	}

	inline void bindRange(uint32_t binding, uint32_t offset, uint32_t size) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.bindUniformBufferRange(object, binding, offset, size);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
