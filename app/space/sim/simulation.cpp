// Project: libv, File: app/space/sim/simulation.cpp

// hpp
#include <space/sim/simulation.hpp>
// libv
////#include <libv/serial/codec_message_id.hpp>
//#include <libv/utility/random/xoroshiro128.hpp>
//// pro
////#include <space/sim/engine/chrono.hpp>
//#include <space/sim/engine/memory_store.hpp>
////#include <space/sim/galaxy.hpp>

//// hpp
//#include <space/sim/universe.hpp>
// libv
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/archive/json_any.hpp>
#include <libv/serial/serial.hpp>
#include <libv/utility/random/xoroshiro128_serial.hpp>
// pro
//#include <space/sim/engine/snapshot_archive_cc.hpp>
#include <space/sim/engine/snapshot_archive.hpp>
#include <space/sim/engine/snapshot_ptr_resolver.hpp>
#include <space/sim/galaxy_serial.hpp>

// hpp
// libv
#include <libv/algo/linear_find.hpp>
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// std
#include <algorithm>
// pro
#include <space/message/cto.hpp>
#include <space/sim/faction.hpp>
#include <space/sim/fleet.hpp>
#include <space/sim/gen/generation.hpp>
#include <space/sim/gen/generation_settings.hpp>
#include <space/sim/planet.hpp>
//#include <space/sim/colony.hpp>
//#include <space/sim/empire.hpp>
//#include <space/sim/trade.hpp>
#include <space/log.hpp>

//#include <space/sim/universe.hpp>
#include <space/sim/universe.hpp>
#include <space/sim/universe_serial.hpp>

#include <space/sim/engine/snapshot_archive.hpp>



