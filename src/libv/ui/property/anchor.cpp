// File:   align.cpp Author: Vader Created on 18 April 2019, 09:38

// hpp
#include <libv/ui/property/anchor.hpp>
// libv
#include <libv/utility/enum.hpp>
// std
#include <cassert>
#include <ostream>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

libv::vec3f TableAnchor[] = {
	libv::vec3f{0.0f, 1.0f, 0}, // top_left
	libv::vec3f{0.5f, 1.0f, 0}, // top_center
	libv::vec3f{1.0f, 1.0f, 0}, // top_right
	libv::vec3f{0.0f, 0.5f, 0}, // center_left
	libv::vec3f{0.5f, 0.5f, 0}, // center_center
	libv::vec3f{1.0f, 0.5f, 0}, // center_right
	libv::vec3f{0.0f, 0.0f, 0}, // bottom_left
	libv::vec3f{0.5f, 0.0f, 0}, // bottom_center
	libv::vec3f{1.0f, 0.0f, 0}, // bottom_right
};

[[nodiscard]] libv::vec3f to_info(const Anchor value) noexcept {
	if (libv::to_value(value) >= std::size(TableAnchor)) {
		assert(false && "Invalid Anchor enum value");
		return TableAnchor[0];
	}
	return TableAnchor[libv::to_value(value)];
}

std::string_view to_string(const Anchor value) noexcept {
	switch (value) {
	case Anchor::top_left:      return "top-left";
	case Anchor::top_center:    return "top-center";
	case Anchor::top_right:     return "top-right";
	case Anchor::center_left:   return "center-left";
	case Anchor::center_center: return "center-center";
	case Anchor::center_right:  return "center-right";
	case Anchor::bottom_left:   return "bottom-left";
	case Anchor::bottom_center: return "bottom-center";
	case Anchor::bottom_right:  return "bottom-right";
	}

	assert(false && "Invalid Anchor enum value");
	return "<<invalid>>";
}

std::ostream& operator<<(std::ostream& os, const Anchor value) {
	return os << to_string(value);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
