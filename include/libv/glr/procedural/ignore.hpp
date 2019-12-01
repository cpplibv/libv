// File: sphere.hpp Author: Vader Created on January 28, 2017, 3:22 PM

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct Irnoge {
	template <typename... Args>
	constexpr inline void operator()(Args&&...) noexcept {}

	template <typename T>
	constexpr inline void reserve(T) noexcept {}
} ignore;

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
