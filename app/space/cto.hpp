// Project: libv, File: app/space/cto.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/math/vec_serial.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/serial/codec_message_id.hpp>
#include <libv/serial/enable.hpp>
#include <libv/serial/reflection.hpp>
#include <libv/serial/types/std_chrono.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/serial/types/std_vector.hpp>
// std
#include <chrono>
#include <cstdint>
#include <string>
// pro
#include <space/universe/ids.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

/// CTO - Command Transfer Object
///
/// Main group of CTO messages:
/// 	Lobby: Lobby messages relate to the network lobby state replication to clients. All info comes from the host
///				Delay acceptable
/// 	State:
///				Delay not acceptable, always highest priority messages
/// 	Tracking: WIP: Camera position, Selections and other view related tracking information
///				Delay acceptable

// -------------------------------------------------------------------------------------------------

struct CTO_Introduction {
//	using lobby_command = void;
	static constexpr libv::serial::CodecMessageID id{0xA0};

	//	UserID userID;
	std::string user_name;
	uint64_t version;

	LIBV_REFLECTION_ACCESS(user_name);
	LIBV_REFLECTION_ACCESS(version);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

struct CTO_ClientJoined {
	using lobby_command = void;
	static constexpr libv::serial::CodecMessageID id{0x10};

	UserID userID;
	std::string user_name;
	std::chrono::system_clock::time_point joined_at;
//	Timestamp joined_at;
	uint64_t version;

	LIBV_REFLECTION_ACCESS(userID);
	LIBV_REFLECTION_ACCESS(user_name);
	LIBV_REFLECTION_ACCESS(joined_at);
	LIBV_REFLECTION_ACCESS(version);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CTO_ClientLeave {
	using lobby_command = void;
	static constexpr libv::serial::CodecMessageID id{0x11};

	UserID userID;
	//	Timestamp timestamp;

	LIBV_REFLECTION_ACCESS(userID);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

//struct CTO_ClientKick {
//	using lobby_command = void;
//	static constexpr libv::serial::CodecMessageID id{0x11};
//
//	UserID userID;
//
//	LIBV_REFLECTION_ACCESS(userID);
//	LIBV_SERIALIZATION_ENABLE_REFLECTION();
//};

struct CTO_ChatMessage {
	using lobby_command = void;
	static constexpr libv::serial::CodecMessageID id{0x12};

	UserID userID;
	std::chrono::system_clock::time_point sent_at;
	//	Timestamp sent_at;
	std::string message;
//	libv::serial::limit<std::string, 64 * 1024> message;

	LIBV_REFLECTION_ACCESS(userID);
	LIBV_REFLECTION_ACCESS(sent_at);
	LIBV_REFLECTION_ACCESS(message);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct CTO_LobbyStatus {
	using lobby_command = void;
	static constexpr libv::serial::CodecMessageID id{0x13};

	struct Entry {
		UserID userID;
		float ping;
//		float jitter;
//		float packet_loss;

		LIBV_REFLECTION_ACCESS(userID);
		LIBV_REFLECTION_ACCESS(ping);
//		LIBV_REFLECTION_ACCESS(jitter);
//		LIBV_REFLECTION_ACCESS(packet_loss);
		LIBV_SERIALIZATION_ENABLE_REFLECTION();
	};

	std::vector<Entry> pings;

	LIBV_REFLECTION_ACCESS(pings);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

//struct CTO_LobbyClose {
//	using lobby_command = void;
//	static constexpr libv::serial::CodecMessageID id{0x14};
//
//	LIBV_REFLECTION_EMPTY();
//	LIBV_SERIALIZATION_ENABLE_REFLECTION();
//};

// -------------------------------------------------------------------------------------------------

struct CTO_FleetSpawn {
	using state_command = void;
	static constexpr libv::serial::CodecMessageID id{0x20};

//	FactionID factionID;
//	FleetID fleetID;
	libv::vec3f position;

	LIBV_REFLECTION_ACCESS(position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe);
};

struct CTO_FleetSelect {
	using state_command = void;
	static constexpr libv::serial::CodecMessageID id{0x21};

	FleetID fleetID;

	LIBV_REFLECTION_ACCESS(fleetID);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe);
};

struct CTO_FleetSelectAdd {
	using state_command = void;
	static constexpr libv::serial::CodecMessageID id{0x22};

	FleetID fleetID;

	LIBV_REFLECTION_ACCESS(fleetID);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe);
};

struct CTO_FleetBoxSelect {
	using state_command = void;
	static constexpr libv::serial::CodecMessageID id{0x23};

	std::vector<FleetID> fleetIDs;

	LIBV_REFLECTION_ACCESS(fleetIDs);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe);
};

struct CTO_FleetMove {
	using state_command = void;
	static constexpr libv::serial::CodecMessageID id{0x24};

	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe);
};

struct CTO_FleetQueueMove {
	using state_command = void;
	static constexpr libv::serial::CodecMessageID id{0x25};

	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe);
};

struct CTO_ClearFleets {
	using state_command = void;
	static constexpr libv::serial::CodecMessageID id{0x26};

	LIBV_REFLECTION_EMPTY();
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe);
};

struct CTO_Shuffle {
	using state_command = void;
	static constexpr libv::serial::CodecMessageID id{0x27};

	uint64_t seed;

	LIBV_REFLECTION_ACCESS(seed);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe);
};

// -------------------------------------------------------------------------------------------------

struct CTO_TrackView {
	using track_command = void;
	static constexpr libv::serial::CodecMessageID id{0x30};

	UserID userID;
	libv::vec3f eye;
	libv::vec3f target;
	libv::vec3f mouse_direction;

	LIBV_REFLECTION_ACCESS(userID);
	LIBV_REFLECTION_ACCESS(eye);
	LIBV_REFLECTION_ACCESS(target);
	LIBV_REFLECTION_ACCESS(mouse_direction);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

struct CTO_CameraWarpTo {
	using track_command = void;
	static constexpr libv::serial::CodecMessageID id{0x31};

	UserID userID;
	libv::vec3f target_position;

	LIBV_REFLECTION_ACCESS(userID);
	LIBV_REFLECTION_ACCESS(target_position);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	void apply(Universe& universe, Lobby& lobby);
};

//struct CTO_CameraMovement : CommandTrack {
//	using track_command = void;
//	static constexpr libv::serial::CodecMessageID id{0x32};
//
//	UserID userID;
//	libv::vec3f eye;
//	libv::vec3f target;
//	libv::vec3f mouse_direction;
//};
//
//struct CTO_MouseMovement : CommandTrack {
//	using track_command = void;
//	static constexpr libv::serial::CodecMessageID id{0x33};
//
//	UserID userID;
//	libv::vec3f mouse_position;
//	libv::vec3f mouse_direction;
//};

// -------------------------------------------------------------------------------------------------

} // namespace app
