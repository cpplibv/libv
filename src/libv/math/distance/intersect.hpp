// Project: libv.math, File: src/libv/math/distance/intersect.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline libv::vec3f intersect_ray_plane(libv::vec3f ray_point, libv::vec3f ray_dir, libv::vec3f plane_point, libv::vec3f plane_normal) noexcept {
	const auto diff = ray_point - plane_point;
	const auto prod1 = libv::vec::dot(diff, plane_normal);
	const auto prod2 = libv::vec::dot(ray_dir, plane_normal);
	const auto prod3 = prod1 / prod2;
	return ray_point - ray_dir * prod3;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
