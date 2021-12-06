// Project: libv.math, File: src/libv/math/bezier_curve.hpp

#pragma once

// std
#include <algorithm>
#include <cmath>
#include <vector>
// pro
#include <libv/math/bezier.hpp>
#include <libv/math/vec.hpp>


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

struct BezierCurve {
	std::vector<libv::vec2f> points;

public:
	[[nodiscard]] inline std::size_t size() const noexcept {
		return (points.size() + 2) / 3;
	}

	[[nodiscard]] constexpr inline float min() const noexcept {
		return 0.f;
	}

	[[nodiscard]] inline float max() const noexcept {
		return static_cast<float>((points.size() - 1) / 3);
	}

	libv::vec2f eval(float t) {
		if (t < min())
			return points[0];

		if (t > max())
			return points.back();

		const float base_t = std::floor(t);
		const std::size_t i = static_cast<std::size_t>(t) * 3;
		const std::size_t p0 = i + 0;
		const std::size_t p1 = i + 1;
		const std::size_t p2 = i + 2;
		const std::size_t p3 = i + 3;

		return bezierCurvePoint(points[p0], points[p1], points[p2], points[p3], t - base_t);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
