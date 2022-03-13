// Project: libv.parse, File: src/libv/ui/parse/parse_orientation2.hpp

#pragma once

// libv
#include <libv/ui/property/orientation2.hpp>
// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::optional<Orientation2> parse_orientation2_optional(const std::string_view str);
Orientation2 parse_orientation2_or(const std::string_view str, const Orientation2 fallback);
Orientation2 parse_orientation2_or_throw(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
