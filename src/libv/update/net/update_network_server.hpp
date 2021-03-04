// Project: libv.net, File: src/libv/update/net/updater_network_server.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/fwd.hpp>
//#include <libv/net/address.hpp>
//#include <libv/net/io_context.hpp>
//#include <libv/net/mtcp/endpoint.hpp>
//#include <libv/net/mtcp/connection_cb.hpp>
// std
//#include <string>
#include <memory>
//#include <mutex>
//#include <vector>
// pro
//#include <libv/update/server/update_check_result.hpp>
//#include <libv/update/net/protocol.hpp>
//#include <libv/update/version_number.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class UpdateNetworkServer {
private:
	libv::net::IOContext& io_context;
	std::shared_ptr<void> acceptor; // <<< ugly

//	libv::net::Address server_address;
//	libv::net::mtcp::ConnectionAsyncCB connection{io_context};
//
//	msg::UpdateInfo update_info_;
//	std::vector<UpdateInfo> update_infos_;

public:
	explicit UpdateNetworkServer(libv::net::IOContext& ioContext);
	~UpdateNetworkServer();

public:
	void listen(libv::net::mtcp::Endpoint endpoint, int backlog);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
