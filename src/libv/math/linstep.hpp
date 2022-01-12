// Project: libv.math, File: src/libv/math/linstep.hpp

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T linstep(T value, T edge0, T edge1) noexcept {
	const auto x = (value - edge0) / (edge1 - edge0);
	const auto cx = x < T{0} ? T{0} : (x > T{1} ? T{1} : x); // Clamp, <algorithm> is too heavy for only this

	return cx;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
