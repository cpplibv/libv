// Project: libv, File: app/update/common/server.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <update/common/server.hpp>
// libv
#include <libv/algorithm/linear_find.hpp>
#include <libv/algorithm/erase_unstable.hpp>
#include <libv/net/mtcp/connection.hpp>
//#include <libv/net/error.hpp>
// pro
#include <update/common/session.hpp>
//#include <update/common/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

UpdateServer::UpdateServer(uint16_t port, uint16_t num_net_thread) :
		io_context(num_net_thread),
		acceptor(io_context) {

	const auto accept_backlog_size = 4;

	const auto accept_cb = [this](libv::net::mtcp::ConnectionAsnycCB connection) noexcept {
		const auto lock = std::unique_lock(clients_m);
		clients.push_back(std::make_unique<UpdateSession>(*this, std::move(connection)));
	};

	const auto error_cb = [this](auto operation, std::error_code ec) noexcept {
		// log
	};

	acceptor.handle_accept(accept_cb);
	acceptor.handle_error(error_cb);
	acceptor.listen(port, accept_backlog_size);
	acceptor.accept();
}

UpdateServer::~UpdateServer() {
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

// -------------------------------------------------------------------------------------------------

} // namespace app
