// Project: libv, File: app/update/common/client.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <update/common/client.hpp>
// libv
#include <libv/net/error.hpp>
// std
#include <iostream>
// pro
//#include <update/common/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

UpdateClient::UpdateClient() {
	const auto connect_cb = [this](auto local_endpoint, auto remote_endpoint) {
		std::cout << "connect_cb: " << local_endpoint << " - " << remote_endpoint << std::endl;
	};

	const auto disconnect_cb = [this]() {
		std::cout << "disconnect_cb" << std::endl;
	};

	const auto error_cb = [this](auto operation, std::error_code ec) noexcept {
		std::cout << "error_cb: " << libv::net::to_string(ec) << std::endl;
	};

	const auto receive_cb = [this](libv::net::mtcp::Message&& message) noexcept {
		std::cout << "receive_cb: " << message << std::endl;
		std::cout << "num_byte_read       : " << connection.num_byte_read() << std::endl;
		std::cout << "num_byte_wrote      : " << connection.num_byte_wrote() << std::endl;
		std::cout << "num_message_received: " << connection.num_message_received() << std::endl;
		std::cout << "num_message_sent    : " << connection.num_message_sent() << std::endl;
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
	connection.disconnect_teardown();
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
