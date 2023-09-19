// Project: libv.re, File: src/libv/re/core/uniform_buffer.cpp

#include <libv/re/core/uniform_buffer_mapped.hpp>

#include <libv/gl/fence.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/uniform_buffer.hpp>
#include <libv/re/log.hpp>
#include <libv/re/uniform/uniform_block.hpp>
#include <libv/utility/align.hpp>
#include <libv/utility/ceil_div.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

UniformBufferMapped::UniformBufferMapped(uint32_t reserveBytes) :
	reserveBytes(libv::align(reserveBytes, uniformBlockAlignment)) {
}

UniformBufferMapped::~UniformBufferMapped() {
}

void UniformBufferMapped::gl_create(libv::GL& gl) {
	created = true;

	for (auto& buffer : buffers) {
		using libv::gl::BufferAccess;
		using libv::gl::BufferStorage;

		gl(buffer.ubo).dsa_create();
		gl(buffer.ubo).dsa_storage(reserveBytes, nullptr, BufferStorage::Write | BufferStorage::Coherent | BufferStorage::Persistent);
		auto* dataPtr = gl(buffer.ubo).dsa_mapRange(0, reserveBytes, BufferAccess::Write | BufferAccess::Coherent | BufferAccess::Persistent);
		buffer.data = reinterpret_cast<std::byte*>(dataPtr);

		gl(buffer.fence).create();

		buffer.size = reserveBytes;
	}
	reserveBytes = 0;
}

void UniformBufferMapped::gl_swap(GL& gl) {
	const auto currentBufferIndex = generationIndex & bufferIndexMask;
	auto& buffer = buffers[currentBufferIndex];

	if (!created) {
		gl_create(gl);
	} else {
		if (!gl(buffer.fence).clientWait())
			[[unlikely]] log.warn("UniformBufferMapped stalled the pipeline");

		gl(buffer.fence).destroy();
		gl(buffer.fence).create();
	}

	currentBufferOffset = 0;
	++generationIndex;
	if ((generationIndex & bufferIndexMask) == numBuffers) {
		// Wrap around
		generationIndex &= ~bufferIndexMask;
		generationIndex += bufferIndexMask + 1u;
	}
}

UniformBufferOffset UniformBufferMapped::auxWrite(std::span<const std::byte> data) {
	assert(created);

	const auto currentBufferIndex = generationIndex & bufferIndexMask;
	auto& buffer = buffers[currentBufferIndex];
	const auto isOverflow = currentBufferOffset + data.size() > buffer.size;

	if (isOverflow && overflowData.empty()) // First request that overflows
		currentBufferOffset = buffer.size;

	if (!isOverflow) {
		std::memcpy(buffer.data + currentBufferOffset, data.data(), data.size());
	} else {
		const auto currentTmpOffset = libv::align_of_2(currentBufferOffset - buffer.size, uniformBlockAlignment);
		// TODO P4: This resize unnecessarily zero initializes, use a different container or a 'uninitialized' allocator
		overflowData.resize(currentTmpOffset + data.size());
		std::memcpy(overflowData.data() + currentTmpOffset, data.data(), data.size());
	}

	const auto offset = currentBufferOffset;
	currentBufferOffset = libv::align_of_2(currentBufferOffset + static_cast<uint32_t>(data.size()), uniformBlockAlignment);
	return offset;
}

void UniformBufferMapped::write(UniformBlockBufferPlacement& placement, std::span<const std::byte> data) {
	assert(created);

	// NOTE: This 'generationIndex' in the placement could yield false positive matching
	// whenever the request index was not used for exactly ~2^30 frame, we ignore this scenario.
	// As buffers only grow never shrink, garbage data will be read but there will not be invalid memory access.
	if (placement.generationIndex == generationIndex)
		return;

	const auto offset = auxWrite(data);

	placement.bufferOffset = offset;
	placement.generationIndex = generationIndex;
}

void UniformBufferMapped::gl_upload(GL& gl) {
	assert(created);

	if (currentBufferOffset == 0)
		return;

	if (!overflowData.empty()) {
		const auto currentBufferIndex = generationIndex & bufferIndexMask;
		auto& buffer = buffers[currentBufferIndex];

		// New size is requested size + 20% extra growth factor
		const auto newSize = libv::align_of_2((buffer.size + static_cast<uint32_t>(overflowData.size())) * 12u / 10u, uniformBlockAlignment);

		using libv::gl::BufferAccess;
		using libv::gl::BufferStorage;

		// Create new and copy over the important data
		GLSafeUniformBuffer newUBO;
		gl(newUBO).dsa_create();
		gl(newUBO).dsa_storage(newSize, nullptr, BufferStorage::Write | BufferStorage::Coherent | BufferStorage::Persistent);
		auto* newDataPtr = gl(newUBO).dsa_mapRange(0, newSize, BufferAccess::Write | BufferAccess::Coherent | BufferAccess::Persistent);
		auto* newData = reinterpret_cast<std::byte*>(newDataPtr);
		std::memcpy(newData, buffer.data, buffer.size); // Not sure if memcpy is acceptable between mapped buffers, could use glCopyNamedBufferSubData if need to be
		std::memcpy(newData + buffer.size, overflowData.data(), overflowData.size());

		// Update buffer
		std::swap(buffer.ubo.id, newUBO.id);
		buffer.data = newData;
		buffer.size = newSize;

		// Cleanup
		overflowData.clear();
		gl(newUBO).destroy();
	}
}

void UniformBufferMapped::gl_bind(libv::GL& gl, BlockBinding binding, uint32_t offset, uint32_t size) {
	const auto currentBufferIndex = generationIndex & bufferIndexMask;
	auto& buffer = buffers[currentBufferIndex];
	gl(buffer.ubo).bindRange(+binding, offset, size);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
