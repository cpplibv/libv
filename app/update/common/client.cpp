// Project: libv, File: app/update/common/client.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <update/common/client.hpp>
// libv
//#include <libv/net/error.hpp>
// std
#include <iostream>
// pro
//#include <update/common/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

UpdateClient::UpdateClient() {
	const auto connect_cb = [](auto local_endpoint, auto remote_endpoint) {
	};

	const auto disconnect_cb = []() {
	};

	const auto error_cb = [](auto operation, std::error_code ec) noexcept {
		(void) operation;
	};

	const auto receive_cb = [this](libv::net::mtcp::Message&& message) noexcept {
		std::cout << "receive_cb: " << message << std::endl;
		std::cout << "total_read_bytes    : " << connection.total_read_bytes() << std::endl;
		std::cout << "total_write_bytes   : " << connection.total_write_bytes() << std::endl;
		std::cout << "total_read_messages : " << connection.total_read_messages() << std::endl;
		std::cout << "total_write_messages: " << connection.total_write_messages() << std::endl;
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
}

UpdateClient::~UpdateClient() {
	connection.disconnect();
	io_context.join();
}

void UpdateClient::update_from(libv::net::Address server_address) {
	connection.connect(server_address);
//	connection.receive(1);
	connection.receive_repeat();
}

void UpdateClient::send(std::string m) {
	connection.send(m);
}

// -------------------------------------------------------------------------------------------------

} // namespace
