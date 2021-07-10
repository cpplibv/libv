// Project: libv, File: app/space/network_server.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <memory>


namespace app {

// -------------------------------------------------------------------------------------------------

class PlayoutDelayBuffer;
class ImplNetworkServer;

class NetworkServer {
	std::unique_ptr<ImplNetworkServer> self;

public:
	NetworkServer(uint16_t server_port, PlayoutDelayBuffer& pdb);
	~NetworkServer();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
