// File:   vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/program.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class VertexBuffer {
protected:
	GLuint id_ = 0;

public:
	inline VertexBuffer() = default;
	inline VertexBuffer(const VertexBuffer&) = delete;
	inline VertexBuffer(VertexBuffer&&) = delete;
	inline ~VertexBuffer() {
		LIBV_GL_DEBUG_ASSERT(id_ == 0);
	}

public:
	inline void create() {
		LIBV_GL_DEBUG_ASSERT(id_ == 0);
		glGenBuffers(1, &id_);
		LIBV_GL_DEBUG_CHECK();
		if (id_ == 0)
			LIBV_LOG_GL_ERROR("Failed to create VertexBuffer");
	}
	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glDeleteBuffers(1, &id_);
		id_ = 0;
		LIBV_GL_DEBUG_CHECK();
	}

	//	inline void data(const void* dataPtr, size_t length, BufferUsage usage) {
	inline void data(const void* dataPtr, GLsizeiptr length, BufferUsage usage) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		glBufferData(GL_ARRAY_BUFFER, length, dataPtr, to_value(usage));
		LIBV_GL_DEBUG_CHECK();
	}

	//	inline void subData(const void* dataPtr, size_t offset, size_t length) {
	inline void subData(const void* dataPtr, GLintptr offset, GLsizeiptr length) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		glBufferSubData(GL_ARRAY_BUFFER, offset, length, dataPtr);
		LIBV_GL_DEBUG_CHECK();
	}

	//	inline void getSubData(void* dataPtr, size_t offset, size_t length) {
	inline void getSubData(void* dataPtr, GLintptr offset, GLsizeiptr length) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		glGetBufferSubData(GL_ARRAY_BUFFER, offset, length, dataPtr);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void createData(const void* dataPtr, GLsizeiptr length, BufferUsage usage) {
		create();
		data(dataPtr, length, usage);
	}

public:
	inline auto id() const {
		return id_;
	}
	inline operator GLuint() const {
		return id_;
	}
};

// VertexBufferGuard --------------------------------------------------------------------------------

struct VertexBufferGuard : public VertexBuffer {
	inline VertexBufferGuard() {
		create();
	}
	inline VertexBufferGuard(const void* dataPtr, GLsizeiptr length, BufferUsage usage) {
		createData(dataPtr, length, usage);
	}
	inline VertexBufferGuard(const VertexBufferGuard&) = delete;
	inline VertexBufferGuard(VertexBufferGuard&&) = delete;
	inline ~VertexBufferGuard() {
		if (id_)
			destroy();
	}
};

// =================================================================================================

/** @note Vertex Arrays cannot be shared between multiple openGL contexts */
class VertexArray {
protected:
	GLuint id_ = 0;

public:
	inline VertexArray() = default;
	inline VertexArray(const VertexArray&) = delete;
	inline VertexArray(VertexArray&&) = delete;

	inline ~VertexArray() {
		LIBV_GL_DEBUG_ASSERT(id_ == 0);
	};

public:
	inline void create() {
		LIBV_GL_DEBUG_ASSERT(id_ == 0);
		glGenVertexArrays(1, &id_);
		LIBV_GL_DEBUG_CHECK();
		if (id_ == 0)
			LIBV_LOG_GL_ERROR("Failed to create VertexArray");
	}
	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glDeleteVertexArrays(1, &id_);
		id_ = 0;
		LIBV_GL_DEBUG_CHECK();
	}
	template <typename T>
	inline void bindAttribute(const VertexBuffer& buffer, const BaseAttribute<T>& attribute, size_t stride, size_t offset, bool normalized) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		if (attribute.id() == -1)
			return;
		glBindVertexArray(id_);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				normalized,
				stride,
				reinterpret_cast<const GLvoid*> (offset));
		LIBV_GL_DEBUG_CHECK();
	}
	template <typename T>
	inline void bindAttributeInt(const VertexBuffer& buffer, const BaseAttribute<T>& attribute, size_t stride, size_t offset) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		if (attribute.id() == -1)
			return;
		glBindVertexArray(id_);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(attribute);
		glVertexAttribIPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				stride,
				reinterpret_cast<const GLvoid*> (offset));
		LIBV_GL_DEBUG_CHECK();
	}
	template <typename T>
	inline void bindAttributeDouble(const VertexBuffer& buffer, const BaseAttribute<T>& attribute, size_t stride, size_t offset) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		LIBV_GL_DEBUG_ASSERT_STATIC(BaseAttribute<T>::attributeType == to_value(AttributeType::DOUBLE), "");
		if (attribute.id() == -1)
			return;
		glBindVertexArray(id_);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(attribute);
		glVertexAttribLPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				stride,
				reinterpret_cast<const GLvoid*> (offset));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void bindElements(const VertexBuffer& elements) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glBindVertexArray(id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void bindTransformFeedback(GLuint index, const VertexBuffer& buffer) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glBindVertexArray(id_);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, buffer);
		LIBV_GL_DEBUG_CHECK();
	}

public:
	inline auto id() const {
		return id_;
	}
	inline operator GLuint() const {
		return id_;
	}
};

// VertexArrayGuard --------------------------------------------------------------------------------

struct VertexArrayGuard : public VertexArray {
	inline VertexArrayGuard() = default;
	inline VertexArrayGuard(const VertexArrayGuard&) = delete;
	inline VertexArrayGuard(VertexArray&&) = delete;

	inline ~VertexArrayGuard() {
		if (this->id_)
			this->destroy();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
