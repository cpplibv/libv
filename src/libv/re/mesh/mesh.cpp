// Project: libv.re, File: src/libv/re/mesh/mesh.cpp

#include <libv/re/mesh/mesh.hpp>

#include <libv/container/small_vector.hpp>
#include <libv/gl/array_buffer.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/vertex_array.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/utility/memory/unique.hpp>

#include <forward_list>
#include <mutex>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Mesh* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Mesh* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

class ColdMeshData {
	struct Buffer {
		struct DataChunk {
			bool entireBuffer = true;
			std::size_t offset = 0;
			std::size_t size = 0;
			std::unique_ptr<std::byte[]> data = nullptr;
		};

		GLSafeArrayBuffer object;
		libv::gl::BufferUsage usage = libv::gl::BufferUsage::StaticDraw;
		std::size_t size = 0;

		std::forward_list<DataChunk> uploadDataQueue;

	public:
		explicit Buffer(gl::BufferUsage usage) : usage(usage) {}

	public:
		std::byte* queueEntireMemoryWrite(std::size_t dataSize) {
			size = dataSize;
			auto dataChunk = libv::make_unique_uninitialized<std::byte[]>(dataSize);
			auto* writableMemory = dataChunk.get();

			auto last = uploadDataQueue.before_begin();
			auto it = last;
			while (++it != uploadDataQueue.end())
				last = it;
			uploadDataQueue.emplace_after(last, true, 0, dataSize, std::move(dataChunk));

			return writableMemory;
		}

		// Consumes and uploads the attribute.dataChain
		// Also updates the indexCount_ and vertexCount_
		void upload(libv::GL& gl) {
			for (; !uploadDataQueue.empty(); uploadDataQueue.pop_front()) {
				auto& task = uploadDataQueue.front();
				if (task.entireBuffer) {
					gl(object).dsa_data(nullptr, task.size, usage); // Buffer orphaning / allocation
					gl(object).dsa_subData(0, task.data.get(), task.size);
				} else {
					gl(object).dsa_subData(task.offset, task.data.get(), task.size);
				}
			}
		}
	};

	struct AttributeInfo {
		uint32_t bufferIndex = 0;
		AttributeIndex channel = 0;
		libv::gl::AttributeType type = libv::gl::AttributeType::FLOAT;
		int32_t dimension = 0;
		int32_t divisor = 0;
		uint32_t offset = 0;
		uint32_t stride = 0;
	};

public:
	std::mutex mutex;
	bool created = false;
	// A typical layout is: 1 indexBuffer, 3 attributeBuffer, 10 attribute
	// 		Index Buffer: index
	// 		Attribute Buffer 0: position
	// 		Attribute Buffer 1: uv, normal, tangent, boneIndices, boneWeight, ...
	// 		Attribute Buffer 2: instanceMatRow0, instanceMatRow1, instanceMatRow2, instanceVariation
	std::optional<Buffer> indexBuffer;
	libv::small_vector<Buffer, 3> attributeBuffers;
	libv::small_vector<AttributeInfo, 10> attributes;
};

// =================================================================================================

Mesh::Mesh() :
	cold(std::make_unique<ColdMeshData>()) {
}

Mesh::~Mesh() {
	// For the sake of forward declared types
}

BoundingSphere Mesh::boundingSphere() const {
	return {libv::vec3f{0, 0, 0}, 1.f};
}

// -------------------------------------------------------------------------------------------------

MeshBufferProxyParams Mesh::auxAddBuffer(libv::gl::BufferUsage usage, std::size_t size, std::size_t count) {
	auto lock = std::unique_lock(cold->mutex);
	dirty.store(true, std::memory_order_release);
	assert(!cold->created);

	const auto bufferIndex = static_cast<uint32_t>(cold->attributeBuffers.size());
	const auto dataSize = size * count;
	auto& buffer = cold->attributeBuffers.emplace_back(usage);
	auto* writableMemory = buffer.queueEntireMemoryWrite(dataSize);

	return {
			bufferIndex,
			writableMemory,
			count};
}

MeshBufferProxyParams Mesh::auxAddAttribute(
		libv::gl::BufferUsage usage,
		int32_t channel,
		libv::gl::AttributeType type,
		int32_t dimension,
		std::size_t count,
		uint32_t offset,
		uint32_t stride,
		int32_t divisor) {
	auto lock = std::unique_lock(cold->mutex);
	dirty.store(true, std::memory_order_release);
	assert(!cold->created);

	if (divisor == 0) {
		if (vertexCount_ == -1) {
			vertexCount_ = static_cast<int32_t>(count);
		} else {
			assert(vertexCount_ == static_cast<int32_t>(count));
		}
	}

	const auto bufferIndex = static_cast<uint32_t>(cold->attributeBuffers.size());
	const auto dataSize = libv::gl::attributeTypeSize(type) * dimension * count;
	auto& buffer = cold->attributeBuffers.emplace_back(usage);
	auto* writableMemory = buffer.queueEntireMemoryWrite(dataSize);

	cold->attributes.emplace_back(
			bufferIndex,
			channel,
			type,
			dimension,
			divisor,
			offset,
			stride);

	return {
			bufferIndex,
			writableMemory,
			count};
}

