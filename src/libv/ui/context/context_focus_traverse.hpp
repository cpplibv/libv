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
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
