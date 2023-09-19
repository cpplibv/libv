// Project: libv.re, File: src/libv/re/core/uniform_buffer.cpp

#include <libv/re/core/pixel_buffer.hpp>

#include <libv/gl/fence.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/pixel_buffer.hpp>
#include <libv/gl/texture.hpp>
#include <libv/re/log.hpp>
#include <libv/re/resource/texture.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

PixelBuffer::PixelBuffer(libv::vec2ui pixelCount, libv::gl::FormatBase dataFormat, libv::gl::DataType dataType) :
		pixelCount(pixelCount),
		dataFormat(dataFormat),
		dataType(dataType) {
}

PixelBuffer::~PixelBuffer() {
}

void PixelBuffer::gl_create(libv::GL& gl) {
	created = true;

	using libv::gl::BufferAccess;
	using libv::gl::BufferStorage;

	const auto bufferSize = pixelCount.x * pixelCount.y * libv::gl::sizeInfo(dataType).size * libv::gl::dimInfo(dataFormat);
	for (uint32_t i = 0; i < numBuffers; ++i) {
		auto& pbo = pbos[i];
		gl(pbo).dsa_create();

		gl(pbo).dsa_storage(bufferSize, nullptr, BufferStorage::Read | BufferStorage::Coherent | BufferStorage::Persistent);
		data[i] = gl(pbo).dsa_mapRange(0, bufferSize, BufferAccess::Read | BufferAccess::Coherent | BufferAccess::Persistent);
	}
}

void PixelBuffer::gl_asyncFetch(GL& gl, const Texture_ptr& texture, uint32_t level, libv::vec2ui offset) {
	if (!created)
		gl_create(gl);

	const auto bufferSize = pixelCount.x * pixelCount.y * libv::gl::sizeInfo(dataType).size * libv::gl::dimInfo(dataFormat);

	{
		auto tex = texture->gl_id(gl);
		auto pbo_guard = gl(pbos[currentIndex]).bind_guard();
		gl(tex).dsa_getTextureSubImage(level, offset.x, offset.y, 0, pixelCount.x, pixelCount.y, 1, dataFormat, dataType, bufferSize, nullptr);
	}

	if (fences[currentIndex].id)
		gl(fences[currentIndex]).destroy();
	gl(fences[currentIndex]).create();

	++currentIndex;
	if (currentIndex == numBuffers) // Wrap around
		currentIndex = 0;
}

const void* PixelBuffer::gl_lastResult(GL& gl) {
	// TODO P5: Find freshest data (relatable examples in readback buffer)
	if (fences[currentIndex].id) {
		if (!gl(fences[currentIndex]).clientWait())
			[[unlikely]] log.warn("Pixel read buffer stalled the pipeline");
	}

	return data[currentIndex];
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
