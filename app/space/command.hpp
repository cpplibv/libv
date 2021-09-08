// Project: libv, File: app/space/command.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/vec_serial.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/serial/enable.hpp>
#include <libv/serial/types/std_string.hpp>
// std
#include <cstdint>
#include <string>
// pro
#include <space/universe/ids.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Command {

};

struct CommandLobby : Command {
	static constexpr bool is_session_command = true;
	static constexpr bool is_state_command = false;
	static constexpr bool is_track_command = false;
};

struct CommandState : Command {
	static constexpr bool is_session_command = false;
	static constexpr bool is_state_command = true;
	static constexpr bool is_track_command = false;
};

struct CommandTrack : Command {
	static constexpr bool is_session_command = false;
	static constexpr bool is_state_command = false;
	static constexpr bool is_track_command = true;
};

// -------------------------------------------------------------------------------------------------

//struct CommandPlayerKick {
//	using command_type = CommandLobby;
//
//	PlayerID playerID;
//};

struct CommandChatMessage {
	using command_type = CommandLobby;

//	PlayerID playerID;
	//	Timestamp timestamp;
	std::string sender;
	std::string message;
//	libv::serial::limit<std::string, 64 * 1024> message;

	LIBV_REFLECTION_ACCESS(sender);
	LIBV_REFLECTION_ACCESS(message);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

struct CommandFleetSpawn {
	using command_type = CommandState;

//	FactionID factionID;
//	FleetID fleetID;
	libv::vec3f position;

	LIBV_REFLECTION_ACCESS(position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CommandFleetSelect {
	using command_type = CommandState;

	FleetID fleetID;

	LIBV_REFLECTION_ACCESS(fleetID);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CommandFleetSelectAdd {
	using command_type = CommandState;

	FleetID fleetID;

	LIBV_REFLECTION_ACCESS(fleetID);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CommandFleetMove {
	using command_type = CommandState;

	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CommandFleetQueueMove {
	using command_type = CommandState;

	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CommandClearFleets {
	using command_type = CommandState;

	LIBV_REFLECTION_EMPTY();
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CommandShuffle {
	using command_type = CommandState;

	uint64_t seed;

	LIBV_REFLECTION_ACCESS(seed);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

struct CommandTrackView {
	using command_type = CommandTrack;

	PlayerID playerID;
	libv::vec3f eye;
	libv::vec3f target;
	libv::vec3f mouse_direction;

	LIBV_REFLECTION_ACCESS(playerID);
	LIBV_REFLECTION_ACCESS(eye);
	LIBV_REFLECTION_ACCESS(target);
	LIBV_REFLECTION_ACCESS(mouse_direction);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CommandCameraWarpTo {
	using command_type = CommandTrack;

	PlayerID playerID;
	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(playerID);
	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

//struct CommandCameraMovement : CommandTrack {
//	using command_type = CommandTrack;
//
//	PlayerID playerID;
//	libv::vec3f eye;
//	libv::vec3f target;
//	libv::vec3f mouse_direction;
//};
//
//struct CommandMouseMovement : CommandTrack {
//	using command_type = CommandTrack;
//
//	PlayerID playerID;
//	libv::vec3f mouse_position;
//	libv::vec3f mouse_direction;
//};

// -------------------------------------------------------------------------------------------------

} // namespace app
