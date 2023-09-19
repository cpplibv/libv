// Project: libv.gl, File: src/libv/gl/indirect_command_buffer.hpp

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/indirect_command_buffer_object.hpp>
#include <libv/gl/basic_buffer.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct AccessIndirectCommandBuffer : BasicAccessBuffer<IndirectCommandBuffer, GL_DRAW_INDIRECT_BUFFER> {
public:
	using BasicAccessBuffer<IndirectCommandBuffer, GL_DRAW_INDIRECT_BUFFER>::BasicAccessBuffer;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
