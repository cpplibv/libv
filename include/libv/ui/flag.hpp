// File:   flag.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// libv
#include <libv/utility/flag_enum.hpp>
// std
#include <cstdint>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using Flag_t = libv::flag_enum<uint32_t, struct Flag_tag>;
namespace Flag {

static constexpr Flag_t invalidAttachStrong = libv::bit(0);
static constexpr Flag_t invalidAttachWeak = libv::bit(1);
static constexpr Flag_t invalidAttach = invalidAttachStrong | invalidAttachWeak;
static constexpr Flag_t invalidLayout = libv::bit(2);
static constexpr Flag_t invalidRender = libv::bit(3);

static constexpr Flag_t renderChangeSize = libv::bit(4);
static constexpr Flag_t renderChangePosition = libv::bit(5);

// TODO P5: implement display / enable / layout flags

/// Mark if the component is displayed, not displayed component are still layouted
static constexpr Flag_t render = libv::bit(6);
/// Mark if the component is enabled, disabled component are not notified by events
static constexpr Flag_t enable = libv::bit(7);
/// Mark if the component is layouted, not layouted components are layouted as floating 100% parent size
static constexpr Flag_t layout = libv::bit(8);

static constexpr Flag_t mask_propagateDown =
		enable;
static constexpr Flag_t mask_propagateUp =
		invalidAttachWeak | invalidLayout | invalidRender;
static constexpr Flag_t mask_init =
		mask_propagateDown | mask_propagateUp | renderChangeSize | renderChangePosition | render | layout;

//static constexpr Flag_t invalidLayoutStrong = libv::bit(4); /// Layout has been invalidated
//static constexpr Flag_t invalidLayoutWeak = libv::bit(5); /// Has at least one children with invalidated layout
//static constexpr Flag_t invalidLayout = invalidLayoutStrong | invalidLayoutWeak;

//static constexpr Flag_t hasChildren = libv::bit(7);
//static constexpr Flag_t hasChildrenWithRender = libv::bit(8);
//static constexpr Flag_t hasChildrenWithMouse = libv::bit(9);
//static constexpr Flag_t hasChildrenWithKeyboard = libv::bit(10);
//static constexpr Flag_t hasChildrenWithFocus = libv::bit(11);

} // namespace flag


// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
