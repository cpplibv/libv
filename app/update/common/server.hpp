// Project: libv, File: app/update/common/server.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/acceptor.hpp>
// std
#include <memory>
#include <mutex>
#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

class UpdateServer {
	libv::net::IOContext io_context;
	libv::net::mtcp::AcceptorAsyncCB acceptor;

	std::vector<std::unique_ptr<class UpdateSession>> clients;
//	std::vector<libv::net::mtcp::ConnectionAsnycCB> client_queue;
	std::mutex clients_m;

public:
	UpdateServer(uint16_t port, uint16_t num_net_thread);
	~UpdateServer();

public:
	void remove_session(class UpdateSession*);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
