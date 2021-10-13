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
//#include <space/network/nto.hpp>
#include <space/network/lobby.hpp> // TODO P2: Find a better place for SnapshotLobby


namespace app {

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

				create_entry<Ar, F, CTO_ChatMessage>(),
				create_entry<Ar, F, CTO_ClientJoined>(),
				create_entry<Ar, F, CTO_ClientLeave>(),
				create_entry<Ar, F, CTO_LobbyStatus>(),
//				create_entry<Ar, F, CTO_LobbyClose>(),

				create_entry<Ar, F, CTO_FleetSpawn>(),
				create_entry<Ar, F, CTO_FleetSelect>(),
				create_entry<Ar, F, CTO_FleetSelectAdd>(),
				create_entry<Ar, F, CTO_FleetBoxSelect>(),
				create_entry<Ar, F, CTO_FleetMove>(),
				create_entry<Ar, F, CTO_FleetQueueMove>(),
				create_entry<Ar, F, CTO_ClearFleets>(),
				create_entry<Ar, F, CTO_Shuffle>(),

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
		libv::archive::Binary::output oar(result);
		network_codec.encode(oar, message);
	}
	return result;
}

template <typename F>
void network_decode(std::span<const std::byte> message, F& handler) {
	{
		libv::archive::Binary::input iar(message);
		network_codec.decode(iar, handler);
	}
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string debug_binary_as_json(std::span<const std::byte> message);

// -------------------------------------------------------------------------------------------------

} // namespace app
