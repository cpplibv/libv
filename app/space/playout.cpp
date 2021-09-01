// Project: libv, File: app/space/playout.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/playout.hpp>
// libv
//#include <libv/math/vec.hpp>
//#include <libv/ui/chrono.hpp>
//#include <libv/utility/hex_dump.hpp>
//#include <libv/serial/reflection.hpp>
//#include <libv/serial/archive/binary.hpp>
//#include <libv/serial/codec.hpp>
// std
//#include <memory>
//#include <random>
//#include <vector>
// pro
#include <space/command.hpp>
#include <space/lobby.hpp>
#include <space/universe/ids.hpp>
#include <space/universe/universe.hpp>
//#include <space/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

//void apply(Universe& universe, CommandPlayerKick& command) {
////	state.
//}

void apply(Universe& universe, Lobby& lobby, CommandChatMessage& command) {
	(void) universe;

	// Permission check
	// Identity check
	lobby.chat_entries.emplace_back(command.sender, command.message);
}

// -------------------------------------------------------------------------------------------------

void apply(Universe& universe, Lobby& lobby, CommandFleetMove& command) {
	(void) lobby;

	// Permission check
	// Bound check

	// TODO P3: Use the command API of the fleet
	universe.fleets[+command.fleetID].commands.clear();
	universe.fleets[+command.fleetID].commands.emplace_back(command.target_position, Fleet::CommandType::movement);
}

void apply(Universe& universe, Lobby& lobby, CommandFleetQueueMove& command) {
	(void) lobby;

	// Permission check
	// Bound check

	// TODO P3: Use the command API of the fleet
	universe.fleets[+command.fleetID].commands.emplace_back(command.target_position, Fleet::CommandType::movement);
}

void apply(Universe& universe, Lobby& lobby, CommandFleetSpawn& command) {
	(void) lobby;

	// Permission check
	// Bound check
	universe.fleets.emplace_back(universe.nextFleetID, command.position);
	// !!! Synchronized FleetID generation
	universe.nextFleetID = FleetID{+universe.nextFleetID + 1};
}

void apply(Universe& universe, Lobby& lobby, CommandClearFleets& command) {
	(void) lobby;
	(void) command;

	// Permission check
	// Bound check
	universe.fleets.clear();
}

void apply(Universe& universe, Lobby& lobby, CommandShuffle& command) {
	(void) lobby;
	(void) command;

	// Permission check
	// Bound check

	auto positions = std::vector<libv::vec3f>{};
	for (const auto& fleet : universe.fleets)
		positions.emplace_back(fleet.commands.empty() ? fleet.position : fleet.commands.back().target);

	auto rng = std::mt19937_64{command.seed};

	std::ranges::shuffle(positions, rng);

	std::uniform_int_distribution dst(0, 1);
	for (size_t i = 0; i < positions.size(); ++i) {
		auto& fleet = universe.fleets[i];
		fleet.commands.clear();
		fleet.commands.emplace_back(positions[i], dst(rng) ? Fleet::CommandType::movement : Fleet::CommandType::attack);
	}
}

// -------------------------------------------------------------------------------------------------

void apply(Universe& universe, Lobby& lobby, CommandTrackView& command) {
	(void) universe;
	(void) lobby;
	(void) command; // <<< P5
}

void apply(Universe& universe, Lobby& lobby, CommandCameraWarpTo& command) {
	(void) universe;
	(void) lobby;
	(void) command; // <<< P5
}

//void apply(Universe& universe, CommandCameraMovement& command) {
////	universe.
//}
//
//void apply(Universe& universe, CommandMouseMovement& command) {
////	universe.
//}

// -------------------------------------------------------------------------------------------------

} // namespace app
