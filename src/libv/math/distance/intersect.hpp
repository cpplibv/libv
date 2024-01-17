// Project: libv.math, File: src/libv/math/distance/intersect.hpp

#pragma once

#include <libv/math/plane.hpp>
#include <libv/math/ray.hpp>
#include <libv/math/vec.hpp>

#include <optional>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline libv::vec3f intersect_ray_plane(libv::vec3f ray_point, libv::vec3f ray_dir, libv::vec3f plane_point, libv::vec3f plane_normal) noexcept {
	const auto diff = ray_point - plane_point;
	const auto prod1 = libv::vec::dot(diff, plane_normal);
	const auto prod2 = libv::vec::dot(ray_dir, plane_normal);
	const auto prod3 = prod1 / prod2;
	return ray_point - ray_dir * prod3;
}

[[nodiscard]] constexpr inline std::optional<libv::vec3f> intersect_ray_plane(libv::ray3f ray, libv::planef plane) noexcept {
	assert(ray.direction.is_normalized());
	assert(plane.normal.is_normalized());

	const auto denom = libv::dot(ray.direction, plane.normal);
	if (std::abs(denom) < 1e-6f)
		// Denom zero means ray and plane are parallel to each other
		return std::nullopt;

	const auto t = -(libv::dot(ray.position, plane.normal) + plane.dist) / denom;
	if (t < 0)
		// Negative t means the plane is behind the ray's starting position
		return std::nullopt;

	return ray.position + t * ray.direction;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
