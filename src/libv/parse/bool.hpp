// Project: libv.parse, File: src/libv/parse/bool.hpp

#pragma once

// std
#include <optional>
#include <string_view>


namespace libv {
namespace parse {

// -------------------------------------------------------------------------------------------------

bool parse_bool_or(const std::string_view str, const bool fallback);
bool parse_bool_or_throw(const std::string_view str);
std::optional<bool> parse_bool_optional(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace parse
} // namespace libv
