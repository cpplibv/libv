// Project: libv.gl, File: src/libv/gl/vertex_array_object.hpp

#pragma once

// std
#include <cstdint>
// pro
#include <libv/gl/gl_fwd.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class AccessVertexArray;

/** @note Vertex Arrays cannot be shared between OpenGL contexts */
struct VertexArray {
	uint32_t id = 0;

	template <typename Access = AccessVertexArray>
	inline Access operator()(GL&) noexcept {
		return Access{*this};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
