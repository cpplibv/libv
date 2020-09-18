// Project: libv.math, File: src/libv/math/remap.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline auto remap(T value, T from_lo, T from_hi, T to_lo, T to_hi) noexcept {
	return to_lo + (value - from_lo) * (to_hi - to_lo) / (from_hi - from_lo);
}

template <typename T>
[[nodiscard]] constexpr inline auto remap_clamp(T value, T from_lo, T from_hi, T to_lo, T to_hi) noexcept {
	auto result = remap(value, from_lo, from_hi, to_lo, to_hi);

	if (to_lo < to_hi)
		return std::clamp(result, to_lo, to_hi);
	else
		return std::clamp(result, to_hi, to_lo);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
