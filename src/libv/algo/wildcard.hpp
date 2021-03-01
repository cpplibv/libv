// Project: libv.algorithm, File: src/libv/algorithm/wildcard.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Supported pattern control character sequences:
/// '?' - Matches a single character
/// '*' - Matches zero or more character
bool match_wildcard(const std::string_view str, const std::string_view pattern) noexcept;

/// Supported pattern control character sequences:
/// '/' - The layer separator
/// '\\' - Pattern escape prefix
/// '?' - Matches a single character except the layer separator
/// '*' - Matches zero or more character except the layer separator
/// '**' - Matches zero or more character including the layer separator
bool match_wildcard_glob(const std::string_view str, const std::string_view pattern_str) noexcept;

// -------------------------------------------------------------------------------------------------

} // namespace libv
