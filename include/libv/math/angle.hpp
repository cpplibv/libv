// File: angle.hpp Author: Vader Created on 2017. január 23., 0:07

#pragma once

// libv
#include <libv/math/constants.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline float deg_to_rad(float x) {
	return x / libv::deg_v<float>;
}

[[nodiscard]] constexpr inline double deg_to_rad(double x) {
	return x / libv::deg_v<double>;
}

[[nodiscard]] constexpr inline float rad_to_deg(float x) {
	return x * libv::deg_v<float>;
}

[[nodiscard]] constexpr inline double rad_to_deg(double x) {
	return x * libv::deg_v<double>;
}

[[nodiscard]] constexpr inline float to_rad(float x) {
	return deg_to_rad(x);
}

[[nodiscard]] constexpr inline double to_rad(double x) {
	return deg_to_rad(x);
}

[[nodiscard]] constexpr inline float to_deg(float x) {
	return rad_to_deg(x);
}

[[nodiscard]] constexpr inline double to_deg(double x) {
	return rad_to_deg(x);
}

template <typename T> struct Degrees;
template <typename T> struct Radian;

template <typename T = float>
struct Degrees {
	T value;

	constexpr explicit Degrees(T value) : value(value) { }

	constexpr inline operator Radian<T>() const {
		return Radian<T>{deg_to_rad(value)};
	}
};

template <typename T = float>
struct Radian {
	T value;

	constexpr explicit Radian(T value) : value(value) { }

	constexpr inline operator Degrees<T>() const {
		return Degrees<T>{rad_to_deg(value)};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
