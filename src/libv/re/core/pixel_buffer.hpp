// Project: libv.re, File: src/libv/re/core/uniform_buffer.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/common/async_ring_size.hpp>
#include <libv/re/core/safe_gl.hpp>
#include <libv/re/fwd.hpp>

#include <array>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

// NOTE: Unless the readback data is large and continuous use the ReadbackBufferMapped instead

class PixelBuffer {
private:
	static constexpr uint32_t numBuffers = maxAsyncRingSize;

	bool created = false;

	uint32_t currentIndex = 0;
	std::array<const void*, numBuffers> data;
	std::array<GLSafePixelBuffer, numBuffers> pbos;
	std::array<GLSafeFence, numBuffers> fences;

	libv::vec2ui pixelCount;
	libv::gl::FormatBase dataFormat;
	libv::gl::DataType dataType;

public:
	explicit PixelBuffer(libv::vec2ui pixelCount, libv::gl::FormatBase dataFormat, libv::gl::DataType dataType);
	~PixelBuffer();

private:
	void gl_create(libv::GL& gl);

public:
	void gl_asyncFetch(libv::GL& gl, const Texture_ptr& texture, uint32_t level, libv::vec2ui offset);
	// Returned data pointer remains valid until the next gl_fetchAsync is called
	const void* gl_lastResult(libv::GL& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
