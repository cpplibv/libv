// Project: libv.math, File: src/libv/math/fract.hpp

#pragma once

// std
#include <cmath>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Matches glsl fract
template <typename T>
[[nodiscard]] constexpr inline T fract(T x) noexcept {
	return x - std::floor(x);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
