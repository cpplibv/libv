// File:   std140.hpp Author: Vader Created on 2018. december 19., 17:20

#pragma once

// std
#include <cstdint>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct UniformBlockLayout : T {
	uint32_t size;
	uint32_t binding;
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
