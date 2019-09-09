// Project: libv.math, File: src/libv/math/angle.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/constants.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline float deg_to_rad(float x) noexcept {
	return x / libv::deg_v<float>;
}

[[nodiscard]] constexpr inline double deg_to_rad(double x) noexcept {
	return x / libv::deg_v<double>;
}

[[nodiscard]] constexpr inline float rad_to_deg(float x) noexcept {
	return x * libv::deg_v<float>;
}

[[nodiscard]] constexpr inline double rad_to_deg(double x) noexcept {
	return x * libv::deg_v<double>;
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

template <typename T> struct Degrees;
template <typename T> struct Radian;

template <typename T = float>
struct Degrees {
	T value;

	constexpr explicit Degrees(T value) noexcept : value(value) { }

	constexpr inline operator Radian<T>() const noexcept {
		return Radian<T>{deg_to_rad(value)};
	}
};

template <typename T = float>
struct Radian {
	T value;

	constexpr explicit Radian(T value) noexcept : value(value) { }

	constexpr inline operator Degrees<T>() const noexcept {
		return Degrees<T>{rad_to_deg(value)};
	}
};

namespace math { // --------------------------------------------------------------------------------

template <typename Rep, typename Res>
struct Angle {
	Rep value;

	explicit constexpr inline Angle(Rep value) noexcept :
		value(value) { }

	constexpr inline Angle(const Angle<Rep, Res>& other) noexcept :
		value(other.value) { }

	template <typename CRep, typename CRes>
	constexpr inline Angle(const Angle<CRep, CRes>& other) noexcept :
		value(other.value / CRes::ratio * Res::ratio) { }

	// Assignment
	// Operators
	// OStream
	// ToDeg
	// ToRad
};

template <typename Rep>
struct ResDegrees { static constexpr Rep ratio = libv::DEG<Rep>; };

template <typename Rep>
struct ResRadian { static constexpr Rep ratio = {1}; };

template <typename Rep>
using Degrees = Angle<Rep, ResDegrees<Rep>>;

template <typename Rep>
using Radian = Angle<Rep, ResRadian<Rep>>;

} // namespace math --------------------------------------------------------------------------------
} // namespace libv
