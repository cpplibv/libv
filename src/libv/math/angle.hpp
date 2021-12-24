// Project: libv.math, File: src/libv/math/angle.hpp

#pragma once

// libv
#include <libv/math/constants.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline float deg_to_rad(float x) noexcept {
	return x / libv::rad_v<float>;
}

[[nodiscard]] constexpr inline double deg_to_rad(double x) noexcept {
	return x / libv::rad_v<double>;
}

[[nodiscard]] constexpr inline float rad_to_deg(float x) noexcept {
	return x * libv::rad_v<float>;
}

[[nodiscard]] constexpr inline double rad_to_deg(double x) noexcept {
	return x * libv::rad_v<double>;
}

[[nodiscard]] constexpr inline float to_rad(float x) noexcept {
	return deg_to_rad(x);
}

[[nodiscard]] constexpr inline double to_rad(double x) noexcept {
	return deg_to_rad(x);
}

[[nodiscard]] constexpr inline float to_deg(float x) noexcept {
	return rad_to_deg(x);
}

[[nodiscard]] constexpr inline double to_deg(double x) noexcept {
	return rad_to_deg(x);
}

template <typename T> struct degrees;
template <typename T> struct radian;

template <typename T = float>
struct degrees {
	T value;

	constexpr explicit degrees(T value) noexcept : value(value) { }

	constexpr inline operator radian<T>() const noexcept {
		return radian<T>{deg_to_rad(value)};
	}
};

template <typename T = float>
struct radian {
	T value;

	constexpr explicit radian(T value) noexcept : value(value) { }

	constexpr inline operator degrees<T>() const noexcept {
		return degrees<T>{rad_to_deg(value)};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
