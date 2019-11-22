// File:   snap.hpp Author: Vader Created on 22 November 2019, 07:01

#pragma once

// std
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
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wfloat-equal"
	if (snap_ == 0.0)
		return value;
	#pragma GCC diagnostic pop

	return std::round(value / snap_) * snap_;
}

template <typename T>
[[nodiscard]] constexpr inline T snap_interval(T value, T snap_, T lo, T hi) noexcept {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wfloat-equal"
	if (snap_ == 0.0)
		return clamp_interval(value, lo, hi);
	#pragma GCC diagnostic pop

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

//interval_snap(i, 2, 1, 5.5)
//interval_snap(i, 2, 0, 6)
