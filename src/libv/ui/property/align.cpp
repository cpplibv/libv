// File:   align.cpp Author: Vader Created on 18 April 2019, 09:38

// hpp
#include <libv/ui/property/align.hpp>
// std
#include <cassert>
#include <ostream>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const AlignVertical value) {
	switch (value) {
	case AlignVertical::Top: return os << "top";
	case AlignVertical::Center: return os << "center";
	case AlignVertical::Bottom: return os << "bottom";
	case AlignVertical::Justify: return os << "justify";
	case AlignVertical::JustifyAll: return os << "justify-all";
	}

	assert(false && "Invalid AlignVertical enum value");
	return os << "<<invalid>>";
}

std::ostream& operator<<(std::ostream& os, const AlignHorizontal value) {
	switch (value) {
	case AlignHorizontal::Left: return os << "left";
	case AlignHorizontal::Center: return os << "center";
	case AlignHorizontal::Right: return os << "right";
	case AlignHorizontal::Justify: return os << "justify";
	case AlignHorizontal::JustifyAll: return os << "justify-all";
	}

	assert(false && "Invalid AlignHorizontal enum value");
	return os << "<<invalid>>";
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
