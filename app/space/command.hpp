// Project: libv, File: app/space/icons.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cstdint>
//#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Command {

};

struct CommandSession : Command {

};

struct CommandState : Command {

};

struct CommandTrack : Command {

};

// =================================================================================================

struct CommandPlayerKick : CommandSession {
	int32_t playerID;
};

struct CommandChatMessage : CommandSession {
	int32_t senderPlayerID;
	//	int32_t timestamp;
	std::string message;
};

// -------------------------------------------------------------------------------------------------

struct CommandFleetMove : CommandState {
	int32_t factionID;
	int32_t fleetID;
	libv::vec3f target_position;
};

// -------------------------------------------------------------------------------------------------

struct CommandCameraMovement : CommandTrack {
	libv::vec3f eye;
	libv::vec3f target;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
