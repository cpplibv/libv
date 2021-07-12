// Project: libv, File: app/space/network_client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// std
#include <memory>
#include <string>


namespace app {

// -------------------------------------------------------------------------------------------------

class ImplNetworkClient;

class NetworkClient {
private:
	std::unique_ptr<ImplNetworkClient> self;

public:
	NetworkClient(std::string server_address, uint16_t server_port, std::string name, Playout& playout);
	~NetworkClient();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
