// Project: libv.re, File: src/libv/re/core/indirect_comand_buffer.cpp

#include <libv/re/core/indirect_comand_buffer_mapped.hpp>

#include <libv/gl/fence.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/indirect_command_buffer.hpp>
#include <libv/re/log.hpp>
#include <libv/utility/align.hpp>
#include <libv/utility/ceil_div.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

IndirectCommandBufferMapped::IndirectCommandBufferMapped(uint32_t reserveBytes) :
	reserveBytes(libv::align(reserveBytes, indirectCommandAlignment)) {
}

IndirectCommandBufferMapped::~IndirectCommandBufferMapped() {
}

void IndirectCommandBufferMapped::gl_create(libv::GL& gl) {
	created = true;

	for (auto& buffer : buffers) {
		using libv::gl::BufferAccess;
		using libv::gl::BufferStorage;

		gl(buffer.icbo).dsa_create();
		gl(buffer.icbo).dsa_storage(reserveBytes, nullptr, BufferStorage::Write | BufferStorage::Coherent | BufferStorage::Persistent);
		auto* dataPtr = gl(buffer.icbo).dsa_mapRange(0, reserveBytes, BufferAccess::Write | BufferAccess::Coherent | BufferAccess::Persistent);
		buffer.data = reinterpret_cast<std::byte*>(dataPtr);

		gl(buffer.fence).create();

		buffer.size = reserveBytes;
	}
	reserveBytes = 0;
}

void IndirectCommandBufferMapped::gl_swap(GL& gl) {
	const auto currentBufferIndex = generationIndex;
	auto& buffer = buffers[currentBufferIndex];

	if (!created) {
		gl_create(gl);
	} else {
		if (!gl(buffer.fence).clientWait())
			[[unlikely]] log.warn("IndirectCommandBufferMapped stalled the pipeline");

		gl(buffer.fence).destroy();
		gl(buffer.fence).create();
	}

	currentBufferOffset = 0;
	++generationIndex;
	if (generationIndex == numBuffers) // Wrap around
		generationIndex = 0;
}

IndirectCommandBufferOffset IndirectCommandBufferMapped::write(std::span<const libv::gl::IndirectCommandDrawElements> commands) {
	assert(created);
	const std::span<const std::byte> data = std::as_bytes(commands);

	const auto currentBufferIndex = generationIndex;
	auto& buffer = buffers[currentBufferIndex];
	const auto isOverflow = currentBufferOffset + data.size() > buffer.size;

	if (isOverflow && overflowData.empty()) // First request that overflows
		currentBufferOffset = buffer.size;

	if (!isOverflow) {
		std::memcpy(buffer.data + currentBufferOffset, data.data(), data.size());
	} else {
		const auto currentTmpOffset = libv::align_of_2(currentBufferOffset - buffer.size, indirectCommandAlignment);
		// TODO P4: This resize unnecessarily zero initializes, use a different container or a 'uninitialized' allocator
		overflowData.resize(currentTmpOffset + data.size());
		std::memcpy(overflowData.data() + currentTmpOffset, data.data(), data.size());
	}

	const auto offset = currentBufferOffset;
	currentBufferOffset = libv::align_of_2(currentBufferOffset + static_cast<uint32_t>(data.size()), indirectCommandAlignment);
	return offset;
}

void IndirectCommandBufferMapped::gl_upload(GL& gl) {
	assert(created);

	if (currentBufferOffset == 0)
		return;

	if (!overflowData.empty()) {
		const auto currentBufferIndex = generationIndex;
		auto& buffer = buffers[currentBufferIndex];

		// New size is requested size + 20% extra growth factor
		const auto newSize = libv::align((buffer.size + static_cast<uint32_t>(overflowData.size())) * 12u / 10u, indirectCommandAlignment);

		using libv::gl::BufferAccess;
		using libv::gl::BufferStorage;

		// Create new and copy over the important data
		GLSafeIndirectCommandBuffer icbo;
		gl(icbo).dsa_create();
		gl(icbo).dsa_storage(newSize, nullptr, BufferStorage::Write | BufferStorage::Coherent | BufferStorage::Persistent);
		auto* newDataPtr = gl(icbo).dsa_mapRange(0, newSize, BufferAccess::Write | BufferAccess::Coherent | BufferAccess::Persistent);
		auto* newData = reinterpret_cast<std::byte*>(newDataPtr);
		std::memcpy(newData, buffer.data, buffer.size);
		std::memcpy(newData + buffer.size, overflowData.data(), overflowData.size());

		// Update buffer
		std::swap(buffer.icbo.id, icbo.id);
		buffer.data = newData;
		buffer.size = newSize;

		// Cleanup
		overflowData.clear();
		gl(icbo).destroy();
	}
}

void IndirectCommandBufferMapped::gl_bind(libv::GL& gl) {
	const auto currentBufferIndex = generationIndex;
	auto& buffer = buffers[currentBufferIndex];
	gl(buffer.icbo).bind();
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
