// Project: libv.ui, File: src/libv/ui/component/layout/layout_utility.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

constexpr inline libv::vec2f swizzle(libv::vec2f value0, uint32_t dim0, uint32_t dim1) {
	return libv::vec2f{value0[dim0], value0[dim1]};
}

constexpr inline libv::vec2f swizzle(libv::vec3f value0, uint32_t dim0, uint32_t dim1) {
	return libv::vec2f{value0[dim0], value0[dim1]};
}

constexpr inline libv::vec2f unswizzle(libv::vec2f value0, uint32_t dim0, uint32_t dim1) {
	libv::vec2f result;
	result[dim0] = value0.x;
	result[dim1] = value0.y;
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
