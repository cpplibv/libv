// File: vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// std
#include <cstdint>


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
