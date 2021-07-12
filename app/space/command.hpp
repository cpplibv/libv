// Project: libv, File: app/space/command.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/vec_serial.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/serial/enable.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/utility/to_underlying.hpp>
// std
#include <cstdint>
#include <string>
//#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

enum class FrameIndex : int64_t {};
enum class Timestamp : int64_t {};

enum class PlayerID : int32_t {};

enum class FactionID : int32_t {};
enum class FleetID : int32_t {};

[[nodiscard]] constexpr inline auto operator+(FrameIndex e) noexcept { return libv::to_underlying(e); }
[[nodiscard]] constexpr inline auto operator+(Timestamp e) noexcept { return libv::to_underlying(e); }

[[nodiscard]] constexpr inline auto operator+(PlayerID e) noexcept { return libv::to_underlying(e); }

[[nodiscard]] constexpr inline auto operator+(FactionID e) noexcept { return libv::to_underlying(e); }
[[nodiscard]] constexpr inline auto operator+(FleetID e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

struct Command {

};

struct CommandSession : Command {
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

//struct CommandPlayerKick : CommandSession {
//	PlayerID playerID;
//};

struct CommandChatMessage : CommandSession {
//	PlayerID playerID;
	//	Timestamp timestamp;
	std::string sender;
	std::string message;
//	libv::serial::limit<std::string, 64 * 1024> message;

	LIBV_REFLECTION_ACCESS(sender);
	LIBV_REFLECTION_ACCESS(message);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

//	CommandChatMessage(PlayerID playerId, std::string message) : playerID(playerId), message(std::move(message)) {}
	CommandChatMessage(std::string sender, std::string message) : sender(std::move(sender)), message(std::move(message)) {}
};

// -------------------------------------------------------------------------------------------------

struct CommandFleetSpawn : CommandState {
//	FactionID factionID;
//	FleetID fleetID;
	libv::vec3f position;

	LIBV_REFLECTION_ACCESS(position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

	explicit CommandFleetSpawn(const libv::vec3f& position) : position(position) {}
};

struct CommandFleetMove : CommandState {
//	FactionID factionID;
	FleetID fleetID;
	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(fleetID);
	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

	CommandFleetMove(FleetID fleetId, const libv::vec3f& targetPosition) : fleetID(fleetId), target_position(targetPosition) {}
};

struct CommandClearFleets : CommandState {
};

struct CommandShuffle : CommandState {
};

// -------------------------------------------------------------------------------------------------

struct CommandTrackView : CommandTrack {
	PlayerID playerID;
	libv::vec3f eye;
	libv::vec3f target;
	libv::vec3f mouse_direction;

	LIBV_REFLECTION_ACCESS(playerID);
	LIBV_REFLECTION_ACCESS(eye);
	LIBV_REFLECTION_ACCESS(target);
	LIBV_REFLECTION_ACCESS(mouse_direction);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

	CommandTrackView(PlayerID playerId, const libv::vec3f& eye, const libv::vec3f& target, const libv::vec3f& mouseDirection) : playerID(playerId), eye(eye), target(target), mouse_direction(mouseDirection) {}
};

struct CommandCameraWarpTo : CommandTrack {
	PlayerID playerID;
	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(playerID);
	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

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
