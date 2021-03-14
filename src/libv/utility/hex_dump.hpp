// Project: libv.utility, File: src/libv/utility/hex_dump.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <span>
#include <string>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string hex_dump(const std::string_view& s);
[[nodiscard]] std::string hex_dump(const std::span<const std::byte> s);
[[nodiscard]] std::string hex_dump_with_ascii(const std::string_view& s);
[[nodiscard]] std::string hex_dump_with_ascii(const std::span<const std::byte> s);

// -------------------------------------------------------------------------------------------------

} // namespace libv
