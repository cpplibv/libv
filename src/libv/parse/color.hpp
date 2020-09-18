// Project: libv.parse, File: src/libv/parse/color.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <optional>
#include <string_view>


namespace libv {
namespace parse {

// -------------------------------------------------------------------------------------------------

libv::vec4f parse_color_or(const std::string_view str, const libv::vec4f fallback);
libv::vec4f parse_color_or_throw(const std::string_view str);
std::optional<libv::vec4f> parse_color_optional(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace parse
} // namespace libv
