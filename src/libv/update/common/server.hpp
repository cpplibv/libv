// Project: libv.update, File: src/libv/update/common/server.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/io_context.hpp>
//#include <libv/net/mtcp/acceptor.hpp>
#include <libv/net/mtcp/endpoint.hpp>
// std
#include <memory>
#include <mutex>
#include <vector>
#include <string>


namespace app {

// -------------------------------------------------------------------------------------------------

class UpdateServer {
	std::vector<std::unique_ptr<class UpdateSession>> clients;
//	std::vector<libv::net::mtcp::ConnectionAsyncCB> client_queue;
	std::mutex clients_m;

	libv::net::IOContext io_context;
//	libv::net::mtcp::AcceptorAsyncCB acceptor;

public:
	UpdateServer(libv::net::mtcp::Endpoint endpoint, uint16_t num_net_thread);
	UpdateServer(uint16_t port, uint16_t num_net_thread);
	~UpdateServer();

public:
	void remove_session(class UpdateSession*);
	void broadcast(const std::string& message);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
