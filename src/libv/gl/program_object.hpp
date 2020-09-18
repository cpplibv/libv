// File: program.hpp Author: Vader Created on 2016. július 8., 12:02

#pragma once

// std
#include <cstdint>
// pro
#include <libv/gl/gl_fwd.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class AccessProgram;

struct Program {
	uint32_t id = 0;

	template <typename Access = AccessProgram>
	inline Access operator()(GL&) noexcept {
		return Access{*this};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
