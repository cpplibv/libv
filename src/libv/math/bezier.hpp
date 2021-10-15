// Project: libv.math, File: src/libv/math/bezier.hpp

#pragma once


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

template <typename T, typename S = T>
[[nodiscard]] constexpr inline T bezierCurvePoint(const T p0, const T p1, const T p2, const T p3, const S t) noexcept {
	const auto _1 = S{1.0};
	const auto _3 = S{3.0};
	const auto u = _1 - t;
	const auto u2 = u * u;
	const auto u3 = u2 * u;
	const auto t2 = t * t;
	const auto t3 = t2 * t;

    return
	        p0 * u3 +
	        p1 * u2 * t * _3 +
	        p2 * u * t2 * _3 +
	        p3 * t3;
}

template <typename T, typename S = T>
[[nodiscard]] constexpr inline T bezierCurveTangent(const T p0, const T p1, const T p2, const T p3, const S t) noexcept {
	const auto _1 = S{1.0};
	const auto _2 = S{2.0};
	const auto _3 = S{3.0};
	const auto u = _1 - t;
	const auto u2 = u * u;
	const auto t2 = t * t;

	auto tangent =
			p0 * (-u2) +
			p1 * (_3 * u2 - _2 * u) +
			p2 * (-_3 * t2 + _2 * t) +
			p3 * (t2);

    return normalize(tangent);
}

template <typename T, typename S = T>
[[nodiscard]] constexpr inline T bezierCurveNormal(const T p0, const T p1, const T p2, const T p3, const S t, const T up) noexcept {
	const auto tangent = bezierCurveTangent(p0, p1, p2, p3, t);
	const auto binormal = normalize(cross(tangent, up));
	const auto normal = cross(binormal, tangent);
//	const auto binormal = normalize(cross(up, tangent));
//	const auto normal = cross(tangent, binormal);

    return normal;
}

//template <typename T, typename S = T>
//[[nodiscard]] constexpr inline quat bezierCurveOrientation(const T p0, const T p1, const T p2, const T p3, const S t, const T up) noexcept {
//	const auto tangent = bezierCurveTangent(p0, p1, p2, p3, t);
//	const auto normal = bezierCurveNormal(p0, p1, p2, p3, t, up);
//
//    return quat::look_rotation(tangent, normal);
//}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
