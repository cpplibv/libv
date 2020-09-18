// Project: libv.ui, File: src/libv/ui/property/align.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/property/align.hpp>
// std
#include <cassert>
#include <ostream>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

static constexpr std::string_view storage_AlignVertical_to_string[] = {
	"top"        , // top
	"center"     , // center
	"bottom"     , // bottom
	"justify"    , // justify
	"justify-all", // justify_all
};

std::string_view AlignVertical::to_string() const noexcept {
	if (value >= std::size(storage_AlignVertical_to_string)) {
		assert(false && "Invalid AlignVertical enum value");
		return "<<invalid>>";
	}
	return storage_AlignVertical_to_string[value];
}

std::ostream& operator<<(std::ostream& os, const AlignVertical& var) {
	return os << var.to_string();
}

std::span<const AlignVertical> AlignVertical::list() noexcept {
	return detail::storage_AlignVertical;
}

static constexpr float storage_AlignVertical_to_info[] = {
	1.0f, // top
	0.5f, // center
	0.0f, // bottom
	0.0f, // justify
	0.0f, // justify_all
};

float AlignVertical::to_info() const noexcept {
	if (value >= std::size(storage_AlignVertical_to_info)) {
		assert(false && "Invalid AlignVertical enum value");
		return storage_AlignVertical_to_info[0];
	}
	return storage_AlignVertical_to_info[value];
}

// -------------------------------------------------------------------------------------------------

static constexpr std::string_view storage_AlignHorizontal_to_string[] = {
	"left"       , // left
	"center"     , // center
	"right"      , // right
	"justify"    , // justify
	"justify-all", // justify_all
};

std::string_view AlignHorizontal::to_string() const noexcept {
	if (value >= std::size(storage_AlignHorizontal_to_string)) {
		assert(false && "Invalid AlignHorizontal enum value");
		return "<<invalid>>";
	}
	return storage_AlignHorizontal_to_string[value];
}

std::ostream& operator<<(std::ostream& os, const AlignHorizontal& var) {
	return os << var.to_string();
}

std::span<const AlignHorizontal> AlignHorizontal::list() noexcept {
	return detail::storage_AlignHorizontal;
}

static constexpr float storage_AlignHorizontal_to_info[] = {
	0.0f, // left
	0.5f, // center
	1.0f, // right
	0.0f, // justify
	0.0f, // justify_all
};

float AlignHorizontal::to_info() const noexcept {
	if (value >= std::size(storage_AlignHorizontal_to_info)) {
		assert(false && "Invalid AlignHorizontal enum value");
		return storage_AlignHorizontal_to_info[0];
	}
	return storage_AlignHorizontal_to_info[value];
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
