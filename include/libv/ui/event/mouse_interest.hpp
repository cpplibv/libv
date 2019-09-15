// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// libv
#include <libv/utility/flag_enum.hpp>
// std
#include <cstdint>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using MouseInterest_t = libv::flag_enum<uint16_t, struct MouseInterest_tag>;
namespace MouseInterest {

static constexpr MouseInterest_t none = MouseInterest_t{};

static constexpr MouseInterest_t press          = libv::bit(0);
static constexpr MouseInterest_t release        = libv::bit(1);
static constexpr MouseInterest_t movement       = libv::bit(2);
static constexpr MouseInterest_t enter          = libv::bit(3);
static constexpr MouseInterest_t leave          = libv::bit(4);
static constexpr MouseInterest_t scroll         = libv::bit(5);
//static constexpr MouseInterest_t ______       = libv::bit(6);
//static constexpr MouseInterest_t ______       = libv::bit(7);

static constexpr MouseInterest_t singleClick    = libv::bit(8);
static constexpr MouseInterest_t doubleClick    = libv::bit(9);
static constexpr MouseInterest_t globalMovement = libv::bit(10);
//static constexpr MouseInterest_t ______       = libv::bit(11);
//static constexpr MouseInterest_t ______       = libv::bit(12);
//static constexpr MouseInterest_t ______       = libv::bit(13);
//static constexpr MouseInterest_t ______       = libv::bit(14);
//static constexpr MouseInterest_t ______       = libv::bit(15);

static constexpr MouseInterest_t mask_button =
		press | release;

static constexpr MouseInterest_t mask_position =
		movement | enter | leave;

static constexpr MouseInterest_t mask_scroll =
		scroll;

static constexpr MouseInterest_t mask_mouse =
		press | release | movement | enter | leave | scroll |
		singleClick | doubleClick | globalMovement;

} // namespace interest

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
