// File:   bezier.hpp Author: Vader Created on 2019. január 20., 3:20

#pragma once


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

template <typename T>
inline T bezierCurvePoint(const T p0, const T p1, const T p2, const T p3, const T t) {
	const auto u = T{1.0} - t;

    return
        p0 * u * u * u +
        p1 * u * u * t * T{3.0} +
        p2 * u * t * t * T{3.0} +
        p3 * t * t * t;
}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
