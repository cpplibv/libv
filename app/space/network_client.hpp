// Project: libv, File: app/space/network_client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <memory>
#include <string>


namespace app {

// -------------------------------------------------------------------------------------------------

class PlayoutDelayBuffer;
class ImplNetworkClient;

class NetworkClient {
private:
	std::unique_ptr<ImplNetworkClient> self;

public:
	NetworkClient(std::string server_address, uint16_t server_port, std::string name, PlayoutDelayBuffer& pdb);
	~NetworkClient();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
