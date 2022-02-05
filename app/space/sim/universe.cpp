// Project: libv, File: app/space/sim/universe.cpp

// hpp
#include <space/sim/universe.hpp>
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
//#include <space/log.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Universe::Universe() : galaxy(memory) { // For de-serialization only
	controlledFaction = galaxy.faction("Faction 1"); // <<<
}

Universe::Universe(GalaxyGenerationSettings ggs) :
	universe_rng(ggs.seed),
	galaxy(generateGalaxy(memory, std::move(ggs))) {
	controlledFaction = galaxy.faction("Faction 1"); // <<<
}

Universe::Universe(Universe&&) noexcept = default;

Universe& Universe::operator=(Universe&& other) & noexcept {
	galaxy.kill();

	universe_rng = std::move(other.universe_rng);
	galaxy = std::move(other.galaxy);
	controlledFaction = std::move(other.controlledFaction);

	// NOTE: Memory has to be moved last to not discard still alive objects
	memory = std::move(other.memory);
	// NOTE: having to reassign galaxy's memory address, would be nice to have a better solution
	galaxy.memory = libv::make_observer_ptr(&memory);

	return *this;
}

Universe::~Universe() {
	galaxy.kill();
}

// -------------------------------------------------------------------------------------------------

void Universe::update(sim_duration delta_time) {
	//	void update(tick tick_count) {
	//		for (tick i = 0; i < tick_count; ++i) {
	//			const auto tick_time = std::chrono::duration_cast<sim_duration>(std::chrono::seconds{1}) / 60.0;
	//			const auto dt = tick_time * libv::to_underlying(tick_count);
	//
	//			galaxy.update(dt);
	//		}
	galaxy.update(delta_time);
}

void Universe::debug() {
	galaxy.debug();
}

// -------------------------------------------------------------------------------------------------

void Universe::process(CTO_FleetSpawn&& cto) {
	// Permission check
	// Bound check

//	fleets.emplace_back(cto.id, cto.position);

	// !!! Synchronized FleetID generation

	// <<< Random faction assignments, and yeah, random string generation
	auto dist = libv::make_uniform_distribution_inclusive(0, 6);
	auto faction = galaxy.faction("Faction " + std::to_string(dist(universe_rng)));
	galaxy.fleets.emplace_back(memory.fleet.create(galaxy.nextFleetID++, cto.position, std::move(faction)));
}

void Universe::process(CTO_FleetSelect&& cto) {
	// Permission check
	// Bound check
	galaxy.selectedFleetIDList.clear();
	galaxy.selectedFleetIDList.insert(cto.fleetID);
}

void Universe::process(CTO_FleetSelectAdd&& cto) {
	// Permission check
	// Bound check
	galaxy.selectedFleetIDList.insert(cto.fleetID);
}

void Universe::process(CTO_FleetClearSelection&&) {
	// Permission check
	// Bound check
	galaxy.selectedFleetIDList.clear();
}

void Universe::process(CTO_FleetSelectBox&& cto) {
	// Permission check
	// Bound check
	galaxy.selectedFleetIDList.clear();
	galaxy.selectedFleetIDList.insert(cto.fleetIDs.begin(), cto.fleetIDs.end());
}

void Universe::process(CTO_FleetMove&& cto) {
	// Permission check
	// Bound check
	for (const auto& fleetID : galaxy.selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->clearCommandQueue();
		(*fleet)->queueMoveTo(cto.target_position);
	}
}

void Universe::process(CTO_FleetMoveQueue&& cto) {
	// Permission check
	// Bound check
	for (const auto& fleetID : galaxy.selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->queueMoveTo(cto.target_position);
	}
}

void Universe::process(CTO_FleetAttackFleet&& cto) {
	// Permission check
	// Bound check
	auto target = libv::linear_find_optional(galaxy.fleets, cto.targetFleetID, &Fleet::id);
	if (!target)
		return;

	for (const auto& fleetID : galaxy.selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->clearCommandQueue();
		(*fleet)->queueAttack(*target);
	}
}

void Universe::process(CTO_FleetAttackFleetQueue&& cto) {
	// Permission check
	// Bound check
	auto target = libv::linear_find_optional(galaxy.fleets, cto.targetFleetID, &Fleet::id);
	if (!target)
		return;

	for (const auto& fleetID : galaxy.selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->queueAttack(*target);
	}
}

void Universe::process(CTO_FleetAttackPlanet&& cto) {
	// Permission check
	// Bound check
	auto target = libv::linear_find_optional(galaxy.planets, cto.targetPlanetID, &Planet::id);
	if (!target)
		return;

	for (const auto& fleetID : galaxy.selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->clearCommandQueue();
		(*fleet)->queueAttack(*target);
	}
}

void Universe::process(CTO_FleetAttackPlanetQueue&& cto) {
	// Permission check
	// Bound check
	auto target = libv::linear_find_optional(galaxy.planets, cto.targetPlanetID, &Planet::id);
	if (!target)
		return;

	for (const auto& fleetID : galaxy.selectedFleetIDList) {
		// TODO P1: O(n^2), because selection stored as IDs
		auto fleet = libv::linear_find_optional(galaxy.fleets, fleetID, &Fleet::id);
		if (!fleet)
			continue;

		(*fleet)->queueAttack(*target);
	}
}

void Universe::process(CTO_ClearFleets&&) {
	// Permission check
	// Bound check
	galaxy.selectedFleetIDList.clear();
	for (const auto& fleet : galaxy.fleets)
		fleet->kill();
	galaxy.fleets.clear();
//	galaxy.nextFleetID = firstFleetID;
}

void Universe::process(CTO_Shuffle&& cto) {
	// Permission check
	// Bound check
	auto positions = std::vector<libv::vec3f>{};
	for (const auto& fleet : galaxy.fleets)
//		positions.emplace_back(fleet.commands.empty() ? fleet.position : fleet.commands.back().target);
		positions.emplace_back(fleet->position);

	auto rng = libv::xoroshiro128(cto.seed);

	std::ranges::shuffle(positions, rng);

	auto dst = libv::make_uniform_distribution_inclusive(0, 1);
	for (std::size_t i = 0; i < positions.size(); ++i) {
		auto& fleet = *galaxy.fleets[i];
		fleet.clearCommandQueue();
		if (dst(rng))
			fleet.queueAttack(positions[i]);
		else
			fleet.queueMoveTo(positions[i]);
//		fleet.commands.clear();
//		fleet.commands.emplace_back(positions[i], dst(rng) ? FleetCommandType::movement : FleetCommandType::attack);
	}
}

void Universe::process(CTO_PlanetSpawn&& cto) {
	// Permission check
	// Bound check
	auto rng_planet = libv::xoroshiro128(+galaxy.nextPlanetID);
	// !!! Synchronized PlanetID generation (Split request and action CTO's?)
	galaxy.planets.emplace_back(generatePlanet(memory, galaxy.nextPlanetID++, cto.position, rng_planet));
	galaxy.planets.back()->faction = galaxy.factionNeutral();
}

void Universe::process(CTO_ClearPlanets&&) {
	// Permission check
	// Bound check
	galaxy.planets.clear();
//	galaxy.nextPlanetID = firstPlanetID;
}

// -------------------------------------------------------------------------------------------------

} // namespace space
