// Project: libv.ui, File: src/libv/ui/context/context_focus_traverse.hpp

#pragma once

#include <libv/math/vec.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextFocusTraverse {
public:
	bool forward = true;
	bool backward = false;
	libv::vec2f direction;

public:
	[[nodiscard]] static ContextFocusTraverse makeForward() {
		return {true, false, {1.f, 0.f}};
	}
	[[nodiscard]] static ContextFocusTraverse makeBackward() {
		return {false, true, {-1.f, 0.f}};
	}
	[[nodiscard]] static ContextFocusTraverse makeDirection(libv::vec2f dir) {
		const auto forwardLike = dir.x - dir.y > 0.f;
		return {forwardLike, !forwardLike, dir};
	}

public:
	[[nodiscard]] constexpr inline bool isForward() const noexcept {
		return forward;
	}

	[[nodiscard]] constexpr inline bool isBackward() const noexcept {
		return backward;
	}

// private:
// 	mutable bool focus_rejected_ = false; // Only relevant on focus gain
//
// public:
// 	explicit constexpr inline ContextEventFocus(bool focus) noexcept : EventFocus(focus) { }
//
// 	constexpr inline void reject_focus() const noexcept {
// 		focus_rejected_ = true;
// 	}
//
// 	[[nodiscard]] constexpr inline bool focus_rejected() const noexcept {
// 		return focus_rejected_;
// 	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
