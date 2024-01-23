// Project: libv.parse, File: src/libv/ui/parse/parse_focus_mode.hpp

#pragma once

// libv
#include <libv/ui/property/focus_mode.hpp>
// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::optional<FocusMode> parse_focus_mode_optional(const std::string_view str);
FocusMode parse_focus_mode_or(const std::string_view str, const FocusMode fallback);
FocusMode parse_focus_mode_or_throw(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
