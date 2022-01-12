// Project: libv, File: src/space/universe/galaxy.hpp

#pragma once

// hpp
#include <space/universe/galaxy.hpp>
// libv
#include <libv/serial/serial.hpp>
#include <libv/serial/types/boost_flat_set.hpp>
#include <libv/serial/types/std_memory.hpp>
#include <libv/serial/types/std_vector.hpp>
#include <libv/utility/entity/entity_ptr_serial.hpp>
// pro
#include <space/universe/engine/memory_store.hpp>
#include <space/universe/faction_serial.hpp>
#include <space/universe/fleet_serial.hpp>
#include <space/universe/planet_serial.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

template <typename Archive> void Galaxy::serialize(Archive& ar) {
	ar & LIBV_NVP(nextFactionID);
	ar & LIBV_NVP(nextPlanetID);
	ar & LIBV_NVP(nextFleetID);

	ar & LIBV_NVP_NAMED("factions", libv::entity_primary_owner(memory->faction, factions));
	ar & LIBV_NVP_NAMED("planets", libv::entity_primary_owner(memory->planet, planets));
	ar & LIBV_NVP_NAMED("fleets", libv::entity_primary_owner(memory->fleet, fleets));

	if (ar.isLocal())
		ar & LIBV_NVP(selectedFleetIDList);
	else
		if constexpr (Archive::is_loading::value)
			// TODO P5: Instead of full clear, remove/update only the elements that are no longer available (for seamless snapshot update)
			selectedFleetIDList.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace space
