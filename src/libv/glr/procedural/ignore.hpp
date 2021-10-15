// Project: libv.glr, File: src/libv/glr/procedural/ignore.hpp

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
