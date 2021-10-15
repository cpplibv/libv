// Project: libv.utility, File: src/libv/utility/trim.hpp

#pragma once

// std
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline std::string_view trim_front(std::string_view s) noexcept {
    s.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
    return s;
}

[[nodiscard]] constexpr inline std::string_view trim_back(std::string_view s) noexcept {
    s.remove_suffix(std::min(s.size() - s.find_last_not_of(" \t\r\v\n") - 1, s.size()));
    return s;
}

[[nodiscard]] constexpr inline std::string_view trim(std::string_view s) noexcept {
	return trim_front(trim_back(s));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
