// Project: libv.math, File: src/libv/math/distance/distance.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/solve_quadratic.hpp>
// std
#include <cmath>
#include <chrono>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline auto distanceTestLineToSphere(libv::vec3f linePos, libv::vec3f lineDir, libv::vec3f spherePos, float sphereRadius) noexcept {
	struct Result {
		bool hit;
		float t;
	};

	auto dir = lineDir;

	float t0, t1;

	auto l = linePos - spherePos;
	auto a = dir.length_sq();
	auto b = 2 * libv::vec::dot(dir, l);
	auto c = l.length_sq() - sphereRadius * sphereRadius;

	if (!solve_quadratic(a, b, c, t0, t1))
		return Result(false, 0.f);

	if (t0 > t1)
		std::swap(t0, t1);

	if (t0 < 0.f) {
		t0 = t1;
		if (t0 < 0.f)
			return Result(false, 0.f);
	}

	return Result(true, t0);
}

/// http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
[[nodiscard]] constexpr inline auto distanceLineToPoint(libv::vec3f linePos, libv::vec3f lineDir, libv::vec3f point) noexcept {
	auto w = point - linePos;

	auto c1 = libv::vec::dot(w, lineDir);
	auto c2 = lineDir.length_sq();
	auto b = c1 / c2;

	auto pb = linePos + b * lineDir;
	return (point - pb).length();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
