// Project: libv.re, File: src/libv/re/core/uniform_buffer.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/core/readback_query_index.hpp>
#include <libv/re/core/safe_gl.hpp>
#include <libv/re/fwd.hpp>

#include <array>
#include <span>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class ReadbackBufferMapped {
private:
	static constexpr uint32_t numBuffers = maxAsyncRingSize;

	struct Buffer {
		GLSafeShaderStorageBuffer coordinatesSSBO;
		GLSafeShaderStorageBuffer resultsSSBO;
		GLSafeFence fence;
		libv::vec2f* coordinatesData;
		const libv::vec4f* resultsData;
		uint32_t size;
	};

	uint32_t currentLoopIndex = 0;
	uint32_t currentBufferIndex = 0;
	uint32_t currentNumSamples = 0;
	std::array<Buffer, numBuffers> buffers;

	bool created = false;
	Shader_ptr shader;
	uint32_t reserveSamples = 0; /// Used only during init
	std::vector<libv::vec2f> overflowCoordinatesData; /// Used only during resizing

public:
	explicit ReadbackBufferMapped(uint32_t reserveSamples);
	~ReadbackBufferMapped();

private:
	void gl_create(libv::GL& gl);

public:
	void gl_swap(libv::GL& gl);
	void gl_fetch(libv::GL& gl, const Texture_ptr& texture);

public:
	/// Returned span remains valid until the next request() is called
	/// \thread Not thread safe, the pointers returned into overflowCoordinatesData can become invalidated (solvable)
	std::span<libv::vec2f> request(ReadbackQueryIndex& index, uint32_t count);

	/// Returned span remains valid until the next gl_swap is called
	/// Count must match the request's counts
	/// If result is not ready an empty span is returned, with repeated calls a valid result will arrive
	/// in at most asyncMaxRingSize (3-4) frame.
	std::span<const libv::vec4f> gl_result(libv::GL& gl, const ReadbackQueryIndex& index, uint32_t count);

	// struct RequestAndResult {
	// 	std::span<const libv::vec2f> request;
	// 	std::span<const libv::vec4f> result;
	// };
	// RequestAndResult gl_resultWithRequest(libv::GL& gl, const ReadbackQueryIndex& index, uint32_t count);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
