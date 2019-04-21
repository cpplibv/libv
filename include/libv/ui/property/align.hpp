// File:   color.hpp Author: Vader Created on 19 March 2019, 00:59

#pragma once

// std
#include <iosfwd>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class AlignVertical {
	Top = 0,
	Center = 1,
	Bottom = 2,
	Justify = 3, /// Apply top alignment on the last line and justify every other line
	JustifyAll = 4, /// Apply justify to every line including the last line
};

std::ostream& operator<<(std::ostream& os, const AlignVertical value);

enum class AlignHorizontal {
	Left = 0,
	Center = 1,
	Right = 2,
	Justify = 3, /// Apply left alignment on the last line and justify every other line
	JustifyAll = 4, /// Apply justify to every line including the last line
};

std::ostream& operator<<(std::ostream& os, const AlignHorizontal value);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
