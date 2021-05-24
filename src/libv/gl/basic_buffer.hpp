// Project: libv.gl, File: src/libv/gl/basic_buffer.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/utility/guard.hpp>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

// TODO P5: BasicAccessBuffer is competing with AccessBuffer
template <typename Object, GLenum Target>
struct BasicAccessBuffer {
protected:
	Object& object;

public:
	BasicAccessBuffer(Object& object) :
		object(object) { }

public:
	inline void create() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		glGenBuffers(1, &object.id);
		checkGL();
	}

	inline void destroy() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteBuffers(1, &object.id);
		object.id = 0;
		checkGL();
	}

public:
	void bind() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBindBuffer(Target, object.id);
		checkGL();
	}

	void unbind() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBindBuffer(Target, 0);
		checkGL();
	}

	[[nodiscard]] inline auto bind_guard() noexcept {
	    bind();

		return libv::guard([this] {
			unbind();
		});
	}

public:
	inline void data(const void* ptr, GLsizeiptr length, BufferUsage usage) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBufferData(Target, length, ptr, to_value(usage));
		checkGL();
	}

	template <typename Range>
	inline void data(const Range& data, BufferUsage usage) noexcept {
		this->data(data.data(), data.size() * sizeof(typename Range::value_type), usage);
	}

	inline void subData(const void* ptr, GLintptr offset, GLsizeiptr length) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBufferSubData(Target, offset, length, ptr);
		checkGL();
	}

	template <typename Range>
	inline void subData(const Range& data, GLintptr offset) noexcept {
		this->subData(data.data(), offset, data.size() * sizeof(typename Range::value_type));
	}

	inline void getSubData(void* ptr, GLintptr offset, GLsizeiptr length) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glGetBufferSubData(Target, offset, length, ptr);
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
