// Project: libv, File: app/space/network/network_client.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// std
#include <memory>
#include <string>
#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

class ImplNetworkClient;

class NetworkClient {
private:
	std::unique_ptr<ImplNetworkClient> self;

public:
	NetworkClient(std::string server_address, uint16_t server_port, std::string name, Playout& playout);
	~NetworkClient();

public:
	void send(std::vector<std::byte> message);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
