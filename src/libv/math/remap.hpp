// Project: libv.math, File: src/libv/math/remap.hpp

#pragma once

// std
#include <algorithm> // For std::clamp


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Remaps the value from [from_lo..from_hi] to [to_lo..to_hi] with extrapolation
template <typename T>
[[nodiscard]] constexpr inline T remap(T value, T from_lo, T from_hi, T to_lo, T to_hi) noexcept {
	return to_lo + (value - from_lo) * (to_hi - to_lo) / (from_hi - from_lo);
}

/// Remap but key and value types can differ.
template <typename K, typename V>
[[nodiscard]] constexpr inline V remap_key(K value, K from_lo, K from_hi, V to_lo, V to_hi) noexcept {
	return to_lo + (value - from_lo) * (to_hi - to_lo) / (from_hi - from_lo);
}

/// Remap but clamps the result to [to_lo..to_hi] (disables extrapolation)
template <typename T>
[[nodiscard]] constexpr inline T remap_clamp(T value, T from_lo, T from_hi, T to_lo, T to_hi) noexcept {
	auto result = remap(value, from_lo, from_hi, to_lo, to_hi);

	if (to_lo < to_hi)
		return std::clamp(result, to_lo, to_hi);
	else
		return std::clamp(result, to_hi, to_lo);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
