// Project: libv.ui, File: src/libv/ui/property/anchor.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/property/anchor.hpp>
// std
#include <cassert>
#include <ostream>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

static constexpr std::string_view storage_Anchor_to_string[] = {
	"top-left"     , // top_left
	"top-center"   , // top_center
	"top-right"    , // top_right
	"center-left"  , // center_left
	"center-center", // center_center
	"center-right" , // center_right
	"bottom-left"  , // bottom_left
	"bottom-center", // bottom_center
	"bottom-right" , // bottom_right
};

std::string_view Anchor::to_string() const noexcept {
	if (value >= std::size(storage_Anchor_to_string)) {
		assert(false && "Invalid Anchor enum value");
		return "<<invalid>>";
	}
	return storage_Anchor_to_string[value];
}

std::ostream& operator<<(std::ostream& os, const Anchor& var) {
	return os << var.to_string();
}

std::span<const Anchor> Anchor::list() noexcept {
	return detail::storage_Anchor;
}

static constexpr libv::vec3f storage_Anchor_to_info[] = {
	libv::vec3f{0.0f, 1.0f, 0.0f}, // top_left
	libv::vec3f{0.5f, 1.0f, 0.0f}, // top_center
	libv::vec3f{1.0f, 1.0f, 0.0f}, // top_right
	libv::vec3f{0.0f, 0.5f, 0.0f}, // center_left
	libv::vec3f{0.5f, 0.5f, 0.0f}, // center_center
	libv::vec3f{1.0f, 0.5f, 0.0f}, // center_right
	libv::vec3f{0.0f, 0.0f, 0.0f}, // bottom_left
	libv::vec3f{0.5f, 0.0f, 0.0f}, // bottom_center
	libv::vec3f{1.0f, 0.0f, 0.0f}, // bottom_right
};

libv::vec3f Anchor::to_info() const noexcept {
	if (value >= std::size(storage_Anchor_to_info)) {
		assert(false && "Invalid Anchor enum value");
		return storage_Anchor_to_info[0];
	}
	return storage_Anchor_to_info[value];
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
