// Project: libv.ui, File: src/libv/ui/context/context_layout.hpp, Author: Császár Mátyás [Vader]

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
	libv::vec3f float_position;

	libv::vec3f position;
	libv::vec3f size;

	int depth = 0;

public:
	constexpr inline ContextLayout2(libv::vec3f position, libv::vec3f size)  noexcept :
		position(position),
		size(size) { }

private:
	constexpr inline ContextLayout2(libv::vec3f abs_position, libv::vec3f position, libv::vec3f size, int depth) noexcept :
		float_position(abs_position),
		position(position),
		size(size),
		depth(depth) { }

public:
	[[nodiscard]] constexpr inline ContextLayout2 enter(libv::vec3f position_, libv::vec3f size_) const noexcept {
		return ContextLayout2{
				float_position + position_,
				position_,
				size_,
				depth + 1
		};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
