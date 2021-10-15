// Project: libv.gl, File: src/libv/gl/array_buffer.hpp

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/basic_buffer.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct AccessArrayBuffer : BasicAccessBuffer<ArrayBuffer, GL_ARRAY_BUFFER> {
public:
	using BasicAccessBuffer<ArrayBuffer, GL_ARRAY_BUFFER>::BasicAccessBuffer;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
