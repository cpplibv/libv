// Project: libv.gl, File: src/libv/gl/basic_buffer.hpp

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
	explicit BasicAccessBuffer(Object& object) :
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
		this->data(std::data(data), std::size(data) * sizeof(data[0]), usage);
	}

	inline void subData(const void* ptr, GLintptr offset, GLsizeiptr length) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glBufferSubData(Target, offset, length, ptr);
		checkGL();
	}

	template <typename Range>
	inline void subData(const Range& data, GLintptr offset) noexcept {
		this->subData(std::data(data), offset, std::size(data) * sizeof(data[0]));
	}

	inline void getSubData(void* ptr, GLintptr offset, GLsizeiptr length) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glGetBufferSubData(Target, offset, length, ptr);
		checkGL();
	}

public:
	/// OpenGL 4.5, DSA
	inline void* map(BufferAccessFull access) noexcept {
		auto ptr = glMapNamedBuffer(object.id, libv::to_underlying(access));
		checkGL();
		return ptr;
	}

	/// OpenGL 4.5, DSA
	inline void* mapRange(GLintptr offset, GLsizeiptr length , BufferAccess access) noexcept {
		auto ptr = glMapNamedBufferRange(object.id, offset, length, libv::to_underlying(access));
		checkGL();
		return ptr;
	}

	/// OpenGL 4.5, DSA
	/// \Returns true unless the data store contents have become corrupt during the time the data store was mapped
	[[nodiscard]] inline bool unmap() noexcept {
		auto valid = glUnmapNamedBuffer(object.id);
		checkGL();
		return valid;
	}

	/// OpenGL 4.5, DSA
	/// - must previously have been mapped with the GL_MAP_FLUSH_EXPLICIT_BIT flag
	inline void flushMappedRange(GLintptr offset, GLsizeiptr length) noexcept {
		glFlushMappedNamedBufferRange(object.id, offset, length);
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
