// Project: libv.parse, File: src/libv/ui/parse/parse_align.hpp

#pragma once

// libv
#include <libv/ui/property/align.hpp>
// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::optional<AlignVertical> parse_align_vertical_optional(const std::string_view str);
AlignVertical parse_align_vertical_or(const std::string_view str, const AlignVertical fallback);
AlignVertical parse_align_vertical_or_throw(const std::string_view str);

std::optional<AlignHorizontal> parse_align_horizontal_optional(const std::string_view str);
AlignHorizontal parse_align_horizontal_or(const std::string_view str, const AlignHorizontal fallback);
AlignHorizontal parse_align_horizontal_or_throw(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
