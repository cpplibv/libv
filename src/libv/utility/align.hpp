// File:   slice.hpp Author: Vader Created on 2018. május 28., 19:03

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T align(const T value, const T alignment) noexcept {
	return (alignment - 1 + value) / alignment * alignment;
}

template <typename T>
[[nodiscard]] constexpr inline T align_of_2(const T value, const T alignment) noexcept {
	return (alignment - 1 + value) & ~(alignment - 1);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
