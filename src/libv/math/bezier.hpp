// Project: libv.math, File: src/libv/math/bezier.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

template <typename T, typename S = T>
[[nodiscard]] constexpr inline T bezierCurvePoint(const T p0, const T p1, const T p2, const T p3, const S t) noexcept {
	const auto u = S{1.0} - t;

    return
        p0 * u * u * u +
        p1 * u * u * t * S{3.0} +
        p2 * u * t * t * S{3.0} +
        p3 * t * t * t;
}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
