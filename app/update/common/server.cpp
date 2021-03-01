// Project: libv, File: app/update/common/server.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <update/common/server.hpp>
// libv
#include <libv/algo/linear_find.hpp>
#include <libv/algo/erase_unstable.hpp>
#include <libv/net/mtcp/connection_cb.hpp>
//#include <libv/net/error.hpp>
// pro
#include <update/common/session.hpp>
//#include <update/common/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

UpdateServer::UpdateServer(libv::net::mtcp::Endpoint endpoint, uint16_t num_net_thread) :
		io_context(num_net_thread),
		acceptor(io_context) {

	const auto accept_backlog_size = 4;

	const auto accept_cb = [this](libv::net::mtcp::ConnectionAsyncCB connection) noexcept {
		const auto lock = std::unique_lock(clients_m);
		clients.push_back(std::make_unique<UpdateSession>(*this, std::move(connection)));
	};

	const auto error_cb = [this](auto operation, std::error_code ec) noexcept {
		// log
	};

	acceptor.handle_accept(accept_cb);
	acceptor.handle_error(error_cb);
	acceptor.listen(endpoint, accept_backlog_size);
	acceptor.accept();
}

UpdateServer::UpdateServer(uint16_t port, uint16_t num_net_thread) :
	UpdateServer(libv::net::mtcp::Endpoint(port), num_net_thread) { }

UpdateServer::~UpdateServer() {
	{
		const auto lock = std::unique_lock(clients_m);

		acceptor.cancel();

		for (const auto& client : clients)
			client->kick();
	}

	io_context.join();
}

void UpdateServer::remove_session(class UpdateSession* session) {
	const auto lock = std::unique_lock(clients_m);

	auto it = libv::linear_find_if_iterator(clients, [&](auto& up) { return up.get() == session; });

	if (it == clients.end()) {
		assert(false && "Internal error: Attempting to remove a not contained session.");
		return;
	}

	libv::erase_unstable(clients, it);
}

void UpdateServer::broadcast(const std::string& message) {
	const auto lock = std::unique_lock(clients_m);

	for (const auto& client : clients)
		client->send(message);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
