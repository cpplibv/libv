// Project: libv, File: app/space/sim/galaxy_serial.hpp

#pragma once

// hpp
#include <space/sim/galaxy.hpp>
// libv
#include <libv/serial/serial.hpp>
//#include <libv/serial/types/std_vector.hpp>
#include <libv/utility/entity/entity_ptr_serial.hpp>
// pro
//#include <space/sim/engine/memory_store.hpp>
#include <space/sim/fleet_serial.hpp>
#include <space/sim/planet_serial.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

template <typename Archive> void Galaxy::serialize(Archive& ar) {
//	ar(LIBV_NVP_NAMED("planets", libv::entity_primary_owner(ar.simulationContext.memory.planet, planets)));
//	ar(LIBV_NVP_NAMED("fleets", libv::entity_primary_owner(ar.simulationContext.memory.fleet, fleets)));
	ar(LIBV_NVP(planets));
	ar(LIBV_NVP(fleets));
}

// -------------------------------------------------------------------------------------------------

} // namespace space
