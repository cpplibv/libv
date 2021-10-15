// Project: libv.gl, File: src/libv/gl/shader_object.hpp

#pragma once

// std
#include <cstdint>
// pro
#include <libv/gl/gl_fwd.hpp>


namespace libv {
namespace gl {

// Shader ------------------------------------------------------------------------------------------

class AccessShader;

struct Shader {
	uint32_t id = 0;

	template <typename Access = AccessShader>
	inline Access operator()(GL&) noexcept {
		return Access{*this};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
