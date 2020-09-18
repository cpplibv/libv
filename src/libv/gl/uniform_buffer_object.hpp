// Project: libv.gl, File: src/libv/gl/uniform_buffer_object.hpp, Author: Császár Mátyás [Vader]

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
