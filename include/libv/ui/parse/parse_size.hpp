// File: parser_size.hpp - Created on 2017.10.22. 06:09 - Author: Vader

#pragma once

// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Size;
Size parse_size_or(const std::string_view str, const Size fallback);
Size parse_size_or_throw(const std::string_view str);
std::optional<Size> parse_size_optional(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
