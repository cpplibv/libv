// Project: libv, File: app/space/cto.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
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

// CTO - Command Transfer Object

// -------------------------------------------------------------------------------------------------

struct CTOType {

};

struct CTOType_Lobby : CTOType {
	static constexpr bool is_session_command = true;
	static constexpr bool is_state_command = false;
	static constexpr bool is_track_command = false;
};

struct CTOType_State : CTOType {
	static constexpr bool is_session_command = false;
	static constexpr bool is_state_command = true;
	static constexpr bool is_track_command = false;
};

struct CTOType_Track : CTOType {
	static constexpr bool is_session_command = false;
	static constexpr bool is_state_command = false;
	static constexpr bool is_track_command = true;
};

// -------------------------------------------------------------------------------------------------

//struct CTO_PlayerKick {
//	using type = CTOType_Lobby;
//
//	PlayerID playerID;
//};

struct CTO_ChatMessage {
	using type = CTOType_Lobby;

//	PlayerID playerID;
	//	Timestamp timestamp;
	std::string sender;
	std::string message;
//	libv::serial::limit<std::string, 64 * 1024> message;

	LIBV_REFLECTION_ACCESS(sender);
	LIBV_REFLECTION_ACCESS(message);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

// -------------------------------------------------------------------------------------------------

struct CTO_FleetSpawn {
	using type = CTOType_State;

//	FactionID factionID;
//	FleetID fleetID;
	libv::vec3f position;

	LIBV_REFLECTION_ACCESS(position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

struct CTO_FleetSelect {
	using type = CTOType_State;

	FleetID fleetID;

	LIBV_REFLECTION_ACCESS(fleetID);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

struct CTO_FleetSelectAdd {
	using type = CTOType_State;

	FleetID fleetID;

	LIBV_REFLECTION_ACCESS(fleetID);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

struct CTO_FleetMove {
	using type = CTOType_State;

	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

struct CTO_FleetQueueMove {
	using type = CTOType_State;

	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

struct CTO_ClearFleets {
	using type = CTOType_State;

	LIBV_REFLECTION_EMPTY();
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

struct CTO_Shuffle {
	using type = CTOType_State;

	uint64_t seed;

	LIBV_REFLECTION_ACCESS(seed);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

// -------------------------------------------------------------------------------------------------

struct CTO_TrackView {
	using type = CTOType_Track;

	PlayerID playerID;
	libv::vec3f eye;
	libv::vec3f target;
	libv::vec3f mouse_direction;

	LIBV_REFLECTION_ACCESS(playerID);
	LIBV_REFLECTION_ACCESS(eye);
	LIBV_REFLECTION_ACCESS(target);
	LIBV_REFLECTION_ACCESS(mouse_direction);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

struct CTO_CameraWarpTo {
	using type = CTOType_Track;

	PlayerID playerID;
	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(playerID);
	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

//struct CTO_CameraMovement : CommandTrack {
//	using type = CTOType_Track;
//
//	PlayerID playerID;
//	libv::vec3f eye;
//	libv::vec3f target;
//	libv::vec3f mouse_direction;
//};
//
//struct CTO_MouseMovement : CommandTrack {
//	using type = CTOType_Track;
//
//	PlayerID playerID;
//	libv::vec3f mouse_position;
//	libv::vec3f mouse_direction;
//};

// -------------------------------------------------------------------------------------------------

} // namespace app