MeshBufferProxyParams Mesh::auxAddIndex(libv::gl::BufferUsage usage, libv::gl::AttributeType type, std::size_t count) {
	auto lock = std::unique_lock(cold->mutex);
	dirty.store(true, std::memory_order_release);
	assert(!cold->created);
	assert(!cold->indexBuffer);

	indexCount_ = static_cast<int32_t>(count);
	indexType = type;

	const auto dataSize = libv::gl::attributeTypeSize(type) * count;
	auto& buffer = cold->indexBuffer.emplace(usage);
	auto* writableMemory = buffer.queueEntireMemoryWrite(dataSize);

	return {
			std::numeric_limits<uint32_t>::max(),
			writableMemory,
			count};
}

void Mesh::auxAddAttributeToBuffer(
		uint32_t bufferIndex,
		int32_t channel,
		libv::gl::AttributeType type,
		int32_t dimension,
		uint32_t offset,
		uint32_t stride,
		int32_t divisor) {
	auto lock = std::unique_lock(cold->mutex);
	dirty.store(true, std::memory_order_release);
	assert(!cold->created);

	if (divisor == 0) {
		assert(offset < stride); // Vertex count calculation would break if it isn't a nicely interleaved buffer
		const auto count = static_cast<int32_t>(cold->attributeBuffers[bufferIndex].size / stride);
		if (vertexCount_ == -1) {
			vertexCount_ = count;
		} else {
			assert(vertexCount_ == count);
		}
	}

	cold->attributes.emplace_back(
			bufferIndex,
			channel,
			type,
			dimension,
			divisor,
			offset,
			stride);
}

// -------------------------------------------------------------------------------------------------

void Mesh::gl_update(ContextUpdate& ctx) {
	if (!dirty.load(std::memory_order_acquire))
		return;

	auto lock = std::unique_lock(cold->mutex);
	dirty.store(false, std::memory_order_release);

	if (!cold->created) {
		cold->created = true;
		ctx.gl(vao).dsa_create();

		if (cold->indexBuffer) {
			ctx.gl(cold->indexBuffer->object).dsa_create();
			ctx.gl(vao).dsa_bindElements(cold->indexBuffer->object);
		}

		for (auto& buffer : cold->attributeBuffers)
			ctx.gl(buffer.object).dsa_create();

		uint32_t bindingPoint = 0;
		for (const auto& attribute : cold->attributes) {
			ctx.gl(vao).dsa_attributeFormat(attribute.channel, bindingPoint, attribute.dimension, attribute.type);
			ctx.gl(vao).dsa_bindBuffer(bindingPoint, cold->attributeBuffers[attribute.bufferIndex].object, attribute.offset, attribute.stride);
			if (attribute.divisor != 0)
				ctx.gl(vao).dsa_attributeBindingDivisor(bindingPoint, attribute.divisor);
			++bindingPoint;
		}
	}

	if (cold->indexBuffer)
		cold->indexBuffer->upload(ctx.gl);
	for (auto& buffer : cold->attributeBuffers)
		buffer.upload(ctx.gl);
}

void Mesh::gl_render(ContextRender& ctx) {
	assert(indexCount_ != -1);
	assert(vertexCount_ != -1);

	ctx.gl(vao).bind();
	ctx.gl(vao).drawElements(primitive, 0, indexCount_);
}

void Mesh::gl_render(ContextRender& ctx, uint32_t indexOffset, uint32_t indexCount) {
	assert(indexCount_ != -1);
	assert(vertexCount_ != -1);

	ctx.gl(vao).bind();
	ctx.gl(vao).drawElements(primitive, indexOffset, static_cast<int32_t>(indexCount));
}

void Mesh::gl_renderBaseVertexBaseInstance(ContextRender& ctx, int32_t vertexOffset, std::size_t indexOffset, int32_t indexCount_, int32_t instanceOffset, int32_t instanceCount) {
	assert(indexCount_ != -1);
	assert(vertexCount_ != -1);

	ctx.gl(vao).bind();
	ctx.gl(vao).drawElementsInstancedBaseVertexBaseInstance(primitive, indexType, vertexOffset, indexOffset, indexCount_, instanceOffset, instanceCount);
}

void Mesh::gl_renderInstanced(ContextRender& ctx, int32_t instanceCount) {
	assert(indexCount_ != -1);
	assert(vertexCount_ != -1);

	ctx.gl(vao).bind();
	ctx.gl(vao).drawElementsInstancedBaseVertexBaseInstance(primitive, indexType, 0, 0, indexCount_, 0, instanceCount);
}

void Mesh::gl_renderIndirect(ContextRender& ctx, uint32_t indirectCommandBufferOffset, uint32_t indirectCommandCount) {
	assert(indexCount_ != -1);
	assert(vertexCount_ != -1);

	ctx.gl(vao).bind();
	ctx.gl(vao).drawMultiElementsIndirect(primitive, indexType, indirectCommandBufferOffset, indirectCommandCount);
}

// ctx.gl(vao).drawElements(primitive, indexOffset, indexCount_);
// ctx.gl(vao).drawElementsBaseVertex(primitive, vertexOffset, indexOffset, indexCount_);
// ctx.gl(vao).drawArraysInstanced(primitive, 0, vertexCount_, instanceCount);
// ctx.gl(vao).drawElementsInstanced(primitive, 0, indexCount_, instanceCount);

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
