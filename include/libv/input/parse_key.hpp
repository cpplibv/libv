// File: inputs.hpp Author: Vader Created on 2017. január 24., 3:11

#pragma once

// std
#include <optional>
#include <string_view>
// pro
#include <libv/input/input.hpp>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

Key parse_key_or(const std::string_view str, const Key fallback);
Key parse_key_or_throw(const std::string_view str);
std::optional<Key> parse_key_optional(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace input
} // namespace libv
