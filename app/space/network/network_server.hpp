// Project: libv, File: app/space/network/network_server.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/utility/nexus_fwd.hpp>
// std
#include <memory>
#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

//class NetworkControlMPServer {
//	/// Sends message to everyone connect to the server
//	void broadcast(std::string message);
//};
//
//class NetworkControlMPClient {
//	/// Sends message to the server to be broadcasted to everyone connected
//	void broadcast(std::string message);
//};
//
//class NetworkControlSP {
//	/// Noop
//	void broadcast(std::string message);
//};
//
//// -------------------------------------------------------------------------------------------------
//
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

//struct SomethingCodec {
//
//	template <typename T>
//	void send(const T& message);
//
//	template <typename T>
//	T receive();
//};

// -------------------------------------------------------------------------------------------------

class ImplNetworkServer;

class NetworkServer {
	std::unique_ptr<ImplNetworkServer> self;

public:
	NetworkServer(uint16_t server_port, Playout& playout);
	~NetworkServer();

public:
	void broadcast(std::vector<std::byte> message);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
