// Project: libv, File: app/space/sim/universe_serial.hpp

#pragma once

// hpp
#include <space/sim/universe.hpp>
// libv
#include <libv/serial/serial.hpp>
#include <libv/serial/types/boost_flat_set.hpp>
#include <libv/utility/random/xoroshiro128_serial.hpp>
// pro
//#include <space/sim/engine/snapshot_archive_cc.hpp>
//#include <space/sim/engine/snapshot_ptr_resolver.hpp>
#include <space/sim/galaxy_serial.hpp>
//#include <space/sim/faction.hpp>
//#include <space/sim/engine/memory_store.hpp>
#include <space/sim/faction_serial.hpp>

namespace space {

// -------------------------------------------------------------------------------------------------

//template <typename Archive> void Universe::save(Archive& ar) const {
//	static_assert(ccSnapshotArchive<Archive>);
//
//	ar & LIBV_NVP(universe_rng);
//	ar & LIBV_NVP(galaxy);
//	//if (ar.isLocal())
//	//	ar & LIBV_NVP(controlledFaction);
//}
//
//template <typename Archive> void Universe::load(Archive& ar) {
//	static_assert(ccSnapshotArchive<Archive>);
//
//	ar & LIBV_NVP(universe_rng);
//	ar & LIBV_NVP(galaxy);
//	//if (ar.isLocal())
//	//	ar & LIBV_NVP(controlledFaction);
//
//	SnapshotPtrResolverArchive resolver{*this, ar.isLocal()};
//
//	resolver & LIBV_NVP(universe_rng);
//	resolver & LIBV_NVP(galaxy);
//	//if (ar.isLocal())
//	//	resolver & LIBV_NVP(controlledFaction);
//}

template <typename Archive> void Universe::serialize(Archive& ar) {
	ar(LIBV_NVP(nextFactionID));
	ar(LIBV_NVP(nextPlanetID));
	ar(LIBV_NVP(nextFleetID));

	ar(LIBV_NVP(factions));
//	ar(LIBV_NVP_NAMED("factions", libv::entity_primary_owner(ar.simulationContext.memory.faction, factions)));
//	ar(LIBV_NVP_NAMED("planets", libv::entity_primary_owner(ar.simulationContext.memory.planet, planets)));
//	ar(LIBV_NVP_NAMED("fleets", libv::entity_primary_owner(ar.simulationContext.memory.fleet, fleets)));

	ar(LIBV_NVP(galaxy));

	ar(LIBV_NVP_NAMED("factionNeutral", SerialID{factionNeutral_}));
	ar(LIBV_NVP_NAMED("factionControlled", SerialID{factionControlled_})); // !!! factionControlled placement )?

//	ar(LIBV_NVP(selectedFleetIDList), SnapshotType::local);
	if (ar.isLocal())
		ar(LIBV_NVP(selectedFleetIDList));
	else
//		if constexpr (Archive::is_output)
		if constexpr (Archive::is_input)
			// TODO P5: Instead of full clear, remove/update only the elements that are no longer available (for seamless snapshot update)
			selectedFleetIDList.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace space
