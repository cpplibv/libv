// Project: libv.gl, File: src/libv/gl/vertex_array.hpp

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/utility/guard.hpp>
#include <libv/utility/member_offset.hpp>
// std
#include <bit>
// pro
#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/assert.hpp>
#include <libv/gl/attribute.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/vertex_array_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

///** @note Vertex Arrays cannot be shared between multiple openGL contexts */
class AccessVertexArray {
private:
	VertexArray& object;
	GL& gl;

public:
	inline AccessVertexArray(VertexArray& object, GL& gl) noexcept :
		object(object), gl(gl) { }

public:
	inline void create() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		glGenVertexArrays(1, &object.id);
		checkGL();
	}

	inline void destroy() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteVertexArrays(1, &object.id);
		checkGL();
	}

public:
	inline void bind() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.bind(object);
	}

	inline void unbind() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.unbindVertexArray();
	}

	[[nodiscard]] inline auto bind_guard() noexcept {
	    bind();

		return libv::guard([this] {
			unbind();
		});
	}

	// TODO P4: reverse the order of offset and count (for all overload)
	inline void drawArrays(Primitive mode, std::size_t vertexCount, std::size_t vertexOffset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawArrays(to_value(mode), static_cast<GLint>(vertexOffset), static_cast<GLsizei>(vertexCount));
		checkGL();
	}

	inline void drawElements(Primitive mode, std::size_t vertexCount, std::size_t indexOffset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElements(to_value(mode), static_cast<GLsizei>(vertexCount), GL_UNSIGNED_INT, std::bit_cast<const void*>(sizeof (GLuint) * indexOffset));
		checkGL();
	}

	inline void drawElementsBaseVertex(Primitive mode, std::size_t vertexCount, std::size_t indexOffset, std::size_t vertexOffset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElementsBaseVertex(to_value(mode), static_cast<GLsizei>(vertexCount), GL_UNSIGNED_INT, std::bit_cast<void*>(sizeof (GLuint) * indexOffset), static_cast<GLint>(vertexOffset));
		checkGL();
	}

	template <typename T>
	inline void bindAttribute(const ArrayBuffer& buffer, const BaseAttribute<T>& attribute, GLsizei stride, std::size_t offset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);
		if (attribute.id() == -1)
			return;

		glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				false,
				stride,
				std::bit_cast<const void*>(offset));
		checkGL();
	}

	template <typename T>
	inline void bindAttributeInt(const ArrayBuffer& buffer, const BaseAttribute<T>& attribute, GLsizei stride, std::size_t offset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);
		if (attribute.id() == -1)
			return;

		glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
		glEnableVertexAttribArray(attribute);
		glVertexAttribIPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				stride,
				std::bit_cast<const void*>(offset));
		checkGL();
	}

	template <typename T>
	inline void bindAttributeDouble(const ArrayBuffer& buffer, const BaseAttribute<T>& attribute, GLsizei stride, std::size_t offset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);
		LIBV_GL_DEBUG_ASSERT_STATIC(BaseAttribute<T>::attributeType == to_value(AttributeType::DOUBLE), "");
		if (attribute.id() == -1)
			return;

		glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
		glEnableVertexAttribArray(attribute);
		glVertexAttribLPointer(attribute,
				BaseAttribute<T>::attributeSize,
				BaseAttribute<T>::attributeType,
				stride,
				std::bit_cast<const void*>(offset));
		checkGL();
	}

	template <typename T, typename M>
	inline void bindAttribute(const ArrayBuffer& buffer, GLint attribute, M T::* member) noexcept {
		bindAttribute(
				buffer,
				AttributeFixLocation<M>(attribute),
				sizeof (T),
				libv::member_offset(member));
	}

	template <typename T, typename M>
	inline void bindAttributeInt(const ArrayBuffer& buffer, GLint attribute, M T::* member) noexcept {
		bindAttributeInt(
				buffer,
				AttributeFixLocation<M>(attribute),
				sizeof (T),
				libv::member_offset(member));
	}

	template <typename T, typename M>
	inline void bindAttributeDouble(const ArrayBuffer& buffer, GLint attribute, M T::* member) noexcept {
		bindAttributeDouble(
				buffer,
				AttributeFixLocation<M>(attribute),
				sizeof (T),
				libv::member_offset(member));
	}

	///
	/// @param buffer
	/// @param channel
	/// @param size
	/// @param dimension
	/// @param stride - the byte offset between two entry
	/// @param offset
	///
	inline void bindAttribute(const ArrayBuffer& buffer, GLuint channel, AttributeType type, GLint dimension, GLsizei stride, std::size_t offset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
		glEnableVertexAttribArray(channel);

		switch (type) {
		case AttributeType::BYTE:
		case AttributeType::INT:
		case AttributeType::SHORT:
		case AttributeType::UBYTE:
		case AttributeType::UINT:
		case AttributeType::USHORT:
			glVertexAttribIPointer(
					channel,
					dimension,
					to_value(type),
					stride,
					std::bit_cast<const void*>(offset));
			break;

		case AttributeType::FLOAT:
		case AttributeType::HFLOAT:
			glVertexAttribPointer(
					channel,
					dimension,
					to_value(type),
					false,
					stride,
					std::bit_cast<const void*>(offset));
			break;

		case AttributeType::DOUBLE:
			glVertexAttribLPointer(
					channel,
					dimension,
					to_value(type),
					stride,
					std::bit_cast<const void*>(offset));
			break;

		default: // TODO P5: libv.gl: instead of default log and assert invalid input
		case AttributeType::FIXED:
		case AttributeType::INT_2_10_10_10_REV:
		case AttributeType::UNSIGNED_INT_10F_11F_11F_REV:
		case AttributeType::UNSIGNED_INT_2_10_10_10_REV:
			// not supported
			break;
		}

		checkGL();
	}

	inline void bindElements(const ArrayBuffer& buffer) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id);
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
