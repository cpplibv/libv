// Project: libv.net, File: src/libv/update/net/updater_network_server.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/fwd.hpp>
// std
#include <memory>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class UpdateNetworkServer {
private:
	std::unique_ptr<class ImplUpdateNetworkServer> self;

public:
	explicit UpdateNetworkServer(libv::net::IOContext& io_context, std::shared_ptr<class UpdateInfoDictionary> dictionary);
	~UpdateNetworkServer();

public:
	void listen(libv::net::mtcp::Endpoint endpoint, int backlog);
	void start();
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
