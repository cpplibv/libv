// Project: libv.gl, File: src/libv/gl/vertex_array_object.hpp

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
