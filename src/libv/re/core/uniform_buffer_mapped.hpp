// Project: libv.re, File: src/libv/re/core/uniform_buffer.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/common/async_ring_size.hpp>
#include <libv/re/common/block_binding.hpp>
#include <libv/re/core/safe_gl.hpp>
#include <libv/re/fwd.hpp>

#include <array>
#include <cstdint>
#include <span>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

using UniformBufferOffset = uint32_t;

struct UniformBufferMapped {
private:
	static constexpr uint32_t numBuffers = maxAsyncRingSize;
	static constexpr uint32_t bufferIndexMask = std::bit_ceil(numBuffers + 1u) - 1u;
	static constexpr uint32_t uniformBlockAlignment = 256;

	struct Buffer {
		GLSafeUniformBuffer ubo;
		GLSafeFence fence;
		std::byte* data;
		uint32_t size;
	};

	uint32_t generationIndex = 0;
	uint32_t currentBufferOffset = 0;
	std::array<Buffer, numBuffers> buffers;

	bool created = false;
	uint32_t reserveBytes = 0; /// Used only during init
	std::vector<std::byte> overflowData; /// Used only during resizing

public:
	explicit UniformBufferMapped(uint32_t reserveBytes);
	~UniformBufferMapped();

private:
	void gl_create(libv::GL& gl);
	[[nodiscard]] UniformBufferOffset auxWrite(std::span<const std::byte> data);
public:
	void gl_swap(libv::GL& gl);
	void write(UniformBlockBufferPlacement& placement, std::span<const std::byte> data);
	void gl_upload(libv::GL& gl);
	void gl_bind(libv::GL& gl, BlockBinding binding, uint32_t offset, uint32_t size);

public:
	template <typename T>
	[[nodiscard]] UniformBufferOffset write(const T& data) {
		return auxWrite(std::span(reinterpret_cast<const std::byte*>(&data), sizeof(T)));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
