// Project: libv.ui, File: src/libv/ui/property/scroll_area_mode.hpp, Author: Császár Mátyás [Vader]

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
