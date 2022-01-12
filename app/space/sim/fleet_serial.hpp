// Project: libv, File: app/space/sim/fleet_serial.hpp

#pragma once

// hpp
#include <space/sim/fleet.hpp>
// libv
#include <libv/math/vec_serial.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_string.hpp>
// pro
#include <space/sim/engine/serial_id.hpp>
#include <space/sim/faction.hpp> // Only for SerialID id
#include <space/sim/planet.hpp> // Only for SerialID id


namespace space {

// -------------------------------------------------------------------------------------------------

template <typename Archive> void Fleet::Command::serialize(Archive& ar) {
	ar & LIBV_NVP(type);
	ar & LIBV_NVP(targetPosition);

	ar & LIBV_NVP_NAMED("targetPlanet", SerialID{targetPlanet});
	ar & LIBV_NVP_NAMED("targetFleet", SerialID{targetFleet});
}

template <typename Archive> void Fleet::serialize(Archive& ar) {
	ar & LIBV_NVP(id);
	ar & LIBV_NVP(position);
	ar & LIBV_NVP(name);
//	ar & LIBV_NVP(orientation);
	ar & LIBV_NVP(commands);

	if (ar.isLocal())
		ar & LIBV_NVP(selectionStatus);

	ar & LIBV_NVP_NAMED("faction", SerialID{faction});

	ar & LIBV_NVP(number_of_ships);
	ar & LIBV_NVP(distance_travelled);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
