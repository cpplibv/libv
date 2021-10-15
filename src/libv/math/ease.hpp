// Project: libv.math, File: src/libv/math/ease.hpp

#pragma once

// ext
#include <glm/gtx/easing.hpp> // NOTE: This is kind a heavy include: By duplicating the implementations here it could be removed


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

//enum class ease_function {
//	linear,
//	ease_in_quadratic,
//	ease_out_quadratic,
//	ease_in_out_quadratic,
//	ease_in_cubic,
//	ease_out_cubic,
//	ease_in_out_cubic,
//	ease_in_quartic,
//	ease_out_quartic,
//	ease_in_out_quartic,
//	ease_in_quintic,
//	ease_out_quintic,
//	ease_in_out_quintic,
//	ease_in_sine,
//	ease_out_sine,
//	ease_in_out_sine,
//	ease_in_circular,
//	ease_out_circular,
//	ease_in_out_circular,
//	ease_in_exponential,
//	ease_out_exponential,
//	ease_in_out_exponential,
//	ease_in_elastic,
//	ease_out_elastic,
//	ease_in_out_elastic,
//	ease_in_back,
//	ease_out_back,
//	ease_in_out_back,
//	ease_in_bounce,
//	ease_out_bounce,
//	ease_in_out_bounce,
//};

// -------------------------------------------------------------------------------------------------

/// Slower but custom exponent variant ease in function
/// \param x parameter in [0, 1]
/// \param e used exponent
template <typename T>
[[nodiscard]] constexpr inline T ease_in_custom(const T x, const T e) noexcept {
	return std::pow(x, e);
}

/// Slower but custom exponent variant ease out function
/// \param x parameter in [0, 1]
/// \param e used exponent
template <typename T>
[[nodiscard]] constexpr inline T ease_out_custom(const T x, const T e) noexcept {
	return T{1} - std::pow(T{1} - x, e);
}

/// Slower but custom exponent variant ease in out function
/// \param x parameter in [0, 1]
/// \param e used exponent
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_custom(const T x, const T e) noexcept {
	if (x < T{0.5})
		return std::pow(T{2} * x, e) * T{0.5};
	else
		return (T{2} - std::pow(T{2} - T{2} * x, e)) * T{0.5};
}

// -------------------------------------------------------------------------------------------------

/// Modelled after the line y = x
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T linear_interpolation(const T x) noexcept {
	return glm::linearInterpolation(x);
}

/// Modelled after the parabola y = x^2
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_quadratic(const T x) noexcept {
	return glm::quadraticEaseIn(x);
}

/// Modelled after the parabola y = -x^2 + 2x
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_quadratic(const T x) noexcept {
	return glm::quadraticEaseOut(x);
}

/// Modelled after the piecewise quadratic
/// y = (1/2)((2x)^2)				; [0, 0.5)
/// y = -(1/2)((2x-1)*(2x-3) - 1)	; [0.5, 1]
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_quadratic(const T x) noexcept {
	return glm::quadraticEaseInOut(x);
}

/// Modelled after the cubic y = x^3
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_cubic(const T x) noexcept {
	return glm::cubicEaseIn(x);
}

/// Modelled after the cubic y = (x - 1)^3 + 1
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_cubic(const T x) noexcept {
	return glm::cubicEaseOut(x);
}

/// Modelled after the piecewise cubic
/// y = (1/2)((2x)^3)		; [0, 0.5)
/// y = (1/2)((2x-2)^3 + 2)	; [0.5, 1]
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_cubic(const T x) noexcept {
	return glm::cubicEaseInOut(x);
}

/// Modelled after the quartic x^4
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_quartic(const T x) noexcept {
	return glm::quarticEaseIn(x);
}

/// Modelled after the quartic y = 1 - (x - 1)^4
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_quartic(const T x) noexcept {
	return glm::quarticEaseOut(x);
}

/// Modelled after the piecewise quartic
/// y = (1/2)((2x)^4)			; [0, 0.5)
/// y = -(1/2)((2x-2)^4 - 2)	; [0.5, 1]
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_quartic(const T x) noexcept {
	return glm::quarticEaseInOut(x);
}

