// Project: libv.utility, File: src/libv/utility/ceil_div.hpp

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Integer division with ceil instead of floor if there is any remainder
template <typename T, typename K>
[[nodiscard]] constexpr inline T ceil_div(const T x, const K y) noexcept {
	return (x + y - 1) / y;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
