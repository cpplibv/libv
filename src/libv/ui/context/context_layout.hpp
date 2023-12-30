// Project: libv.ui, File: src/libv/ui/context/context_layout.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextLayout1 {
public:
	/// Indicator if the given direction is unlimited or not.
	/// Unbounded dimensions can ignore limit and occupy as much space as they want.
	libv::vec2b unlimited;
	/// Max size that a node can acquire (negative values represents oversized children).
	/// Includes the space that was already assigned to the component.
	libv::vec2f limit;
	/// Parent size (never infinite or negative). Falls back to viewport size if parent is not constrained.
	libv::vec2f parent;

	ContextLayout1(libv::vec2b unlimited, libv::vec2f limit, libv::vec2f parent) :
			unlimited(unlimited),
			limit(limit),
	        parent(parent) {
	}

	[[nodiscard]] static ContextLayout1 fix(libv::vec2f limit) {
		return {libv::vec2b{false, false}, limit, limit};
	}

	[[nodiscard]] ContextLayout1 reduce(libv::vec2f reduceLimit, libv::vec2f reduceParent) const {
		return {unlimited, limit - reduceLimit, parent - reduceParent};
	}

	[[nodiscard]] libv::vec2f minusOneIfUnlimited(libv::vec2f limit_) const {
		return {
			unlimited.x ? -1.f : std::max(limit_.x, 0.f),
			unlimited.y ? -1.f : std::max(limit_.y, 0.f),
		};
	}
};

class ContextLayout2 {
public:
	libv::vec2f float_position;
	mutable bool float_position_changed = false; // Mutable as CoreComponent::layout2 sets it after it 'enters' a child (there are better ways for this)

	libv::vec2f position;
	libv::vec2f size;

	int depth = 0;

public:
	constexpr inline ContextLayout2(libv::vec2f position, libv::vec2f size)  noexcept :
		position(position),
		size(size) { }

private:
	constexpr inline ContextLayout2(libv::vec2f abs_position, bool float_position_changed, libv::vec2f position, libv::vec2f size, int depth) noexcept :
		float_position(abs_position),
		float_position_changed(float_position_changed),
		position(position),
		size(size),
		depth(depth) { }

public:
	[[nodiscard]] constexpr inline ContextLayout2 enter(libv::vec2f position_, libv::vec2f size_) const noexcept {
		return ContextLayout2{
				float_position + position_,
				float_position_changed,
				position_,
				size_,
				depth + 1
		};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
