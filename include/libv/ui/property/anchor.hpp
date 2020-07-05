// File: size.hpp Author: Vader Created on 2017. augusztus 17., 0:04

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cstdint>
#include <iosfwd>
#include <string_view>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class Anchor : uint16_t {
	top_left      = 0,
	top_center    = 1,
	top_right     = 2,
	center_left   = 3,
	center_center = 4,
	center_right  = 5,
	bottom_left   = 6,
	bottom_center = 7,
	bottom_right  = 8,
};

// extern Anchor anchor_list[];
//[[nodiscard]] std::optional<Anchor> parse_anchor_optional(std::string_view text) noexcept;
//[[nodiscard]] Anchor parse_anchor_or(std::string_view text, const Anchor fallback) noexcept;
//[[nodiscard]] Anchor parse_anchor_or_throw(std::string_view text);

[[nodiscard]] libv::vec3f to_info(const Anchor value) noexcept;
[[nodiscard]] std::string_view to_string(const Anchor value) noexcept;
std::ostream& operator<<(std::ostream& os, const Anchor value);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