/// Modelled after the quintic y = x^5
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_quintic(const T x) noexcept {
	return glm::quinticEaseIn(x);
}

/// Modelled after the quintic y = (x - 1)^5 + 1
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_quintic(const T x) noexcept {
	return glm::quinticEaseOut(x);
}

/// Modelled after the piecewise quintic
/// y = (1/2)((2x)^5)		; [0, 0.5)
/// y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_quintic(const T x) noexcept {
	return glm::quinticEaseInOut(x);
}

/// Modelled after quarter-cycle of sine wave
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_sine(const T x) noexcept {
	return glm::sineEaseIn(x);
}

/// Modelled after quarter-cycle of sine wave (different phase)
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_sine(const T x) noexcept {
	return glm::sineEaseOut(x);
}

/// Modelled after half sine wave
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_sine(const T x) noexcept {
	return glm::sineEaseInOut(x);
}

/// Modelled after shifted quadrant IV of unit circle
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_circular(const T x) noexcept {
	return glm::circularEaseIn(x);
}

/// Modelled after shifted quadrant II of unit circle
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_circular(const T x) noexcept {
	return glm::circularEaseOut(x);
}

/// Modelled after the piecewise circular function
/// y = (1/2)(1 - sqrt(1 - 4x^2))			; [0, 0.5)
/// y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_circular(const T x) noexcept {
	return glm::circularEaseInOut(x);
}

/// Modelled after the exponential function y = 2^(10(x - 1))
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_exponential(const T x) noexcept {
	return glm::exponentialEaseIn(x);
}

/// Modelled after the exponential function y = -2^(-10x) + 1
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_exponential(const T x) noexcept {
	return glm::exponentialEaseOut(x);
}

/// Modelled after the piecewise exponential
/// y = (1/2)2^(10(2x - 1))			; [0,0.5)
/// y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_exponential(const T x) noexcept {
	return glm::exponentialEaseInOut(x);
}

/// Modelled after the damped sine wave y = sin(13pi/2*x)*pow(2, 10 * (x - 1))
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_elastic(const T x) noexcept {
	return glm::elasticEaseIn(x);
}

/// Modelled after the damped sine wave y = sin(-13pi/2*(x + 1))*pow(2, -10x) + 1
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_elastic(const T x) noexcept {
	return glm::elasticEaseOut(x);
}

/// Modelled after the piecewise exponentially-damped sine wave:
/// y = (1/2)*sin(13pi/2*(2*x))*pow(2, 10 * ((2*x) - 1))		; [0,0.5)
/// y = (1/2)*(sin(-13pi/2*((2x-1)+1))*pow(2,-10(2*x-1)) + 2)	; [0.5, 1]
/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_elastic(const T x) noexcept {
	return glm::elasticEaseInOut(x);
}

/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_back(const T x) noexcept {
	return glm::backEaseIn(x);
}

/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_back(const T x) noexcept {
	return glm::backEaseOut(x);
}

/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_back(const T x) noexcept {
	return glm::backEaseInOut(x);
}

/// \param x parameter in [0, 1]
/// \param o Optional overshoot modifier
template <typename T>
[[nodiscard]] constexpr inline T ease_in_back(const T x, const T o) noexcept {
	return glm::backEaseIn(x, o);
}

/// \param x parameter in [0, 1]
/// \param o Optional overshoot modifier
template <typename T>
[[nodiscard]] constexpr inline T ease_out_back(const T x, const T o) noexcept {
	return glm::backEaseOut(x, o);
}

/// \param x parameter in [0, 1]
/// \param o Optional overshoot modifier
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_back(const T x, const T o) noexcept {
	return glm::backEaseInOut(x, o);
}

/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_bounce(const T x) noexcept {
	return glm::bounceEaseIn(x);
}

/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_out_bounce(const T x) noexcept {
	return glm::bounceEaseOut(x);
}

/// \param x parameter in [0, 1]
template <typename T>
[[nodiscard]] constexpr inline T ease_in_out_bounce(const T x) noexcept {
	return glm::bounceEaseInOut(x);
}


// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
