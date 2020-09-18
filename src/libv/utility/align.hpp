// Project: libv.utility, File: src/libv/utility/align.hpp, Author: Császár Mátyás [Vader]

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
