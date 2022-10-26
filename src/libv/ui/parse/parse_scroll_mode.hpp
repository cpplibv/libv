// Project: libv.parse, File: src/libv/ui/parse/parse_scroll_mode.hpp

#pragma once

// libv
#include <libv/ui/property/scroll_mode.hpp>
// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::optional<ScrollMode> parse_scroll_mode_optional(const std::string_view str);
ScrollMode parse_scroll_mode_or(const std::string_view str, const ScrollMode fallback);
ScrollMode parse_scroll_mode_or_throw(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
