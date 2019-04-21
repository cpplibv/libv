// File:   orient.hpp Author: Vader Created on 22 March 2019, 08:02

#pragma once

// std
#include <cstdint>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// TODO P4: Adopt Orientation in layout_line

enum class Orientation2 : uint32_t {
	BOTTOM_TO_TOP = 0,
	LEFT_TO_RIGHT = 1,
	RIGHT_TO_LEFT = 2,
	TOP_TO_BOTTOM = 3,
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
