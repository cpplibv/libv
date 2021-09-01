// Project: libv.parse, File: src/libv/ui/parse/parse_anchor.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/ui/property/anchor.hpp>
// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::optional<Anchor> parse_anchor_optional(const std::string_view str);
Anchor parse_anchor_or(const std::string_view str, const Anchor fallback);
Anchor parse_anchor_or_throw(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
