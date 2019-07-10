// File: parser_align.hpp - Created on 2017.10.22. 06:09 - Author: Vader

#pragma once

// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class AlignVertical : uint32_t;
AlignVertical parse_align_vertical_or(const std::string_view str, const AlignVertical fallback);
AlignVertical parse_align_vertical_or_throw(const std::string_view str);
std::optional<AlignVertical> parse_align_vertical_optional(const std::string_view str);

enum class AlignHorizontal : uint32_t;
AlignHorizontal parse_align_horizontal_or(const std::string_view str, const AlignHorizontal fallback);
AlignHorizontal parse_align_horizontal_or_throw(const std::string_view str);
std::optional<AlignHorizontal> parse_align_horizontal_optional(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
