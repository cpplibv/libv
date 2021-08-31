// Project: libv.parse, File: src/libv/ui/parse/parse_align.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/ui/property/orientation.hpp>
// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::optional<Orientation> parse_orientation_optional(const std::string_view str);
Orientation parse_orientation_or(const std::string_view str, const Orientation fallback);
Orientation parse_orientation_or_throw(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
