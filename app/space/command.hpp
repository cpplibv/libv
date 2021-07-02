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

enum class FrameIndex : int64_t {};
enum class Timestamp : int64_t {};

enum class PlayerID : int32_t {};

enum class FactionID : int32_t {};
enum class FleetID : int32_t {};

[[nodiscard]] constexpr inline auto operator+(PlayerID e) noexcept { return libv::to_underlying(e); }

[[nodiscard]] constexpr inline auto operator+(FactionID e) noexcept { return libv::to_underlying(e); }
[[nodiscard]] constexpr inline auto operator+(FleetID e) noexcept { return libv::to_underlying(e); }

// =================================================================================================

//struct CommandPlayerKick : CommandSession {
//	PlayerID playerID;
//};

struct CommandChatMessage : CommandSession {
//	PlayerID playerID;
	//	Timestamp timestamp;
	std::string sender;
	std::string message;
//	libv::serial::limit<std::string, 64 * 1024> message;

//	CommandChatMessage(PlayerID playerId, std::string message) : playerID(playerId), message(std::move(message)) {}
	CommandChatMessage(std::string sender, std::string message) : sender(std::move(sender)), message(std::move(message)) {}
};

// -------------------------------------------------------------------------------------------------

struct CommandFleetSpawn : CommandState {
//	FactionID factionID;
//	FleetID fleetID;
	libv::vec3f position;

	explicit CommandFleetSpawn(const libv::vec3f& position) : position(position) {}
};

struct CommandFleetMove : CommandState {
//	FactionID factionID;
	FleetID fleetID;
	libv::vec3f target_position;

	CommandFleetMove(FleetID fleetId, const libv::vec3f& targetPosition) : fleetID(fleetId), target_position(targetPosition) {}
};

struct CommandClearFleets : CommandState {
};

// -------------------------------------------------------------------------------------------------

struct CommandTrackView : CommandTrack {
	PlayerID playerID;
	libv::vec3f eye;
	libv::vec3f target;
	libv::vec3f mouse_direction;

	CommandTrackView(PlayerID playerId, const libv::vec3f& eye, const libv::vec3f& target, const libv::vec3f& mouseDirection) : playerID(playerId), eye(eye), target(target), mouse_direction(mouseDirection) {}
};

struct CommandCameraWarpTo : CommandTrack {
	PlayerID playerID;
	libv::vec3f target_position;

	CommandCameraWarpTo(PlayerID playerId, const libv::vec3f& target_position) : playerID(playerId), target_position(target_position) {}
};

//struct CommandCameraMovement : CommandTrack {
//	PlayerID playerID;
//	libv::vec3f eye;
//	libv::vec3f target;
//	libv::vec3f mouse_direction;
//};
//
//struct CommandMouseMovement : CommandTrack {
//	PlayerID playerID;
//	libv::vec3f mouse_position;
//	libv::vec3f mouse_direction;
//};

// -------------------------------------------------------------------------------------------------

} // namespace app
