// Project: libv.glr, File: src/libv/glr/uniform_block_binding.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstdint>
#include <string>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

using UniformBlockBindingLocation = uint32_t;

struct UniformBlockBinding {
	UniformBlockBindingLocation binding;
	std::string identifier;
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
