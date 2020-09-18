// File:   orient.hpp Author: Vader Created on 22 March 2019, 08:02

#pragma once

// std
#include <cstdint>
#include <iosfwd>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class Orientation2 : uint32_t {
	DOWN_LEFT = 0,
	DOWN_RIGHT = 1,
	LEFT_DOWN = 2,
	LEFT_UP = 3,
	RIGHT_DOWN = 4,
	RIGHT_UP = 5,
	UP_LEFT = 6,
	UP_RIGHT = 7,
};

std::ostream& operator<<(std::ostream& os, const Orientation2 value);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
