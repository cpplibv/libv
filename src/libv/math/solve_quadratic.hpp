// Project: libv.math, File: src/libv/math/solve_quadratic.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/float_equal.hpp>
// std
#include <cmath>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline bool solve_quadratic(float a, float b, float c, float& x0, float& x1) noexcept {
	float discr = b * b - 4 * a * c;

	if (discr < 0) {
		return false;

	} else if (libv::float_equal(discr, 0.0f)) {
		x0 = x1 = -0.5f * b / a;

	} else {
		float q = -0.5f * (b + std::copysign(std::sqrt(discr), b));
		x0 = q / a;
		x1 = c / q;
	}

	if (x0 > x1)
		std::swap(x0, x1);

	return true;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
