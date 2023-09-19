// Project: libv.re, File: src/libv/re/core/uniform_buffer.cpp

#include <libv/re/core/readback_buffer_mapped.hpp>

#include <libv/gl/fence.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/shader_storage_buffer.hpp>
#include <libv/re/log.hpp>
#include <libv/re/resource/shader.hpp>
#include <libv/re/resource/shader_loader.hpp>
#include <libv/re/resource/texture.hpp>
#include <libv/utility/align.hpp>
#include <libv/utility/ceil_div.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

ReadbackBufferMapped::ReadbackBufferMapped(uint32_t reserveSamples) :
	reserveSamples(reserveSamples) {
	shader = r.shader.load("shader/re/readback_sampler.cs");
}

ReadbackBufferMapped::~ReadbackBufferMapped() {
}

void ReadbackBufferMapped::gl_create(libv::GL& gl) {
	created = true;

	const auto numAllocatedSample = libv::align_of_2(reserveSamples, 64u);

	for (auto& buffer : buffers) {
		using libv::gl::BufferAccess;
		using libv::gl::BufferStorage;

		const auto coordinatesBufferSize = static_cast<uint32_t>(numAllocatedSample * sizeof(libv::vec2f));
		gl(buffer.coordinatesSSBO).dsa_create();
		gl(buffer.coordinatesSSBO).dsa_storage(coordinatesBufferSize, nullptr, BufferStorage::Write | BufferStorage::Coherent | BufferStorage::Persistent);
		auto* coordinatesPtr = gl(buffer.coordinatesSSBO).dsa_mapRange(0, coordinatesBufferSize, BufferAccess::Write | BufferAccess::Coherent | BufferAccess::Persistent);
		buffer.coordinatesData = reinterpret_cast<libv::vec2f*>(coordinatesPtr);

		const auto resultsBufferSize = static_cast<uint32_t>(numAllocatedSample * sizeof(libv::vec4f));
		gl(buffer.resultsSSBO).dsa_create();
		gl(buffer.resultsSSBO).dsa_storage(resultsBufferSize, nullptr, BufferStorage::Read | BufferStorage::Coherent | BufferStorage::Persistent);
		const auto* resultsPtr = gl(buffer.resultsSSBO).dsa_mapRange(0, resultsBufferSize, BufferAccess::Read | BufferAccess::Coherent | BufferAccess::Persistent);
		buffer.resultsData = reinterpret_cast<const libv::vec4f*>(resultsPtr);

		gl(buffer.fence).create();

		buffer.size = numAllocatedSample;
	}
}

void ReadbackBufferMapped::gl_swap(GL& gl) {
	if (!created)
		gl_create(gl);

	currentNumSamples = 0;
	++currentBufferIndex;
	if (currentBufferIndex == numBuffers) { // Wrap around
		currentBufferIndex = 0;
		++currentLoopIndex;
	}
}

std::span<libv::vec2f> ReadbackBufferMapped::request(ReadbackQueryIndex& index, uint32_t count) {
	auto& buffer = buffers[currentBufferIndex];
	const auto isOverflow = currentNumSamples + count > buffer.size;

	if (isOverflow && overflowCoordinatesData.empty()) // First request that overflows
		currentNumSamples = buffer.size;

	const auto dataIndex = static_cast<uint16_t>(currentNumSamples);
	const auto loopIndex = static_cast<uint16_t>(currentLoopIndex);

	index.set(currentBufferIndex, loopIndex, dataIndex);
	currentNumSamples += count;

	if (!isOverflow) {
		return std::span(buffer.coordinatesData + dataIndex, count);
	} else {
		const auto tmpIndex = overflowCoordinatesData.size();
		// TODO P4: This resize unnecessarily zero initializes, use a different container or a 'uninitialized' allocator
		overflowCoordinatesData.resize(overflowCoordinatesData.size() + count);
		return std::span(overflowCoordinatesData.data() + tmpIndex, count);
	}
}

