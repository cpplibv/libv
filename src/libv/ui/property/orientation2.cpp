// File:   align.cpp Author: Vader Created on 18 April 2019, 09:38

// hpp
#include <libv/ui/property/orientation2.hpp>
// std
#include <ostream>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Orientation2 value) {
	switch (value) {
	case Orientation2::DOWN_LEFT: return os << "down-left";
	case Orientation2::DOWN_RIGHT: return os << "down-right";
	case Orientation2::LEFT_DOWN: return os << "left-down";
	case Orientation2::LEFT_UP: return os << "left-up";
	case Orientation2::RIGHT_DOWN: return os << "right-down";
	case Orientation2::RIGHT_UP: return os << "right-up";
	case Orientation2::UP_LEFT: return os << "up-left";
	case Orientation2::UP_RIGHT: return os << "up-right";
	default: return os << "<<invalid>>";
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv