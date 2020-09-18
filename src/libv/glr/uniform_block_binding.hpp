// File: program.hpp - Created on 2017.12.20. 11:27 - Author: Vader

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
