// Project: libv, File: app/space/network_server.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// std
#include <memory>


namespace app {

// -------------------------------------------------------------------------------------------------

class ImplNetworkServer;

class NetworkServer {
	std::unique_ptr<ImplNetworkServer> self;

public:
	NetworkServer(uint16_t server_port, Playout& playout);
	~NetworkServer();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
