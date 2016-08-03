// File:   vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>

namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class BaseVertexBuffer {
private:
	GLuint vbo = 0;

protected:
	inline BaseVertexBuffer() = default;
	inline BaseVertexBuffer(const BaseVertexBuffer&) = delete;
	inline BaseVertexBuffer(BaseVertexBuffer&&) = delete;
	inline ~BaseVertexBuffer() = default;

public:
	inline void create() {
		LIBV_GL_DEBUG_ASSERT(vbo == 0);
		glGenBuffers(1, &vbo);
		LIBV_GL_DEBUG_CHECK_GL();
		if (vbo == 0)
			LIBV_LOG_GL_ERROR("Failed to create VertexBuffer");
	}

	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(vbo != 0);
		glDeleteBuffers(1, &vbo);
		vbo = 0;
		LIBV_GL_DEBUG_CHECK_GL();
	}

	//	inline void data(const void* dataPtr, size_t length, BufferUsage usage) {
	inline void data(const void* dataPtr, GLsizeiptr length, BufferUsage usage) {
		LIBV_GL_DEBUG_ASSERT(vbo != 0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, length, dataPtr, to_value(usage));
		LIBV_GL_DEBUG_CHECK_GL();
	}

//	inline void subData(const void* dataPtr, size_t offset, size_t length) {
	inline void subData(const void* dataPtr, GLintptr offset, GLsizeiptr length) {
		LIBV_GL_DEBUG_ASSERT(vbo != 0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, offset, length, dataPtr);
		LIBV_GL_DEBUG_CHECK_GL();
	}

//	inline void getSubData(void* dataPtr, size_t offset, size_t length) {
	inline void getSubData(void* dataPtr, GLintptr offset, GLsizeiptr length) {
		LIBV_GL_DEBUG_ASSERT(vbo != 0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glGetBufferSubData(GL_ARRAY_BUFFER, offset, length, dataPtr);
		LIBV_GL_DEBUG_CHECK_GL();
	}

	inline void createData(const void* dataPtr, GLsizeiptr length, BufferUsage usage) {
		create();
		data(dataPtr, length, usage);
	}

public:
	inline auto id() const {
		return vbo;
	}

	inline operator GLuint() const {
		return vbo;
	}
};

// VertexBufferAC ---------------------------------------------------------------------------------------

class VertexBufferAC : public BaseVertexBuffer {
public:
	inline VertexBufferAC() {
		create();
	}
	inline VertexBufferAC(const void* dataPtr, GLsizeiptr length, BufferUsage usage) {
		createData(dataPtr, length, usage);
	}
	inline VertexBufferAC(const VertexBufferAC&) = delete;
	inline VertexBufferAC(VertexBufferAC&&) = delete;
	inline ~VertexBufferAC() {
		if (id())
			destroy();
	}
};

using VertexBuffer = VertexBufferAC;

// VertexBufferNC ---------------------------------------------------------------------------------------

class VertexBufferNC : public BaseVertexBuffer {
public:
	inline VertexBufferNC() = default;
	inline ~VertexBufferNC() {
		LIBV_GL_DEBUG_ASSERT(id() == 0);
	}
};

// =================================================================================================

/** @note Vertex Arrays cannot be shared between multiple openGL contexts */
class BaseVertexArray {
private:
	GLuint vao = 0;

protected:
	inline BaseVertexArray() = default;
	inline BaseVertexArray(const BaseShader&) = delete;
	inline BaseVertexArray(BaseShader&&) = delete;
	inline ~BaseVertexArray() = default;

public:
	inline void create() {
		LIBV_GL_DEBUG_ASSERT(vao == 0);
		glGenVertexArrays(1, &vao);
		LIBV_GL_DEBUG_CHECK_GL();
		if (vao == 0)
			LIBV_LOG_GL_ERROR("Failed to create VertexArray");
	}

	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(vao != 0);
		glDeleteVertexArrays(1, &vao);
		vao = 0;
		LIBV_GL_DEBUG_CHECK_GL();
	}

	template <typename T>
	inline void bindAttribute(const VertexBuffer& buffer, const BaseAttribute<T>& attribute, size_t stride, size_t offset, bool normalized) {
		LIBV_GL_DEBUG_ASSERT(vao != 0);
		if (attribute.id() == -1)
			return;
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				normalized,
				stride,
				reinterpret_cast<const GLvoid*>(offset));
		LIBV_GL_DEBUG_CHECK_GL();
	}

	template <typename T>
	inline void bindAttributeInt(const VertexBuffer& buffer, const BaseAttribute<T>& attribute, size_t stride, size_t offset) {
		LIBV_GL_DEBUG_ASSERT(vao != 0);
		if (attribute.id() == -1)
			return;
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(attribute);
		glVertexAttribIPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				stride,
				reinterpret_cast<const GLvoid*>(offset));
		LIBV_GL_DEBUG_CHECK_GL();
	}

	template <typename T>
	inline void bindAttributeDouble(const VertexBuffer& buffer, const BaseAttribute<T>& attribute, size_t stride, size_t offset) {
		LIBV_GL_DEBUG_ASSERT(vao != 0);
		LIBV_GL_DEBUG_ASSERT_STATIC(BaseAttribute<T>::attributeType == to_value(AttributeType::DOUBLE), "");
		if (attribute.id() == -1)
			return;
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(attribute);
		glVertexAttribLPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				stride,
				reinterpret_cast<const GLvoid*>(offset));
		LIBV_GL_DEBUG_CHECK_GL();
	}

	inline void bindElements(const VertexBuffer& elements) {
		LIBV_GL_DEBUG_ASSERT(vao != 0);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
		LIBV_GL_DEBUG_CHECK_GL();
	}

	inline void bindTransformFeedback(GLuint index, const VertexBuffer& buffer) {
		LIBV_GL_DEBUG_ASSERT(vao != 0);
		glBindVertexArray(vao);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, buffer);
		LIBV_GL_DEBUG_CHECK_GL();
	}

public:
	inline auto id() const {
		return vao;
	}

	inline operator GLuint() const {
		return vao;
	}
};

// VertexArrayAC ---------------------------------------------------------------------------------------

class VertexArrayAC : public BaseVertexArray {
public:
	inline VertexArrayAC() {
		create();
	}
	inline ~VertexArrayAC() {
		if (id())
			destroy();
	}
};

using VertexArray = VertexArrayAC;

// VertexArrayNC ---------------------------------------------------------------------------------------

class VertexArrayNC : public BaseVertexArray {
public:
	inline VertexArrayNC() = default;
	inline ~VertexArrayNC() {
		LIBV_GL_DEBUG_ASSERT(id() == 0);
	}
};


// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv
