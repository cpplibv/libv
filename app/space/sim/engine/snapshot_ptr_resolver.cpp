// Project: libv, File: app/space/sim/engine/snapshot_ptr_resolver.cpp

// hpp
#include <space/sim/engine/snapshot_ptr_resolver.hpp>
// libv
#include <libv/algo/linear_find.hpp>
// pro
#include <space/log.hpp>
#include <space/sim/faction.hpp>
#include <space/sim/fleet.hpp>
#include <space/sim/planet.hpp>
#include <space/sim/universe.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

// TODO P4: log_space.error_if most likely should be an exception

libv::entity_ptr<Faction> SnapshotPtrResolverArchive::resolve(FactionID id) const {
	auto opt = libv::linear_find_optional(universe.factions, id, &Faction::id);
	log_space.error_if(!opt, "Failed to resolve FactionID: {}", +id);
	return opt ? libv::entity_ptr<Faction>(*opt) : libv::entity_ptr<Faction>(nullptr);
}

libv::entity_ptr<Planet> SnapshotPtrResolverArchive::resolve(PlanetID id) const {
	auto opt = libv::linear_find_optional(universe.galaxy.planets, id, &Planet::id);
	log_space.error_if(!opt, "Failed to resolve PlanetID: {}", +id);
	return opt ? libv::entity_ptr<Planet>(*opt) : libv::entity_ptr<Planet>(nullptr);
}

libv::entity_ptr<Fleet> SnapshotPtrResolverArchive::resolve(FleetID id) const {
	auto opt = libv::linear_find_optional(universe.galaxy.fleets, id, &Fleet::id);
	log_space.error_if(!opt, "Failed to resolve FleetID: {}", +id);
	return opt ? libv::entity_ptr<Fleet>(*opt) : libv::entity_ptr<Fleet>(nullptr);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
