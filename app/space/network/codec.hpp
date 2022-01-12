// Project: libv, File: app/space/network/codec.hpp

#pragma once

// libv
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/codec3.hpp>
// std
#include <span>
#include <vector>
// pro
#include <space/cto.hpp>
#include <space/network/lobby.hpp> // TODO P2: Find a better place for SnapshotLobby
#include <space/sim/engine/snapshot_archive.hpp>
#include <space/sim/universe.hpp> // TODO P2: Find a better place for SnapshotUniverse
#include <space/sim/universe_serial.hpp> // TODO P2: Find a better place for SnapshotUniverse
//#include <space/network/nto.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct ProtocolViolation : std::exception {
	[[nodiscard]] virtual const char* what() const noexcept override {
		return "Protocol Violation";
	}
};

// -------------------------------------------------------------------------------------------------

struct Codec : libv::serial::Codec3<Codec> {
	template <typename Ar, typename F> struct gen_table {
		static constexpr TypeAccessEntry types[] = {
				create_entry<Ar, F, CTO_Introduction>(),

				create_entry<Ar, F, SnapshotLobby>(),

				create_entry<Ar, F, SnapshotUniverse>(),

				create_entry<Ar, F, CTO_ChatMessage>(),
				create_entry<Ar, F, CTO_ClientJoined>(),
				create_entry<Ar, F, CTO_ClientLeave>(),
				create_entry<Ar, F, CTO_LobbyStatus>(),
//				create_entry<Ar, F, CTO_LobbyClose>(),

				create_entry<Ar, F, CTO_FleetSpawn>(),
				create_entry<Ar, F, CTO_FleetSelect>(),
				create_entry<Ar, F, CTO_FleetSelectAdd>(),
				create_entry<Ar, F, CTO_FleetSelectBox>(),
				create_entry<Ar, F, CTO_FleetClearSelection>(),
				create_entry<Ar, F, CTO_FleetMove>(),
				create_entry<Ar, F, CTO_FleetMoveQueue>(),
				create_entry<Ar, F, CTO_FleetAttackFleet>(),
				create_entry<Ar, F, CTO_FleetAttackFleetQueue>(),
				create_entry<Ar, F, CTO_FleetAttackPlanet>(),
				create_entry<Ar, F, CTO_FleetAttackPlanetQueue>(),
				create_entry<Ar, F, CTO_ClearFleets>(),
				create_entry<Ar, F, CTO_Shuffle>(),
				create_entry<Ar, F, CTO_PlanetSpawn>(),
				create_entry<Ar, F, CTO_ClearPlanets>(),

				create_entry<Ar, F, CTO_TrackView>(),
				create_entry<Ar, F, CTO_CameraWarpTo>(),
				//	create_entry<Ar, F, CTO_CameraMovement>(),
				//	create_entry<Ar, F, CTO_MouseMovement>(),

				//	create_entry<Ar, F, CTO_Snapshot>(),
		};
	};
};

// -------------------------------------------------------------------------------------------------

static inline Codec network_codec;

template <typename T>
std::vector<std::byte> network_encode(const T& message) {
	std::vector<std::byte> result;
	{
		SnapshotArchive<libv::archive::BasicBinaryOutput> oar{false, result};
		network_codec.encode(oar, message);
	}
	return result;
}

template <typename F>
void network_decode(std::span<const std::byte> message, F& handler) {
	{
		SnapshotArchive<libv::archive::BasicBinaryInput> iar{false, message};
		network_codec.decode(iar, handler);
	}
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string debug_binary_as_json(std::span<const std::byte> message);

// -------------------------------------------------------------------------------------------------

} // namespace space
