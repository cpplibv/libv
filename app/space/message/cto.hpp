//
// Generated source code
// Generator version: v5.0.0
// Input file: app/space/message/cto.in.lua

#pragma once

//
#include <chrono>
#include <cstdint>
#include <libv/math/vec.hpp>
#include <libv/serial/archive/binary_fwd.hpp>
#include <libv/serial/codec_message_id.hpp>
#include <space/sim/ids.hpp>
#include <string>
#include <vector>


namespace space {

// -------------------------------------------------------------------------------------------------

struct CTO_Introduction {
	static constexpr libv::serial::CodecMessageID id{0};

	std::string user_name;
	uint64_t version;

	CTO_Introduction() = default;
	explicit inline CTO_Introduction(std::string user_name, uint64_t version) : user_name(std::move(user_name)), version(version) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_ClientJoined {
	static constexpr libv::serial::CodecMessageID id{20};

	UserID userID = invalidUserID;
	std::string user_name;
	std::chrono::system_clock::time_point joined_at;
	uint64_t version;

	CTO_ClientJoined() = default;
	explicit inline CTO_ClientJoined(UserID userID, std::string user_name, std::chrono::system_clock::time_point joined_at, uint64_t version) : userID(userID), user_name(std::move(user_name)), joined_at(joined_at), version(version) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_ClientLeave {
	static constexpr libv::serial::CodecMessageID id{21};

	UserID userID = invalidUserID;

	CTO_ClientLeave() = default;
	explicit inline CTO_ClientLeave(UserID userID) : userID(userID) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_ChatMessage {
	static constexpr libv::serial::CodecMessageID id{23};

	UserID userID = invalidUserID;
	std::chrono::system_clock::time_point sent_at;
	std::string message;

	CTO_ChatMessage() = default;
	explicit inline CTO_ChatMessage(UserID userID, std::chrono::system_clock::time_point sent_at, std::string message) : userID(userID), sent_at(sent_at), message(std::move(message)) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_LobbyStatus {
	struct Entry {
		UserID userID = invalidUserID;
		float ping;

		Entry() = default;
		explicit inline Entry(UserID userID, float ping) : userID(userID), ping(ping) {}
		void save(libv::archive::BinaryOutput& ar) const;
		void load(libv::archive::BinaryInput& ar);
	};

	static constexpr libv::serial::CodecMessageID id{24};

	std::vector<Entry> pings;

	CTO_LobbyStatus() = default;
	explicit inline CTO_LobbyStatus(std::vector<Entry> pings) : pings(std::move(pings)) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_LobbyClose {
	static constexpr libv::serial::CodecMessageID id{25};

	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetSpawn {
	static constexpr libv::serial::CodecMessageID id{30};

	libv::vec3f position;

	CTO_FleetSpawn() = default;
	explicit inline CTO_FleetSpawn(libv::vec3f position) : position(position) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetSelect {
	static constexpr libv::serial::CodecMessageID id{31};

	FleetID fleetID = invalidFleetID;

	CTO_FleetSelect() = default;
	explicit inline CTO_FleetSelect(FleetID fleetID) : fleetID(fleetID) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetSelectAdd {
	static constexpr libv::serial::CodecMessageID id{32};

	FleetID fleetID = invalidFleetID;

	CTO_FleetSelectAdd() = default;
	explicit inline CTO_FleetSelectAdd(FleetID fleetID) : fleetID(fleetID) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetClearSelection {
	static constexpr libv::serial::CodecMessageID id{33};

	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetSelectBox {
	static constexpr libv::serial::CodecMessageID id{34};

	std::vector<FleetID> fleetIDs;

	CTO_FleetSelectBox() = default;
	explicit inline CTO_FleetSelectBox(std::vector<FleetID> fleetIDs) : fleetIDs(std::move(fleetIDs)) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetMove {
	static constexpr libv::serial::CodecMessageID id{35};

	libv::vec3f target_position;

	CTO_FleetMove() = default;
	explicit inline CTO_FleetMove(libv::vec3f target_position) : target_position(target_position) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetMoveQueue {
	static constexpr libv::serial::CodecMessageID id{36};

	libv::vec3f target_position;

	CTO_FleetMoveQueue() = default;
	explicit inline CTO_FleetMoveQueue(libv::vec3f target_position) : target_position(target_position) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetAttackFleet {
	static constexpr libv::serial::CodecMessageID id{37};

	FleetID targetFleetID = invalidFleetID;

	CTO_FleetAttackFleet() = default;
	explicit inline CTO_FleetAttackFleet(FleetID targetFleetID) : targetFleetID(targetFleetID) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetAttackFleetQueue {
	static constexpr libv::serial::CodecMessageID id{38};

	FleetID targetFleetID = invalidFleetID;

	CTO_FleetAttackFleetQueue() = default;
	explicit inline CTO_FleetAttackFleetQueue(FleetID targetFleetID) : targetFleetID(targetFleetID) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetAttackPlanet {
	static constexpr libv::serial::CodecMessageID id{39};

	PlanetID targetPlanetID = invalidPlanetID;

	CTO_FleetAttackPlanet() = default;
	explicit inline CTO_FleetAttackPlanet(PlanetID targetPlanetID) : targetPlanetID(targetPlanetID) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_FleetAttackPlanetQueue {
	static constexpr libv::serial::CodecMessageID id{40};

	PlanetID targetPlanetID = invalidPlanetID;

	CTO_FleetAttackPlanetQueue() = default;
	explicit inline CTO_FleetAttackPlanetQueue(PlanetID targetPlanetID) : targetPlanetID(targetPlanetID) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_ClearFleets {
	static constexpr libv::serial::CodecMessageID id{41};

	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_Shuffle {
	static constexpr libv::serial::CodecMessageID id{42};

	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_PlanetSpawn {
	static constexpr libv::serial::CodecMessageID id{43};

	libv::vec3f position;

	CTO_PlanetSpawn() = default;
	explicit inline CTO_PlanetSpawn(libv::vec3f position) : position(position) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_ClearPlanets {
	static constexpr libv::serial::CodecMessageID id{44};

	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_TrackView {
	static constexpr libv::serial::CodecMessageID id{50};

	UserID userID = invalidUserID;
	libv::vec3f eye;
	libv::vec3f target;
	libv::vec3f mouse_direction;

	CTO_TrackView() = default;
	explicit inline CTO_TrackView(UserID userID, libv::vec3f eye, libv::vec3f target, libv::vec3f mouse_direction) : userID(userID), eye(eye), target(target), mouse_direction(mouse_direction) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

struct CTO_CameraWarpTo {
	static constexpr libv::serial::CodecMessageID id{51};

	UserID userID = invalidUserID;
	libv::vec3f target_position;

	CTO_CameraWarpTo() = default;
	explicit inline CTO_CameraWarpTo(UserID userID, libv::vec3f target_position) : userID(userID), target_position(target_position) {}
	void save(libv::archive::BinaryOutput& ar) const;
	void load(libv::archive::BinaryInput& ar);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
