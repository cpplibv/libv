// File:   fast_rsqrt.hpp Author: Vader Created on 09 September 2019, 10:40

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cmath>


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

/// Determines the signed angle between the two vectors on the plane
template <typename T>
[[nodiscard]] constexpr inline auto signed_angle(libv::vec3_t<T> v0, libv::vec3_t<T> v1, libv::vec3_t<T> plane_normal = libv::vec3_t<T>{0, 0, 1}) noexcept {
	const auto v0n = v0.normalize_copy();
	const auto v1n = v1.normalize_copy();

	const auto theta = std::acos(libv::vec::dot(v0n, v1n));

	const auto v2 = libv::vec::cross(v0n, v1n);

	if (libv::vec::dot(plane_normal, v2) < T{0})
		return -theta;
	else
		return theta;
}

/// Determines the signed angle between the two vectors
template <typename T>
[[nodiscard]] constexpr inline auto signed_angle(libv::vec2_t<T> v0, libv::vec2_t<T> v1) noexcept {
	return signed_angle<T>({v0, T{0}}, {v1, T{0}}, {T{0}, T{0}, T{1}});
}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
