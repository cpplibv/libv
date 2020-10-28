// Project: libv.math, File: src/libv/math/mix.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T mix(T v0, T v1, T t) noexcept {
	return (T{1} - t) * v0 + t * v1;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
