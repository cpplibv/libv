// Project: libv, File: app/space/universe/universe.cpp

// hpp
#include <space/universe/universe.hpp>
// std
#include <algorithm>
#include <random>
// pro
#include <space/cto.hpp>
//#include <space/log.hpp>
//#include <space/universe/ids.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void Universe::process(CTO_FleetSpawn&& cto) {
	// Permission check
	// Bound check

	fleets.emplace_back(nextFleetID, cto.position);
	// !!! Synchronized FleetID generation
	nextFleetID = FleetID{+nextFleetID + 1};
}

void Universe::process(CTO_FleetSelect&& cto) {
	// Permission check
	// Bound check
	selectedFleetIDList.clear();
	fleetIdSelectionMap.clear();
	fleetIdSelectionMap.emplace(cto.fleetID, Fleet::SelectionStatus::selected);
	selectedFleetIDList.insert(cto.fleetID);
}

void Universe::process(CTO_FleetSelectAdd&& cto) {
	// Permission check
	// Bound check
	fleetIdSelectionMap.insert_or_assign(cto.fleetID, Fleet::SelectionStatus::selected);
	selectedFleetIDList.insert(cto.fleetID);
}

void Universe::process(CTO_FleetBoxSelect&& cto) {
	// Permission check
	// Bound check
	selectedFleetIDList.clear();
	fleetIdSelectionMap.clear();
	for (const auto & fleetID : cto.fleetIDs) {
		fleetIdSelectionMap.emplace(fleetID, Fleet::SelectionStatus::selected);
	}
	selectedFleetIDList.insert(cto.fleetIDs.begin(), cto.fleetIDs.end());
}

void Universe::process(CTO_FleetMove&& cto) {
	// Permission check
	// Bound check
	// TODO P3: Use a command API of the fleet
	for (const auto& fleetID : selectedFleetIDList) {
		fleets[+fleetID].commands.clear();
		fleets[+fleetID].commands.emplace_back(cto.target_position, FleetCommandType::movement);
	}
}

void Universe::process(CTO_FleetQueueMove&& cto) {
	// Permission check
	// Bound check
	// TODO P3: Use the command API of the fleet
	for (const auto& fleetID : selectedFleetIDList) {
		fleets[+fleetID].commands.emplace_back(cto.target_position, FleetCommandType::movement);
	}
}

void Universe::process(CTO_ClearFleets&& cto) {
	(void) cto;

	// Permission check
	// Bound check
	selectedFleetIDList.clear();
	fleets.clear();
	nextFleetID = FleetID{0};
}

void Universe::process(CTO_Shuffle&& cto) {
	// Permission check
	// Bound check
	auto positions = std::vector<libv::vec3f>{};
	for (const auto& fleet : fleets)
		positions.emplace_back(fleet.commands.empty() ? fleet.position : fleet.commands.back().target);

	auto rng = std::mt19937_64{cto.seed};

	std::ranges::shuffle(positions, rng);

	std::uniform_int_distribution dst(0, 1);
	for (size_t i = 0; i < positions.size(); ++i) {
		auto& fleet = fleets[i];
		fleet.commands.clear();
		fleet.commands.emplace_back(positions[i], dst(rng) ? FleetCommandType::movement : FleetCommandType::attack);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
