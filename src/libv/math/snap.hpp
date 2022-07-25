// Project: libv.math, File: src/libv/math/snap.hpp

#pragma once

// libv
#include <libv/utility/float_equal.hpp>
// std
#include <algorithm> // For std::clamp
#include <cmath>
#include <utility>


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline bool check_interval(T value, T lo, T hi) noexcept {
	return !((value > lo && value > hi) || (value < lo && value < hi));
}

template <typename T>
[[nodiscard]] constexpr inline T clamp_interval(T value, T lo, T hi) noexcept {
	if (hi < lo)
		return std::clamp(value, hi, lo);
	else
		return std::clamp(value, lo, hi);
}

template <typename T>
[[nodiscard]] constexpr inline T snap(T value, T snap_) noexcept {
	if (libv::float_equal(snap_, 0.0))
		return value;

	return std::round(value / snap_) * snap_;
}

template <typename T>
[[nodiscard]] constexpr inline T snap_interval(T value, T snap_, T lo, T hi) noexcept {
	if (libv::float_equal(snap_, 0.0))
		return clamp_interval(value, lo, hi);

	if (hi < lo)
		std::swap(hi, lo);

	if (value - lo < std::ceil(lo / snap_) * snap_ - value)
		return lo;

	if (value - hi >= std::floor(hi / snap_) * snap_ - value)
		return hi;

	return std::round(value / snap_) * snap_;
}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
