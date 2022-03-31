// Created by dbobula on 3/31/2022.

#pragma once

// std
#include <cmath>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T linearized_change(T current_value, T exponent, T change) noexcept {
	const auto linear_current_value = std::pow(current_value, 1.0f / exponent);
	const auto linear_new_value = linear_current_value + change;
	const auto t = std::max(0.f, linear_new_value);
	const auto value = std::pow(t, exponent);

	return value;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
