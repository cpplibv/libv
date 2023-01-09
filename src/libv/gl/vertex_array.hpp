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
#include <libv/gl/gl.hpp>
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

	inline void drawArrays(Primitive mode, int32_t vertexOffset, int32_t vertexCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawArrays(to_value(mode), vertexOffset, vertexCount);
		checkGL();
	}

	inline void drawElements(Primitive mode, std::size_t indexOffset, int32_t indexCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElements(to_value(mode), indexCount, GL_UNSIGNED_INT, std::bit_cast<const void*>(sizeof (GLuint) * indexOffset));
		checkGL();
	}

	inline void drawElementsBaseVertex(Primitive mode, int32_t vertexOffset, std::size_t indexOffset, int32_t indexCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElementsBaseVertex(to_value(mode), indexCount, GL_UNSIGNED_INT, std::bit_cast<void*>(sizeof(GLuint) * indexOffset), vertexOffset);
		checkGL();
	}

	inline void drawArraysInstanced(Primitive mode, int32_t vertexOffset, int32_t vertexCount, int32_t instanceCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawArraysInstanced(to_value(mode), vertexOffset, vertexCount, instanceCount);
		checkGL();
	}

	inline void drawElementsInstanced(Primitive mode, std::size_t indexOffset, int32_t indexCount, int32_t instanceCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElementsInstanced(to_value(mode), indexCount, GL_UNSIGNED_INT, std::bit_cast<const void*>(sizeof (GLuint) * indexOffset), instanceCount);
		checkGL();
	}

	inline void drawElementsInstancedBaseVertex(Primitive mode, int32_t vertexOffset, std::size_t indexOffset, int32_t indexCount, int32_t instanceCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElementsInstancedBaseVertex(to_value(mode), indexCount, GL_UNSIGNED_INT, std::bit_cast<void*>(sizeof(GLuint) * indexOffset), vertexOffset, instanceCount);
		checkGL();
	}

	inline void bindAttribute(const ArrayBuffer& buffer, const BaseAttribute& attribute, GLsizei stride, std::size_t offset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);
		if (attribute.channel == -1)
			return;

		glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
		glEnableVertexAttribArray(attribute.channel);
		glVertexAttribPointer(
				attribute.channel,
				attribute.dimension,
				libv::to_underlying(attribute.type),
				false,
				stride,
				std::bit_cast<const void*>(offset));
		checkGL();
	}

	inline void bindAttributeInt(const ArrayBuffer& buffer, const BaseAttribute& attribute, GLsizei stride, std::size_t offset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);
		if (attribute.channel == -1)
			return;

		glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
		glEnableVertexAttribArray(attribute.channel);
		glVertexAttribIPointer(
				attribute.channel,
				attribute.dimension,
				libv::to_underlying(attribute.type),
				stride,
				std::bit_cast<const void*>(offset));
		checkGL();
	}

	inline void bindAttributeDouble(const ArrayBuffer& buffer, const BaseAttribute& attribute, GLsizei stride, std::size_t offset) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);
		LIBV_GL_DEBUG_ASSERT(attribute.type == AttributeType::DOUBLE);
		if (attribute.channel == -1)
			return;

		glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
		glEnableVertexAttribArray(attribute.channel);
		glVertexAttribLPointer(
				attribute.channel,
				attribute.dimension,
				libv::to_underlying(attribute.type),
				stride,
				std::bit_cast<const void*>(offset));
		checkGL();
	}

	template <typename T, typename M>
	inline void bindAttribute(const ArrayBuffer& buffer, const BaseAttribute& attribute, M T::* member) noexcept {
		bindAttribute(
				buffer,
				attribute,
				sizeof(T),
				libv::member_offset(member));
	}

	template <typename T, typename M>
	inline void bindAttributeInt(const ArrayBuffer& buffer, const BaseAttribute& attribute, M T::* member) noexcept {
		bindAttributeInt(
				buffer,
				attribute,
				sizeof(T),
				libv::member_offset(member));
	}

	template <typename T, typename M>
	inline void bindAttributeDouble(const ArrayBuffer& buffer, const BaseAttribute& attribute, M T::* member) noexcept {
		bindAttributeDouble(
				buffer,
				attribute,
				sizeof(T),
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

	inline void attributeDivisor(GLuint channel, int32_t divisor) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glVertexAttribDivisor(channel, divisor);
		checkGL();
	}

	inline void attributeDivisor(const BaseAttribute& attribute, int32_t divisor) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glVertexAttribDivisor(attribute.channel, divisor);
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
