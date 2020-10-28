// Project: libv.math, File: src/libv/math/saturate.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T saturate(T value) noexcept {
	if (value < T{0})
		return T{0};
	else if (value > T{1})
		return T{1};
	else
		return value;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
