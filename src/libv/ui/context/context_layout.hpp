// Project: libv.ui, File: src/libv/ui/context/context_layout.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ContextLayout1 {
	libv::vec3f size;

	explicit ContextLayout1(libv::vec2f size) : size(size, 0) {}
	explicit ContextLayout1(libv::vec3f size) : size(size) {}
};

struct ContextLayout2 {
public:
	libv::vec3f float_position;
	mutable bool float_position_changed = false; // Mutable as CoreComponent::layout2 sets it after it 'enters' a child (there are better ways for this)

	libv::vec3f position;
	libv::vec3f size;

	int depth = 0;

public:
	constexpr inline ContextLayout2(libv::vec3f position, libv::vec3f size)  noexcept :
		position(position),
		size(size) { }

private:
	constexpr inline ContextLayout2(libv::vec3f abs_position, bool float_position_changed, libv::vec3f position, libv::vec3f size, int depth) noexcept :
		float_position(abs_position),
		float_position_changed(float_position_changed),
		position(position),
		size(size),
		depth(depth) { }

public:
	[[nodiscard]] constexpr inline ContextLayout2 enter(libv::vec3f position_, libv::vec3f size_) const noexcept {
		return ContextLayout2{
				float_position + position_,
				float_position_changed,
				position_,
				size_,
				depth + 1
		};
	}

	[[nodiscard]] constexpr inline ContextLayout2 enter(libv::vec2f position_, libv::vec2f size_) const noexcept {
		return ContextLayout2{
				float_position + libv::vec3f(position_, 0.f),
				float_position_changed,
				libv::vec3f(position_, 0.f),
				libv::vec3f(size_, 0.f),
				depth + 1
		};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
