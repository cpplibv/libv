// File:   orient.hpp Author: Vader Created on 22 March 2019, 08:02

#pragma once

// std
#include <cstdint>
#include <iosfwd>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class Orientation : uint32_t {
	BOTTOM_TO_TOP = 0,
	LEFT_TO_RIGHT = 1,
	RIGHT_TO_LEFT = 2,
	TOP_TO_BOTTOM = 3,
};

std::ostream& operator<<(std::ostream& os, const Orientation value);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
