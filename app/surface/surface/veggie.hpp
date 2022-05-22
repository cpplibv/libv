// Created by dbobula on 1/16/2022.

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

using VeggieId = int32_t;

struct Veggie {
	VeggieId id;

	libv::vec3f pos;
	libv::vec3f normal; // = {0, 0, 1};
	float rotation; // Normal + rotation = quaternion?
	float scale;
	// clamp during randomization, except hue <- modf
	libv::vec3f hsv_shift;
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
