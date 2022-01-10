// Project: libv.utility, File: src/libv/utility/int_ceil_div.hpp

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T int_ceil_div(const T x, const T y) noexcept {
	return (x + y - 1) / y;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
