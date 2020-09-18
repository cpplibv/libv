// File: vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// pro
#include <libv/gl/buffer_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class AccessUniformBuffer;

struct UniformBuffer : Buffer {
	template <typename Access = AccessUniformBuffer>
	inline Access operator()(GL&) noexcept {
		return Access{*this};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
