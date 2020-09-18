// Project: libv.ui, File: src/libv/ui/property/orientation.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/property/orientation.hpp>
// std
#include <ostream>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Orientation value) {
	switch (value) {
	case Orientation::BOTTOM_TO_TOP: return os << "bottom-to-top";
	case Orientation::LEFT_TO_RIGHT: return os << "left-to-right";
	case Orientation::RIGHT_TO_LEFT: return os << "right-to-left";
	case Orientation::TOP_TO_BOTTOM: return os << "top-to-bottom";
	}
	return os << "<<invalid>>";
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
