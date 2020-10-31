// Project: libv, File: app/update/common/session.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <update/common/session.hpp>
// libv
#include <libv/utility/read_file.hpp>
//#include <libv/utility/write_file.hpp>
#include <libv/net/error.hpp>
// std
#include <iostream>
// pro
#include <update/common/server.hpp>
//#include <update/common/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

UpdateSession::UpdateSession(UpdateServer& server_, libv::net::mtcp::ConnectionAsnycCB connection_) :
		connection(std::move(connection_)),
		server(server_) {

	const auto connect_cb = [this](auto local_endpoint, auto remote_endpoint) {
		std::cout << "connect_cb: " << local_endpoint << " - " << remote_endpoint << std::endl;
	};

	const auto disconnect_cb = [this]() {
		std::cout << "disconnect_cb" << std::endl;

		server.remove_session(this);
	};

	const auto error_cb = [this](auto operation, std::error_code ec) noexcept {
		std::cout << "error_cb: " << libv::net::to_string(ec) << std::endl;
	};

	const auto receive_cb = [this](libv::net::mtcp::Message&& message) noexcept {
		std::cout << "receive_cb: " << message << std::endl;

		connection.send(libv::read_file_or(message, ""));
	};

	const auto send_cb = [](libv::net::mtcp::Message&& message) noexcept {
		std::cout << "send_cb: " << message << std::endl;
	};

	connection.handle_connect(connect_cb);
	connection.handle_disconnect(disconnect_cb);
	connection.handle_error(error_cb);
	connection.handle_receive(receive_cb);
	connection.handle_send(send_cb);

	connection.read_limit(100); // bytes per second
	connection.write_limit(100); // bytes per second

	connection.start();
	connection.receive_repeat();
}

UpdateSession::~UpdateSession() {
	connection.disconnect_teardown();
}

void UpdateSession::send(const std::string& message) {
//	std::cout << "send: " << message << std::endl;

	connection.send(message);
}

void UpdateSession::kick() {
//	std::cout << "kick" << std::endl;

	connection.disconnect_teardown();
}

// -------------------------------------------------------------------------------------------------

} // namespace app
