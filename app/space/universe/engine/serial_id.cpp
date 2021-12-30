// Created by Vader on 2021.12.20..

// hpp
#include <space/universe/engine/serial_id.hpp>
// libv
#include <libv/algo/linear_find.hpp>
#include <libv/utility/projection.hpp>
// pro
#include <space/universe/universe.hpp>

// TODO P3: Further refine project include hierarchy
#include <space/universe/fleet.hpp>
#include <space/universe/planet.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

//Faction* SnapshotPtrResolverArchive::resolve(FactionID id) const {
//	const auto faction = libv::linear_find_optional(universe.galaxy.factions, id, libv::proj_indirect(&Faction::id));
//	return const_cast<Faction*>(faction);
//}

Planet* SnapshotPtrResolverArchive::resolve(PlanetID id) const {
	const auto planet = libv::linear_find_optional(universe.galaxy.planets, id, &Planet::id);
	return const_cast<Planet*>(planet);
}

Fleet* SnapshotPtrResolverArchive::resolve(FleetID id) const {
	const auto fleet = libv::linear_find_optional(universe.galaxy.fleets, id, &Fleet::id);
	return const_cast<Fleet*>(fleet);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
