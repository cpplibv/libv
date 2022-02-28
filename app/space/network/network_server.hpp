// Project: libv, File: app/space/network/network_server.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/utility/nexus_fwd.hpp>
// std
#include <memory>
#include <vector>


namespace space {

// -------------------------------------------------------------------------------------------------

//namespace mn {
//
//struct ClientStatus {
//	enum class Status {
////		connecting,
////		catching_up,
////		desynchronized,
//		connected,
//		disconnected,
//	};
//
//	int64_t id;
//	std::string name;
//	Status status;
//};
//
//struct ServerStatus {
//	enum class Status {
////		connecting,
//		connected,
//		disconnected,
//	};
//
//	Status status;
//};
//
//} // namespace mn

// -------------------------------------------------------------------------------------------------

class ImplNetworkServer;

class NetworkServer {
	std::unique_ptr<ImplNetworkServer> self;

public:
//	NetworkServer(uint16_t server_port, GameThread& game_thread, Playout& playout, Universe& universe, User& user);
	NetworkServer(uint16_t server_port, GameThread& game_thread, Playout& playout, User& user);
	~NetworkServer();

public:
	void broadcast(std::vector<std::byte> message);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
