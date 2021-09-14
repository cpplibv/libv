// Project: libv.sys, File: src/libv/sys/clipboard.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string>


namespace libv {
namespace sys {

// -------------------------------------------------------------------------------------------------

/// Converts any incoming crlf to lf
[[nodiscard]] std::string clipboard_text();

void clipboard_text(const std::string& string);

// -------------------------------------------------------------------------------------------------

} // namespace sys
} // namespace libv
