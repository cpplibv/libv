// File: vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/assert.hpp>
#include <libv/gl/basic_buffer.hpp>
#include <libv/gl/check.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct AccessArrayBuffer : BasicAccessBuffer<ArrayBuffer, GL_ARRAY_BUFFER> {
public:
	using BasicAccessBuffer<ArrayBuffer, GL_ARRAY_BUFFER>::BasicAccessBuffer;

public:
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