namespace space {

// -------------------------------------------------------------------------------------------------

Simulation::Simulation() :
	universe(std::make_unique<Universe>(ctx)) {
}

Simulation::~Simulation() {
	// For the sake of forward declared ptr
}

// -------------------------------------------------------------------------------------------------

//template <typename Archive> void SimulationSnapshot::save(Archive& ar) const {
//}
//
//template <typename Archive> void SimulationSnapshot::load(Archive& ar) {
//}

void Simulation::saveSnapshot(libv::archive::BinaryOutput& ar, SnapshotType type) const {
	log_space.fatal("Simulation::saveSnapshot: Uni: {}", static_cast<void*>(universe.get()));
	log_space.fatal("Simulation::saveSnapshot 3: Uni: {} Nat: {} Cfa: {}",
			static_cast<void*>(universe.get()),
			static_cast<void*>(universe->factionNeutral().get()),
			static_cast<void*>(universe->factionControlled().get())
		);

	SnapshotArchive<libv::archive::BinaryOutput> sar{ar, *ctx, type};

	sar(LIBV_NVP_FORCED("universe", *universe));

	{
		std::string out;
		{
			libv::archive::JSONAnyOutput jar{out};
			SnapshotArchive<libv::archive::JSONAnyOutput> sjar{jar, *ctx, type};
			sjar(LIBV_NVP_FORCED("universe", *universe));
		}
		log_space.info("{}", out);
	}
}

void Simulation::loadSnapshot(libv::archive::BinaryInput& ar, SnapshotType type) {
	ctx = std::make_shared<SimulationContext>();
	universe = std::make_unique<Universe>(ctx);

	log_space.fatal("Simulation::loadSnapshot 1");

	SnapshotArchive<libv::archive::BinaryInput> sar{ar, *ctx, type};

	sar(LIBV_NVP_FORCED("universe", *universe));

	log_space.fatal("Simulation::loadSnapshot 2");
	log_space.fatal("Simulation::loadSnapshot 3pre: Uni: {}", static_cast<void*>(universe.get()));

	log_space.fatal("Simulation::loadSnapshot 3: Uni: {} Nat: {} Cfa: {}",
			static_cast<void*>(universe.get()),
			static_cast<void*>(universe->factionNeutral().get()),
			static_cast<void*>(universe->factionControlled().get())
		);

	SnapshotPtrResolverArchive resolver{*universe, type};

	resolver(LIBV_NVP_FORCED("universe", *universe));


	log_space.fatal("Simulation::loadSnapshot 4: Uni: {} Nat: {} Cfa: {}",
			static_cast<void*>(universe.get()),
			static_cast<void*>(universe->factionNeutral().get()),
			static_cast<void*>(universe->factionControlled().get())
		);

	{
		std::string out;
		{
			libv::archive::JSONAnyOutput ar{out};
			SnapshotArchive<libv::archive::JSONAnyOutput> sar{ar, *ctx, type};
			sar(LIBV_NVP_FORCED("universe", *universe));
		}
		log_space.info("Loaded state resave:\n{}\nend of loaded state resave", out);
	}
}

std::string Simulation::debugAsJson() const {
	std::string out;
	{
		libv::archive::JSONAnyOutput ar{out};
		SnapshotArchive<libv::archive::JSONAnyOutput> sar{ar, *ctx, SnapshotType::local};
		sar(LIBV_NVP_FORCED("universe", *universe));
	}

	return out;
}

SnapshotHash Simulation::hashState() const {
//	SnapshotArchive<libv::archive::BasicHasher> oar{isLocal, snapshot.data};
//	0;
	// TODO P1: app.space:
	return SnapshotHash{0};
}

// -------------------------------------------------------------------------------------------------

void Simulation::update(sim_duration delta_time) {
	universe->update(delta_time);
}

// =================================================================================================

void Simulation::process(CTO_FleetSpawn&& cto) {
	// Permission check
	// Bound check

//	fleets.emplace_back(cto.id, cto.position);

	// TODO P1: app.space: Synchronized FleetID generation

	// <<< Random faction assignments, and yeah, random string generation
	auto dist = libv::make_uniform_distribution_inclusive(0, 6);
	auto faction = universe->faction("Faction " + std::to_string(dist(simulationRNG)));
	universe->galaxy.fleets.emplace_back(ctx->memory.fleet.create(universe->nextFleetID++, cto.position, std::move(faction)));
}

void Simulation::process(CTO_FleetSelect&& cto) {
	// Permission check
	// Bound check
	universe->selectedFleetIDList.clear();
	universe->selectedFleetIDList.insert(cto.fleetID);
}

void Simulation::process(CTO_FleetSelectAdd&& cto) {
	// Permission check
	// Bound check
	universe->selectedFleetIDList.insert(cto.fleetID);
}

void Simulation::process(CTO_FleetClearSelection&&) {
	// Permission check
	// Bound check
	universe->selectedFleetIDList.clear();
}

void Simulation::process(CTO_FleetSelectBox&& cto) {
	// Permission check
	// Bound check
	universe->selectedFleetIDList.clear();
	universe->selectedFleetIDList.insert(cto.fleetIDs.begin(), cto.fleetIDs.end());
}

void Simulation::process(CTO_FleetMove&& cto) {
	// Permission check
	// Bound check
	for (const auto& fleetID : universe->selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(universe->galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->clearCommandQueue();
		(*fleet)->queueMoveTo(cto.target_position);
	}
}

void Simulation::process(CTO_FleetMoveQueue&& cto) {
	// Permission check
	// Bound check
	for (const auto& fleetID : universe->selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(universe->galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->queueMoveTo(cto.target_position);
	}
}

void Simulation::process(CTO_FleetAttackFleet&& cto) {
	// Permission check
	// Bound check
	auto target = libv::linear_find_optional(universe->galaxy.fleets, cto.targetFleetID, &Fleet::id);
	if (!target)
		return;

	for (const auto& fleetID : universe->selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(universe->galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->clearCommandQueue();
		(*fleet)->queueAttack(*target);
	}
}

void Simulation::process(CTO_FleetAttackFleetQueue&& cto) {
	// Permission check
	// Bound check
	auto target = libv::linear_find_optional(universe->galaxy.fleets, cto.targetFleetID, &Fleet::id);
	if (!target)
		return;

	for (const auto& fleetID : universe->selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(universe->galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->queueAttack(*target);
	}
}

void Simulation::process(CTO_FleetAttackPlanet&& cto) {
	// Permission check
	// Bound check
	auto target = libv::linear_find_optional(universe->galaxy.planets, cto.targetPlanetID, &Planet::id);
	if (!target)
		return;

	for (const auto& fleetID : universe->selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(universe->galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->clearCommandQueue();
		(*fleet)->queueAttack(*target);
	}
}

void Simulation::process(CTO_FleetAttackPlanetQueue&& cto) {
	// Permission check
	// Bound check
	auto target = libv::linear_find_optional(universe->galaxy.planets, cto.targetPlanetID, &Planet::id);
	if (!target)
		return;

	for (const auto& fleetID : universe->selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(universe->galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->queueAttack(*target);
	}
}

void Simulation::process(CTO_ClearFleets&&) {
	// Permission check
	// Bound check
	universe->selectedFleetIDList.clear();
	for (const auto& fleet : universe->galaxy.fleets)
		fleet->kill();
	universe->galaxy.fleets.clear();
//	universe->nextFleetID = firstFleetID;
}

void Simulation::process(CTO_Shuffle&&) {
	// Permission check
	// Bound check
	auto positions = std::vector<libv::vec3f>{};
	for (const auto& fleet : universe->galaxy.fleets)
//		positions.emplace_back(fleet.commands.empty() ? fleet.position : fleet.commands.back().target);
		positions.emplace_back(fleet->position);

	auto rng = simulationRNG.fork();

	std::ranges::shuffle(positions, rng);

	auto dst = libv::make_uniform_distribution_inclusive(0, 1);
	for (std::size_t i = 0; i < positions.size(); ++i) {
		auto& fleet = *universe->galaxy.fleets[i];
		fleet.clearCommandQueue();
		if (dst(rng))
			fleet.queueAttack(positions[i]);
		else
			fleet.queueMoveTo(positions[i]);
//		fleet.commands.clear();
//		fleet.commands.emplace_back(positions[i], dst(rng) ? FleetCommandType::movement : FleetCommandType::attack);
	}
}

void Simulation::process(CTO_PlanetSpawn&& cto) {
	log_space.fatal("{} {} {}",
			static_cast<void*>(universe.get()),
			static_cast<void*>(universe->factionNeutral().get()),
			static_cast<void*>(universe->factionControlled().get())
		);
	// Permission check
	// Bound check
	auto rng_planet = libv::xoroshiro128(+universe->nextPlanetID);
	// TODO P1: app.space: Synchronized PlanetID generation (Split request and action CTO's?)
	universe->galaxy.planets.emplace_back(generatePlanet(ctx, universe->nextPlanetID++, cto.position, rng_planet));
	universe->galaxy.planets.back()->faction = universe->factionNeutral();
}

void Simulation::process(CTO_ClearPlanets&&) {
	// Permission check
	// Bound check
	universe->galaxy.planets.clear();
//	galaxy.nextPlanetID = firstPlanetID;
}

// -------------------------------------------------------------------------------------------------

} // namespace space
