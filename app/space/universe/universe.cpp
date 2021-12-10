// Project: libv, File: app/space/universe/universe.cpp

// hpp
#include <space/universe/universe.hpp>
// libv
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/archive/json_any.hpp>
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// std
#include <algorithm>
#include <random>
// pro
#include <space/cto.hpp>
#include <space/universe/generation/generation.hpp>
//#include <space/log.hpp>
//#include <space/universe/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Universe::Universe(GalaxyGenerationSettings ggs) :
	galaxy(generateGalaxy(ggs)) {
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

void Universe::save(libv::archive::Binary::output& ar) const {
	const_cast<Universe*>(this)->aux_serialize(ar);
}

void Universe::save(libv::archive::JSONAny::output& ar) const {
	const_cast<Universe*>(this)->aux_serialize(ar);
}

void Universe::load(libv::archive::Binary::input& ar) {
	aux_serialize(ar);
}

void Universe::load(libv::archive::JSONAny::input& ar) {
	aux_serialize(ar);
}

// -------------------------------------------------------------------------------------------------

void Universe::process(CTO_FleetSpawn&& cto) {
	// Permission check
	// Bound check

//	fleets.emplace_back(cto.id, cto.position);

	// !!! Synchronized FleetID generation
	galaxy.fleets.emplace_back(galaxy.nextFleetID++, cto.position);
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

void Universe::process(CTO_FleetBoxSelect&& cto) {
	// Permission check
	// Bound check
	galaxy.selectedFleetIDList.clear();
	galaxy.selectedFleetIDList.insert(cto.fleetIDs.begin(), cto.fleetIDs.end());
}

void Universe::process(CTO_FleetClearSelection&&) {
	// Permission check
	// Bound check
	galaxy.selectedFleetIDList.clear();
}

void Universe::process(CTO_FleetMove&& cto) {
	// Permission check
	// Bound check
	// TODO P3: Use a command API of the fleet
	for (const auto& fleetID : galaxy.selectedFleetIDList) {
		galaxy.fleets[+fleetID].commands.clear();
		galaxy.fleets[+fleetID].commands.emplace_back(cto.target_position, FleetCommandType::movement);
	}
}

void Universe::process(CTO_FleetQueueMove&& cto) {
	// Permission check
	// Bound check
	// TODO P3: Use the command API of the fleet
	for (const auto& fleetID : galaxy.selectedFleetIDList) {
		galaxy.fleets[+fleetID].commands.emplace_back(cto.target_position, FleetCommandType::movement);
	}
}

void Universe::process(CTO_ClearFleets&&) {
	// Permission check
	// Bound check
	galaxy.selectedFleetIDList.clear();
	galaxy.fleets.clear();
//	galaxy.nextFleetID = FleetID{0};
}

void Universe::process(CTO_Shuffle&& cto) {
	// Permission check
	// Bound check
	auto positions = std::vector<libv::vec3f>{};
	for (const auto& fleet : galaxy.fleets)
		positions.emplace_back(fleet.commands.empty() ? fleet.position : fleet.commands.back().target);

	auto rng = libv::xoroshiro128(cto.seed);

	std::ranges::shuffle(positions, rng);

	auto dst = libv::make_uniform_distribution_inclusive(0, 1);
	for (std::size_t i = 0; i < positions.size(); ++i) {
		auto& fleet = galaxy.fleets[i];
		fleet.commands.clear();
		fleet.commands.emplace_back(positions[i], dst(rng) ? FleetCommandType::movement : FleetCommandType::attack);
	}
}

void Universe::process(CTO_PlanetSpawn&& cto) {
	// Permission check
	// Bound check
	auto rng_planet = libv::xoroshiro128(+galaxy.nextPlanetID);
	// !!! Synchronized PlanetID generation (Split request and action CTO's?)
	galaxy.planets.emplace_back(generatePlanet(galaxy.nextPlanetID++, cto.position, rng_planet));
}

void Universe::process(CTO_ClearPlanets&&) {
	// Permission check
	// Bound check
	galaxy.planets.clear();
//	galaxy.nextPlanetID = PlanetID{0};
}

// -------------------------------------------------------------------------------------------------

} // namespace space
