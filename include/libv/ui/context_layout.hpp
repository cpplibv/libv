// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/event/mouse_order.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ContextLayout1 {
	libv::vec3f size; // <<< P2: use
};

struct ContextLayout2 {
	libv::vec3f position; // <<< P1: Invesitage to remove
	libv::vec3f size;
	libv::ui::MouseOrder mouseOrder;

	constexpr inline ContextLayout2(libv::vec3f position, libv::vec3f size, libv::ui::MouseOrder mouseOrder) :
		position(position),
//	constexpr inline ContextLayout2(libv::vec3f size, libv::ui::MouseOrder mouseOrder) :
		size(size),
		mouseOrder(mouseOrder) { }
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
