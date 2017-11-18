// File: vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/utility/guard.hpp>
#include <libv/utility/member_offset.hpp>
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

	template <typename Container>
	inline void data(const Container& values, BufferUsage usage) {
		this->data(values.data(), values.size() * sizeof(typename Container::value_type), usage);
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

	inline void bind() noexcept {
	    glBindVertexArray(id_);
		LIBV_GL_DEBUG_CHECK();
	}

	inline void unbind() noexcept {
	    glBindVertexArray(0);
		LIBV_GL_DEBUG_CHECK();
	}

	[[nodiscard]] inline auto bind_guard() noexcept {
	    glBindVertexArray(id_);
		LIBV_GL_DEBUG_CHECK();

		return Guard([this] {
			glBindVertexArray(0);
			LIBV_GL_DEBUG_CHECK();
		});
	}

	// TODO P4: reverse the order of offset and count (for all overload)
	inline void drawArrays(Primitive mode, size_t vertexCount, size_t vertexOffset) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glDrawArrays(to_value(mode), static_cast<GLint>(vertexOffset), static_cast<GLsizei>(vertexCount));
		LIBV_GL_DEBUG_CHECK();
	}

	inline void drawElements(Primitive mode, size_t vertexCount, size_t indexOffset) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glDrawElements(to_value(mode), static_cast<GLsizei>(vertexCount), GL_UNSIGNED_INT, reinterpret_cast<void*> (sizeof (GLuint) * indexOffset));
		LIBV_GL_DEBUG_CHECK();
	}

	inline void drawElementsBaseVertex(Primitive mode, size_t vertexCount, size_t indexOffset, size_t vertexOffset) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glDrawElementsBaseVertex(to_value(mode), static_cast<GLsizei>(vertexCount), GL_UNSIGNED_INT, reinterpret_cast<void*> (sizeof (GLuint) * indexOffset), static_cast<GLint>(vertexOffset));
		LIBV_GL_DEBUG_CHECK();
	}

	template <typename T>
	inline void bindAttribute(const VertexBuffer& buffer, const BaseAttribute<T>& attribute, size_t stride, size_t offset, bool normalized) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		if (attribute.id() == -1)
			return;

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

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glEnableVertexAttribArray(attribute);
		glVertexAttribLPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				stride,
				reinterpret_cast<const GLvoid*> (offset));
		LIBV_GL_DEBUG_CHECK();
	}

	template <typename T, typename M>
	inline void bindAttribute(const VertexBuffer& buffer, GLint attribute, M T::* member, bool normalized) {
		bindAttribute(
				buffer,
				AttributeFixLocation<M>(attribute),
				sizeof (T),
				member_offset(member),
				normalized);
	}

	template <typename T, typename M>
	inline void bindAttributeInt(const VertexBuffer& buffer, GLint attribute, M T::* member) {
		bindAttributeInt(
				buffer,
				AttributeFixLocation<M>(attribute),
				sizeof (T),
				member_offset(member));
	}

	template <typename T, typename M>
	inline void bindAttributeDouble(const VertexBuffer& buffer, GLint attribute, M T::* member) {
		bindAttributeDouble(
				buffer,
				AttributeFixLocation<M>(attribute),
				sizeof (T),
				member_offset(member));
	}

	inline void bindElements(const VertexBuffer& elements) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
		LIBV_GL_DEBUG_CHECK();
	}

	inline void bindTransformFeedback(GLuint index, const VertexBuffer& buffer) {
		LIBV_GL_DEBUG_ASSERT(id_ != 0);
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

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
