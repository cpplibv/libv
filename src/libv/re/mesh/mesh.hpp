// Project: libv.re, File: src/libv/re/mesh/mesh.hpp

#pragma once

#include <libv/gl/enum.hpp>
#include <libv/gl/fwd.hpp>
#include <libv/meta/force_inline.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/common/attribute.hpp>
#include <libv/re/core/safe_gl.hpp>
#include <libv/re/math/bounding_sphere.hpp> // Convenience only
#include <libv/utility/member_offset.hpp>

#include <atomic>
#include <memory>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

using VertexIndex = int32_t;

// -------------------------------------------------------------------------------------------------

struct MeshBufferProxyParams {
	uint32_t bufferIndex = 0;
	std::byte* memory = nullptr;
	std::size_t size = 0;
};

template <typename T>
struct BaseMeshBufferProxy {
	using value_type = T;

private:
	uint32_t bufferIndex_ = 0;
	T* memory = nullptr;
	std::size_t size = 0;

public:
	explicit BaseMeshBufferProxy(MeshBufferProxyParams params) :
		bufferIndex_(params.bufferIndex),
		memory(reinterpret_cast<T*>(static_cast<void*>(params.memory))), // Alignment is enforced during alloc
		size(params.size) {}

public:
	constexpr LIBV_FORCE_INLINE T& operator[](std::size_t index) {
		assert(index < size);
		return memory[index];
	}
	constexpr LIBV_FORCE_INLINE const T& operator[](std::size_t index) const {
		assert(index < size);
		return memory[index];
	}
	constexpr LIBV_FORCE_INLINE T* data() {
		return memory;
	}
	constexpr LIBV_FORCE_INLINE const T* data() const {
		return memory;
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE uint32_t bufferIndex() const noexcept {
		return bufferIndex_;
	}
};

template <typename T>
struct MeshVertexBufferProxy : BaseMeshBufferProxy<T> {
	using BaseMeshBufferProxy<T>::BaseMeshBufferProxy;
};

template <typename T>
struct MeshAttributeBufferProxy : BaseMeshBufferProxy<T> {
	using BaseMeshBufferProxy<T>::BaseMeshBufferProxy;
};

// -------------------------------------------------------------------------------------------------

class ColdMeshData;

class Mesh : public libv::ref_count_base {
public:
	friend libv::ref_count_access;
	using ptr = Mesh_ptr;
	using cptr = Mesh_cptr;

private:
	std::atomic_bool dirty = false;

private:
	friend SceneInternal;
	bool updatePending = false;

private:
	// OpenGL handler - GL Thread access only
	GLSafeVertexArray vao;
public:
	libv::gl::Primitive primitive = libv::gl::Primitive::Triangles;
private:
	libv::gl::AttributeType indexType = libv::gl::AttributeType::UINT;
	int32_t vertexCount_ = -1;
	int32_t indexCount_ = -1;

	// Cold data
	const std::unique_ptr<ColdMeshData> cold;

public:
	Mesh();
	virtual ~Mesh();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}

public:
	virtual BoundingSphere boundingSphere() const;

private:
	MeshBufferProxyParams auxAddBuffer(libv::gl::BufferUsage usage, std::size_t size, std::size_t count);
	MeshBufferProxyParams auxAddAttribute(libv::gl::BufferUsage usage, int32_t channel, libv::gl::AttributeType type, int32_t dimension, std::size_t count, uint32_t offset, uint32_t stride, int32_t divisor);
	MeshBufferProxyParams auxAddIndex(libv::gl::BufferUsage usage, libv::gl::AttributeType type, std::size_t count);
	void auxAddAttributeToBuffer(uint32_t bufferIndex, int32_t channel, libv::gl::AttributeType type, int32_t dimension, uint32_t offset, uint32_t stride, int32_t divisor);

public:
	template <int Channel, typename T>
	MeshAttributeBufferProxy<T> addAttribute(libv::gl::BufferUsage usage, libv::gl::Attribute<Channel, T> attribute, std::size_t count);
	template <int Channel, typename T>
	MeshAttributeBufferProxy<T> addInstanceAttribute(libv::gl::BufferUsage usage, libv::gl::Attribute<Channel, T> attribute, std::size_t count, int32_t divisor);
	template <typename T>
	MeshAttributeBufferProxy<T> addIndex(libv::gl::BufferUsage usage, std::size_t count);

