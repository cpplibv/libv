// Created by Vader on 2021.12.20..

// hpp
#include <space/universe/engine/snapshot_ptr_resolver.hpp>
// libv
#include <libv/algo/linear_find.hpp>
// pro
#include <space/universe/faction.hpp>
#include <space/universe/fleet.hpp>
#include <space/universe/planet.hpp>
#include <space/universe/universe.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

libv::entity_ptr<Faction> SnapshotPtrResolverArchive::resolve(FactionID id) const {
	auto opt = libv::linear_find_optional(universe.galaxy.factions, id, &Faction::id);
	return opt ? *opt : libv::entity_ptr<Faction>(nullptr);
}

libv::entity_ptr<Planet> SnapshotPtrResolverArchive::resolve(PlanetID id) const {
	auto opt = libv::linear_find_optional(universe.galaxy.planets, id, &Planet::id);
	return opt ? *opt : libv::entity_ptr<Planet>(nullptr);
}

libv::entity_ptr<Fleet> SnapshotPtrResolverArchive::resolve(FleetID id) const {
	auto opt = libv::linear_find_optional(universe.galaxy.fleets, id, &Fleet::id);
	return opt ? *opt : libv::entity_ptr<Fleet>(nullptr);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