void ReadbackBufferMapped::gl_fetch(GL& gl, const Texture_ptr& texture) {
	assert(created);

	if (currentNumSamples == 0)
		return;

	auto& buffer = buffers[currentBufferIndex];

	if (currentNumSamples > buffer.size) {
		// New size is requested size + 20% extra growth factor
		const auto newSize = (buffer.size + static_cast<uint32_t>(overflowCoordinatesData.size())) * 12u / 10u;

		using libv::gl::BufferAccess;
		using libv::gl::BufferStorage;

		// Create new and copy over the important data
		GLSafeShaderStorageBuffer newCoordinatesSSBO;
		const auto newCoordinatesBufferSize = static_cast<uint32_t>(newSize * sizeof(libv::vec2f));
		gl(newCoordinatesSSBO).dsa_create();
		gl(newCoordinatesSSBO).dsa_storage(newCoordinatesBufferSize, nullptr, BufferStorage::Write | BufferStorage::Coherent | BufferStorage::Persistent);
		auto* newCoordinatesPtr = gl(newCoordinatesSSBO).dsa_mapRange(0, newCoordinatesBufferSize, BufferAccess::Write | BufferAccess::Coherent | BufferAccess::Persistent);
		auto* newCoordinatesData = reinterpret_cast<libv::vec2f*>(newCoordinatesPtr);
		std::memcpy(newCoordinatesData, buffer.coordinatesData, buffer.size * sizeof(libv::vec2f));
		std::memcpy(newCoordinatesData + buffer.size, overflowCoordinatesData.data(), overflowCoordinatesData.size() * sizeof(libv::vec2f));

		GLSafeShaderStorageBuffer newResultsSSBO;
		const auto newResultsBufferSize = static_cast<uint32_t>(newSize * sizeof(libv::vec4f));
		gl(newResultsSSBO).dsa_create();
		gl(newResultsSSBO).dsa_storage(newResultsBufferSize, nullptr, BufferStorage::Read | BufferStorage::Coherent | BufferStorage::Persistent);
		const auto* newResultsPtr = gl(newResultsSSBO).dsa_mapRange(0, newResultsBufferSize, BufferAccess::Read | BufferAccess::Coherent | BufferAccess::Persistent);
		const auto* newResultsData = reinterpret_cast<const libv::vec4f*>(newResultsPtr);

		// Update buffer
		std::swap(buffer.coordinatesSSBO.id, newCoordinatesSSBO.id);
		buffer.coordinatesData = newCoordinatesData;
		std::swap(buffer.resultsSSBO.id, newResultsSSBO.id);
		buffer.resultsData = newResultsData;
		buffer.size = newSize;

		// Cleanup
		overflowCoordinatesData.clear();
		gl(newCoordinatesSSBO).destroy();
		gl(newResultsSSBO).destroy();
	}

	// Fill the unused coordinates with zeros
	for (auto i = currentNumSamples; i < libv::align_of_2(currentNumSamples, 64u); ++i)
		buffer.coordinatesData[i] = libv::vec2f{0, 0};

	// TODO P4: Not sure if this is a desired usage pattern? Use a material or a new compute material?
	// auto prevProgram = gl.boundProgram();
	shader->gl_bind(gl);
	texture->gl_bind(gl, unitDiffuse);
	gl(buffer.coordinatesSSBO).bindBase(0);
	gl(buffer.resultsSSBO).bindBase(1);
	gl.dispatchCompute(libv::ceil_div(currentNumSamples, 64u), 1, 1);
	// gl.useProgram(prevProgram);

	gl(buffer.fence).destroy();
	gl(buffer.fence).create();
}

std::span<const libv::vec4f> ReadbackBufferMapped::gl_result(libv::GL& gl, const ReadbackQueryIndex& index, uint32_t count) {
	assert(created);
	assert(index.dataIndex(currentBufferIndex) + count < buffers[currentBufferIndex].size);

	const auto bufferOldest = currentBufferIndex + 1 == numBuffers ? 0 : currentBufferIndex + 1;

	auto bufferIt = currentBufferIndex;
	auto loopIt = currentLoopIndex;
	while (true) {
		// NOTE: This 'loop index key' in the query index could yield false positive matching
		// whenever the request index was not used exactly for numBuffers*2^16 +/- numBuffers frame,
		// we ignore this scenario and return whatever data it finds but guard against invalid memory access.
		if (index.loopIndex(bufferIt) == loopIt) {
			// Matching loop index: Query has a request in this buffer
			const auto falseMatchCausesProblem = index.dataIndex(bufferIt) + count > buffers[bufferIt].size;
			if (!falseMatchCausesProblem) {
				if (bufferIt != bufferOldest) {
					if (gl(buffers[bufferIt].fence).ready())
						return std::span(buffers[bufferIt].resultsData + index.dataIndex(bufferIt), count);
				} else {
					// This is the oldest buffer, and none of the newer ones were ready, if we must wait on this
					if (!gl(buffers[bufferIt].fence).clientWait())
						[[unlikely]] log.warn("Readback buffer stalled the pipeline");

					return std::span(buffers[bufferIt].resultsData + index.dataIndex(bufferIt), count);
				}
			}
		}

		if (bufferIt == 0) {
			bufferIt = numBuffers - 1;
			--loopIt;
		} else {
			--bufferIt;
		}

		if (bufferIt == currentBufferIndex)
			// We have looped around, nothing was ready and the oldest buffer has no request for this query
			break;
	}

	return {};
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
