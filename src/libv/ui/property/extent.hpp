// Project: libv.ui, File: src/libv/ui/property/extent.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/meta/force_inline.hpp>
#include <libv/meta/uninitialized.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct Extent {
	T left;
	T bottom;
	T right;
	T top;

public:
	constexpr LIBV_FORCE_INLINE Extent() noexcept :
			left(0),
			bottom(0),
			right(0),
			top(0) {}

	explicit constexpr LIBV_FORCE_INLINE Extent(libv::uninitialized_t) noexcept {
		// uninitialized members
	};

	explicit constexpr LIBV_FORCE_INLINE Extent(T value) noexcept :
			left(value),
			bottom(value),
			right(value),
			top(value) {}

	constexpr LIBV_FORCE_INLINE Extent(T left, T bottom, T right, T top) noexcept :
			left(left),
			bottom(bottom),
			right(right),
			top(top) {}

	constexpr LIBV_FORCE_INLINE Extent(libv::vec2_t<T> left_bottom, libv::vec2_t<T> right_top) noexcept :
			left(left_bottom.x),
			bottom(left_bottom.y),
			right(right_top.x),
			top(right_top.y) {}

	explicit constexpr LIBV_FORCE_INLINE Extent(libv::vec4_t<T> left_bottom_right_top) noexcept :
			left(left_bottom_right_top.x),
			bottom(left_bottom_right_top.y),
			right(left_bottom_right_top.z),
			top(left_bottom_right_top.w) {}

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2_t<T> left_bottom() const noexcept {
		return {left, bottom};
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2_t<T> LB() const noexcept {
		return left_bottom();
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2_t<T> right_top() const noexcept {
		return {right, top};
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2_t<T> RT() const noexcept {
		return right_top();
	}

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec2_t<T> size() const noexcept {
		return left_bottom() + right_top();
	}

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE libv::vec4_t<T> as_vec4() const noexcept {
		return {left, bottom, right, top};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
