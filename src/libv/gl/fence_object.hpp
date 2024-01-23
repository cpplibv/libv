// Project: libv.gl, File: src/libv/gl/fence_object.hpp

#pragma once

// std
#include <cstdint>
// pro
// #include <libv/gl/enum.hpp>


namespace libv {
namespace gl {

// Fence -------------------------------------------------------------------------------------------

struct Fence {
	void* id = nullptr;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
