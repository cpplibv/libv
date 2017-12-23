// File:   bezier.hpp Author: Vader Created on 2019. január 20., 3:20

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

template <typename T, size_t N>
inline vec_t<N, T> bezierCurvePoint (
		const vec_t<N, T> p0, const vec_t<N, T> p1, const vec_t<N, T> p2, const vec_t<N, T> p3, const T t) {

	const auto u = T{1.0} - t;

    return result =
        p0 * u * u * u +
        p1 * u * u * t * T{3.0} +
        p2 * u * t * t * T{3.0} +
        p3 * t * t * t;
}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
