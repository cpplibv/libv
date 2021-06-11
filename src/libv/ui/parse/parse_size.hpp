// Project: libv.parse, File: src/libv/ui/parse/parse_size.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Size;
std::optional<Size> parse_size_optional(const std::string_view str);
Size parse_size_or(const std::string_view str, const Size& fallback);
Size parse_size_or_throw(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
