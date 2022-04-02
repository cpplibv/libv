// Project: libv.math, File: src/libv/math/smoothstep.hpp

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

/// clamped hermite interpolation between edge0 and edge1
template <typename T>
[[nodiscard]] constexpr inline T step(T value, T edge) noexcept {
	return value < edge ? T{0} : T{1};
}

template <typename T>
[[nodiscard]] constexpr inline T linstep(T value, T edge0, T edge1) noexcept {
	const auto x = (value - edge0) / (edge1 - edge0);
	const auto cx = x < T{0} ? T{0} : (x > T{1} ? T{1} : x); // Clamp, <algorithm> is too heavy for only this

	return cx;
}

template <typename T>
[[nodiscard]] constexpr inline T smoothstep(T value, T edge0, T edge1) noexcept {
	const auto x = (value - edge0) / (edge1 - edge0);
	const auto cx = x < T{0} ? T{0} : (x > T{1} ? T{1} : x); // Clamp, <algorithm> is too heavy for only this

	return cx * cx * (T{3} - T{2} * cx);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
