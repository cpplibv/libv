// Project: libv, File: app/space/universe/universe.hpp

#pragma once

// hpp
#include <space/universe/planet.hpp>
// libv
#include <libv/math/vec_serial.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_string.hpp>
// pro
#include <space/universe/engine/serial_id.hpp>
#include <space/universe/faction.hpp> // Only for SerialID id


namespace space {

// -------------------------------------------------------------------------------------------------

template <typename Archive> void Planet::serialize(Archive& ar) {
	ar & LIBV_NVP(id);
	ar & LIBV_NVP(position);
	ar & LIBV_NVP(name);
	ar & LIBV_NVP(radius);
	ar & LIBV_NVP(color0);
	ar & LIBV_NVP(color1);
	ar & LIBV_NVP_NAMED("faction", SerialID{faction});
}

// -------------------------------------------------------------------------------------------------

} // namespace space
