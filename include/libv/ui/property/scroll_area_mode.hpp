// File:   color.hpp Author: Vader Created on 19 March 2019, 00:59

#pragma once

// std
#include <cstdint>
//#include <iosfwd>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class ScrollAreaMode : uint8_t {
	both,
	horizontal,
	vertical,
};

//std::ostream& operator<<(std::ostream& os, const ScrollAreaMode value);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
