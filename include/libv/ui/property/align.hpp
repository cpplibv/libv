// File:   color.hpp Author: Vader Created on 19 March 2019, 00:59

#pragma once

// std
#include <cstdint>
#include <iosfwd>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class AlignVertical : uint16_t {
	Top = 0,
	Center = 1,
	Bottom = 2,
	// TODO P4: libv.ui: it would be nice to have a baseline vertical alignment
	Justify = 3, /// Apply top alignment on the last line and justify every other line
	JustifyAll = 4, /// Apply justify to every line including the last line
};

std::ostream& operator<<(std::ostream& os, const AlignVertical value);

enum class AlignHorizontal : uint16_t {
	Left = 0,
	Center = 1,
	Right = 2,
	// TODO P5: libv.ui: it would be nice to have a baseline horizontal alignment, even if it wont be ever used, and is basically a left align, still.
	Justify = 3, /// Apply left alignment on the last line and justify every other line
	JustifyAll = 4, /// Apply justify to every line including the last line
};

std::ostream& operator<<(std::ostream& os, const AlignHorizontal value);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