	template <typename T>
	MeshVertexBufferProxy<T> addBuffer(libv::gl::BufferUsage usage, std::size_t count);
	template <typename T, int Channel, typename M>
	void addAttribute(MeshVertexBufferProxy<T>& buffer, libv::gl::Attribute<Channel, M> attribute, M T::* memberPtr);
	template <typename T, int Channel, typename M>
	void addInstanceAttribute(MeshVertexBufferProxy<T>& buffer, libv::gl::Attribute<Channel, M> attribute, M T::* memberPtr, int32_t divisor);

public:
	[[nodiscard]] constexpr inline int32_t indexCount() const noexcept {
		return indexCount_;
	}
	[[nodiscard]] constexpr inline int32_t vertexCount() const noexcept {
		return vertexCount_;
	}

public:
	virtual void gl_update(ContextUpdate& ctx);
	virtual void gl_render(ContextRender& ctx);
	virtual void gl_render(ContextRender& ctx, uint32_t indexOffset, uint32_t indexCount);
	virtual void gl_renderBaseVertexBaseInstance(ContextRender& ctx, int32_t vertexOffset, std::size_t indexOffset, int32_t indexCount, int32_t instanceOffset, int32_t instanceCount);
	virtual void gl_renderIndirect(ContextRender& ctx, uint32_t indirectCommandBufferOffset, uint32_t indirectCommandCount);
};

// -------------------------------------------------------------------------------------------------

template <int Channel, typename T>
MeshAttributeBufferProxy<T> Mesh::addAttribute(libv::gl::BufferUsage usage, libv::gl::Attribute<Channel, T> attribute, std::size_t count) {
	return MeshAttributeBufferProxy<T>{auxAddAttribute(
			usage,
			attribute.channel,
			attribute.type,
			attribute.dimension,
			count,
			0,
			sizeof(T),
			0)};
}

template <int Channel, typename T>
MeshAttributeBufferProxy<T> Mesh::addInstanceAttribute(libv::gl::BufferUsage usage, libv::gl::Attribute<Channel, T> attribute, std::size_t count, int32_t divisor) {
	return MeshAttributeBufferProxy<T>{auxAddAttribute(
			usage,
			attribute.channel,
			attribute.type,
			attribute.dimension,
			count,
			0,
			sizeof(T),
			divisor)};
}

template <typename T>
MeshAttributeBufferProxy<T> Mesh::addIndex(libv::gl::BufferUsage usage, std::size_t count) {
	return MeshAttributeBufferProxy<T>{auxAddIndex(
			usage,
			libv::gl::toAttributeType<T>(),
			count)};
}

template <typename T>
MeshVertexBufferProxy<T> Mesh::addBuffer(libv::gl::BufferUsage usage, std::size_t count) {
	return MeshVertexBufferProxy<T>{auxAddBuffer(
			usage,
			sizeof(T),
			count)};
}

template <typename T, int Channel, typename M>
void Mesh::addAttribute(MeshVertexBufferProxy<T>& buffer, libv::gl::Attribute<Channel, M> attribute, M T::* memberPtr) {
	auxAddAttributeToBuffer(buffer.bufferIndex(),
			attribute.channel,
			attribute.type,
			attribute.dimension,
			static_cast<uint32_t>(libv::member_offset(memberPtr)),
			sizeof(T),
			0);
}

template <typename T, int Channel, typename M>
void Mesh::addInstanceAttribute(MeshVertexBufferProxy<T>& buffer,libv::gl::Attribute<Channel,M> attribute,M T::* memberPtr,int32_t divisor) {
	auxAddAttributeToBuffer(buffer.bufferIndex(),
			attribute.channel,
			attribute.type,
			attribute.dimension,
			static_cast<uint32_t>(libv::member_offset(memberPtr)),
			sizeof(T),
			divisor);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
