// Project: libv.gl, File: src/libv/gl/pixel_buffer.hpp

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/basic_buffer.hpp>
#include <libv/gl/pixel_buffer_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct AccessPixelBuffer : BasicAccessBuffer<PixelBuffer, GL_PIXEL_PACK_BUFFER> {
public:
	using BasicAccessBuffer<PixelBuffer, GL_PIXEL_PACK_BUFFER>::BasicAccessBuffer;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
