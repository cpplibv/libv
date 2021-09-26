// Project: libv, File: app/space/hpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/cto.hpp>
// libv
//#include <libv/math/vec.hpp>
//#include <libv/math/vec_serial.hpp>
//#include <libv/meta/reflection_access.hpp>
//#include <libv/serial/enable.hpp>
//#include <libv/serial/types/std_string.hpp>
// std
//#include <cstdint>
//#include <string>
#include <algorithm>
#include <random>
// pro
#include <space/log.hpp>
#include <space/network/lobby.hpp>
#include <space/universe/ids.hpp>
#include <space/universe/ids.hpp>
#include <space/universe/universe.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

//void CTO_PlayerKick::apply(Universe& universe) {
////	state.
//}

void CTO_ChatMessage::apply(Universe& universe, Lobby& lobby) {
	(void) universe;

	// Permission check
	// Identity check
	lobby.chat_entries.emplace_back(sender, message);
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetMove::apply(Universe& universe, Lobby& lobby) {
	(void) lobby;

	// Permission check
	// Bound check
	// TODO P3: Use the command API of the fleet
	for (const auto& fleetID : universe.selectedFleetIDList) {
		universe.fleets[+fleetID].commands.clear();
		universe.fleets[+fleetID].commands.emplace_back(target_position, Fleet::CommandType::movement);
	}
}

void CTO_FleetQueueMove::apply(Universe& universe, Lobby& lobby) {
	(void) lobby;

	// Permission check
	// Bound check
	// TODO P3: Use the command API of the fleet
	for (const auto& fleetID : universe.selectedFleetIDList) {
		universe.fleets[+fleetID].commands.emplace_back(target_position, Fleet::CommandType::movement);
	}
}

void CTO_FleetSpawn::apply(Universe& universe, Lobby& lobby) {
	(void) lobby;

	// Permission check
	// Bound check

	universe.fleets.emplace_back(universe.nextFleetID, position);
	// !!! Synchronized FleetID generation
	universe.nextFleetID = FleetID{+universe.nextFleetID + 1};
}

void CTO_FleetSelect::apply(Universe& universe, Lobby& lobby) {
	(void) lobby;

	universe.selectedFleetIDList.clear();
	universe.selectedFleetIDList.insert(fleetID);
}

void CTO_FleetSelectAdd::apply(Universe& universe, Lobby& lobby) {
	(void) lobby;
	universe.selectedFleetIDList.insert(fleetID);
}

// paging
//	const auto prev_selection = universe.selectedFleetID;
//	// Permission check
//	// Bound check
//	// !!! Synchronized FleetID generation
//	for (auto it = universe.fleets.begin(); it != universe.fleets.end(); ++it) {
//		if (it->id == universe.selectedFleetID) {
//			if ((++it) != universe.fleets.end())
//				universe.selectedFleetID = it->id;
//			else
//				universe.selectedFleetID = universe.fleets.front().id;
//			break;
//		}
//	}

void CTO_ClearFleets::apply(Universe& universe, Lobby& lobby) {
	(void) lobby;

	// Permission check
	// Bound check
	universe.selectedFleetIDList.clear();
	universe.fleets.clear();
	universe.nextFleetID = FleetID{0};
}

void CTO_Shuffle::apply(Universe& universe, Lobby& lobby) {
	(void) lobby;

	// Permission check
	// Bound check
	auto positions = std::vector<libv::vec3f>{};
	for (const auto& fleet : universe.fleets)
		positions.emplace_back(fleet.commands.empty() ? fleet.position : fleet.commands.back().target);

	auto rng = std::mt19937_64{seed};

	std::ranges::shuffle(positions, rng);

	std::uniform_int_distribution dst(0, 1);
	for (size_t i = 0; i < positions.size(); ++i) {
		auto& fleet = universe.fleets[i];
		fleet.commands.clear();
		fleet.commands.emplace_back(positions[i], dst(rng) ? Fleet::CommandType::movement : Fleet::CommandType::attack);
	}
}

// -------------------------------------------------------------------------------------------------

void CTO_TrackView::apply(Universe& universe, Lobby& lobby) {
	(void) universe;
	(void) lobby;

	// <<< P5: Implement
}

void CTO_CameraWarpTo::apply(Universe& universe, Lobby& lobby) {
	(void) universe;
	(void) lobby;

	// <<< P5: Implement
}

//void CTO_CameraMovement::apply(Universe& universe) {
////	universe.
//}
//
//void CTO_MouseMovement::apply(Universe& universe) {
////	universe.
//}

// -------------------------------------------------------------------------------------------------

} // namespace app
