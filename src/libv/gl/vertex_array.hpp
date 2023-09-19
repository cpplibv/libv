// Project: libv.gl, File: src/libv/gl/vertex_array.hpp

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/utility/guard.hpp>
#include <libv/utility/member_offset.hpp>
// std
#include <bit>
#include <span>
// pro
#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/assert.hpp>
#include <libv/gl/attribute.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/indirect_command.hpp>
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

	inline void dsa_create() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		glCreateVertexArrays(1, &object.id);
		checkGL();
	}

	inline void destroy() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.cleanupDestroyedVertexArray(object.id);
		glDeleteVertexArrays(1, &object.id);
		object.id = 0;
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

		return libv::guard([*this] mutable {
			unbind();
		});
	}

	inline void drawArrays(Primitive primitive, int32_t vertexOffset, int32_t vertexCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawArrays(+primitive, vertexOffset, vertexCount);
		checkGL();
	}

	inline void drawElements(Primitive primitive, std::size_t indexOffset, int32_t indexCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElements(+primitive, indexCount, GL_UNSIGNED_INT, std::bit_cast<const void*>(sizeof (GLuint) * indexOffset));
		checkGL();
	}

	inline void drawElementsBaseVertex(Primitive primitive, int32_t vertexOffset, std::size_t indexOffset, int32_t indexCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElementsBaseVertex(+primitive, indexCount, GL_UNSIGNED_INT, std::bit_cast<void*>(sizeof(GLuint) * indexOffset), vertexOffset);
		checkGL();
	}

	inline void drawArraysInstanced(Primitive primitive, int32_t vertexOffset, int32_t vertexCount, int32_t instanceCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawArraysInstanced(+primitive, vertexOffset, vertexCount, instanceCount);
		checkGL();
	}

	inline void drawElementsInstanced(Primitive primitive, std::size_t indexOffset, int32_t indexCount, int32_t instanceCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElementsInstanced(+primitive, indexCount, GL_UNSIGNED_INT, std::bit_cast<const void*>(sizeof (GLuint) * indexOffset), instanceCount);
		checkGL();
	}

	inline void drawElementsInstancedBaseVertex(Primitive primitive, int32_t vertexOffset, std::size_t indexOffset, int32_t indexCount, int32_t instanceCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElementsInstancedBaseVertex(+primitive, indexCount, GL_UNSIGNED_INT, std::bit_cast<void*>(sizeof(GLuint) * indexOffset), vertexOffset, instanceCount);
		checkGL();
	}

	inline void drawElementsInstancedBaseVertexBaseInstance(Primitive primitive, AttributeType indexType, int32_t vertexOffset, std::size_t indexOffset, int32_t indexCount, int32_t instanceOffset, int32_t instanceCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDrawElementsInstancedBaseVertexBaseInstance(+primitive, indexCount, +indexType, std::bit_cast<void*>(sizeof(GLuint) * indexOffset), instanceCount, vertexOffset, instanceOffset);
		checkGL();
	}

	// This only works in compat mode as core does not support reading commands from client memory
	// inline void drawMultiElementsIndirect(Primitive primitive, AttributeType indexType, std::span<libv::gl::IndirectCommandDrawElements> commands) const noexcept {
	// 	LIBV_GL_DEBUG_ASSERT(object.id != 0);
	// 	glMultiDrawElementsIndirect(
	// 			+primitive,
	// 			+indexType,
	// 			commands.data(),
	// 			static_cast<int32_t>(commands.size()),
	// 			sizeof(libv::gl::IndirectCommandDrawElements));
	// 	checkGL();
	// }

	inline void drawMultiElementsIndirect(Primitive primitive, AttributeType indexType, uint32_t indirectCommandBufferOffset, uint32_t indirectCommandCount) const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glMultiDrawElementsIndirect(
				+primitive,
				+indexType,
				reinterpret_cast<const void*>(indirectCommandBufferOffset),
				indirectCommandCount,
				sizeof(libv::gl::IndirectCommandDrawElements));
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

	///
	/// @param buffer
	/// @param channel
	/// @param size
	/// @param dimension
	/// @param stride - the byte offset between two entry
	/// @param offset
	///
	inline void dsa_bindAttribute(const ArrayBuffer& buffer, GLuint channel, uint32_t dimension, AttributeType type, uint32_t offset, uint32_t stride) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);

		// TODO P5: Expose more of the DSA Vertex Array API. Add support for interleaved binding from the same VBO
		//		Split this function into format definition

		switch (type) {
		case AttributeType::BYTE:
		case AttributeType::INT:
		case AttributeType::SHORT:
		case AttributeType::UBYTE:
		case AttributeType::UINT:
		case AttributeType::USHORT:
			glVertexArrayAttribIFormat(
					object.id,
					channel,
					dimension,
					to_value(type),
					0);
			break;

		case AttributeType::FLOAT:
		case AttributeType::HFLOAT:
			glVertexArrayAttribFormat(
					object.id,
					channel,
					dimension,
					to_value(type),
					false,
					0);
			break;

		case AttributeType::DOUBLE:
			glVertexArrayAttribLFormat(
					object.id,
					channel,
					dimension,
					to_value(type),
					0);
			break;

		default: // TODO P5: libv.gl: instead of default log and assert invalid input
		case AttributeType::FIXED:
		case AttributeType::INT_2_10_10_10_REV:
		case AttributeType::UNSIGNED_INT_10F_11F_11F_REV:
		case AttributeType::UNSIGNED_INT_2_10_10_10_REV:
			// not supported
			break;
		}

		const auto binding = channel; // Current limited API will create 1 binding point for each attribute
		glVertexArrayAttribBinding(object.id, channel, binding);
		glVertexArrayVertexBuffer(object.id, binding, buffer.id, offset, stride);
		glEnableVertexArrayAttrib(object.id, channel);
		checkGL();
	}

	inline void dsa_attributeFormat(GLuint channel, uint32_t bufferBindingPoint, uint32_t dimension, AttributeType type) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		switch (type) {
		case AttributeType::BYTE:
		case AttributeType::INT:
		case AttributeType::SHORT:
		case AttributeType::UBYTE:
		case AttributeType::UINT:
		case AttributeType::USHORT:
			glVertexArrayAttribIFormat(
					object.id,
					channel,
					dimension,
					to_value(type),
					0);
			break;

		case AttributeType::FLOAT:
		case AttributeType::HFLOAT:
			glVertexArrayAttribFormat(
					object.id,
					channel,
					dimension,
					to_value(type),
					false,
					0);
			break;

		case AttributeType::DOUBLE:
			glVertexArrayAttribLFormat(
					object.id,
					channel,
					dimension,
					to_value(type),
					0);
			break;

		default: // TODO P5: libv.gl: instead of default log and assert invalid input
		case AttributeType::FIXED:
		case AttributeType::INT_2_10_10_10_REV:
		case AttributeType::UNSIGNED_INT_10F_11F_11F_REV:
		case AttributeType::UNSIGNED_INT_2_10_10_10_REV:
			// not supported
			break;
		}

		glVertexArrayAttribBinding(object.id, channel, bufferBindingPoint);
		glEnableVertexArrayAttrib(object.id, channel);
		checkGL();
	}

	inline void dsa_bindBuffer(uint32_t bufferBindingPoint, const ArrayBuffer& buffer, uint32_t offset, uint32_t stride) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);

		glVertexArrayVertexBuffer(object.id, bufferBindingPoint, buffer.id, offset, static_cast<int32_t>(stride));
		checkGL();
	}

	inline void bindElements(const ArrayBuffer& buffer) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id);
		checkGL();
	}

	inline void dsa_bindElements(const ArrayBuffer& buffer) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		LIBV_GL_DEBUG_ASSERT(buffer.id != 0);
		glVertexArrayElementBuffer(object.id, buffer.id);
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

	inline void dsa_attributeBindingDivisor(GLuint channel, int32_t divisor) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glVertexArrayBindingDivisor(object.id, channel, divisor);
		checkGL();
	}
	inline void dsa_attributeBindingDivisor(const BaseAttribute& attribute, int32_t divisor) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glVertexArrayBindingDivisor(object.id, attribute.channel, divisor);
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
