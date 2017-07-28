// File: parser_size.hpp - Created on 2017.10.22. 06:09 - Author: Vader

#pragma once

// std
#include <string_view>
#include <optional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Size;
std::optional<Size> parse_size(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
