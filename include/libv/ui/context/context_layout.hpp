// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ContextLayout1 {
	libv::vec3f size;
};

struct ContextLayout2 {
public:
	libv::vec3f abs_position;

	libv::vec3f position;
	libv::vec3f size;

	int depth = 0;

public:
	constexpr inline ContextLayout2(libv::vec3f position, libv::vec3f size)  noexcept :
		position(position),
		size(size) { }

	constexpr inline ContextLayout2(libv::vec3f abs_position, libv::vec3f position, libv::vec3f size, int depth) noexcept :
		abs_position(abs_position),
		position(position),
		size(size),
		depth(depth) { }

public:
	[[nodiscard]] constexpr inline ContextLayout2 enter(libv::vec3f position, libv::vec3f size) const noexcept {
		return ContextLayout2{
				abs_position + position,
				position,
				size,
				depth + 1
		};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
