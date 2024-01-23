// Project: libv.re, File: src/libv/re/core/indirect_comand_buffer_mapped.hpp

#pragma once

#include <libv/re/common/async_ring_size.hpp>
#include <libv/gl/fwd.hpp>
#include <libv/gl/indirect_command.hpp>
#include <libv/re/core/safe_gl.hpp>

#include <array>
#include <span>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

using IndirectCommandBufferOffset = uint32_t;

struct IndirectCommandBufferMapped {
private:
	static constexpr uint32_t numBuffers = maxAsyncRingSize;
	static constexpr uint32_t indirectCommandAlignment = sizeof(uint32_t);

	struct Buffer {
		GLSafeIndirectCommandBuffer icbo;
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
	explicit IndirectCommandBufferMapped(uint32_t reserveBytes);
	~IndirectCommandBufferMapped();

private:
	void gl_create(libv::GL& gl);
public:
	void gl_swap(libv::GL& gl);
	IndirectCommandBufferOffset write(std::span<const libv::gl::IndirectCommandDrawElements> commands);
	void gl_upload(libv::GL& gl);
	void gl_bind(libv::GL& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
