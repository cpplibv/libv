// File: vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// std
#include <cstdint>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

/** @note Vertex Arrays cannot be shared between OpenGL contexts */
struct VertexArray {
	uint32_t id = 0;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv