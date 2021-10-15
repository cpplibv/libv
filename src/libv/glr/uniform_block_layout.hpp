// Project: libv.glr, File: src/libv/glr/uniform_block_layout.hpp

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
