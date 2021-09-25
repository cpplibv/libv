// Project: libv, File: app/space/network_client.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/network/network_client.hpp>
// libv
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/connection_he.hpp>
// pro
#include <space/log.hpp>
#include <space/playout.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] inline std::string_view as_sv(const std::span<const std::byte> s) noexcept {
	return {reinterpret_cast<const char*>(s.data()), s.size()};
}

// -------------------------------------------------------------------------------------------------

class ConnectionHandler : public libv::net::mtcp::ConnectionHandler<ConnectionHandler> {
private:
	std::string name;
	Playout& playout; // !!! ConnectionHandler does not keeps alive the playout, message right after closing client could segfault

public:
	inline explicit ConnectionHandler(std::string_view server_address, uint16_t server_port, std::string name, Playout& playout) :
		name(std::move(name)),
		playout(playout) {
		connection.connect_async(libv::net::Address(server_address, server_port));
	}

	~ConnectionHandler() {
	}

public:
	void write(const std::vector<std::byte>& message) {
		connection.send_async(message);
//		connection.send_async(name + ": " + message);
	}

private:
	virtual void on_connect(error_code ec) override {
		if (!ec) {
//			write(name);
			log_space.trace("[{}] on_connect", name);
		} else
			log_space.error("[{}] on_connect {}", name, libv::net::to_string(ec));
	}

	virtual void on_receive(error_code ec, message m) override {
		if (!ec) {
			log_space.trace("[{}] on_receive {}", name, as_sv(m));
			playout.queue_from_network(as_sv(m));
		} else
			log_space.error("[{}] on_receive {}", name, libv::net::to_string(ec));
	}

	virtual void on_send(error_code ec, message m) override {
		if (!ec)
			log_space.trace("[{}] on_send {}", name, as_sv(m));
		else
			log_space.error("[{}] on_send {}", name, libv::net::to_string(ec));
	}

	virtual void on_disconnect(error_code ec) override {
		if (ec)
			log_space.trace("[{}] on_disconnect", name);
		else
			log_space.error("[{}] on_disconnect {}", name, libv::net::to_string(ec));
	}
};
using Connection = libv::net::mtcp::Connection<ConnectionHandler>;

// =================================================================================================

struct ImplNetworkClient {
	libv::net::IOContext io_context{2};
	Connection connection;

	ImplNetworkClient(std::string server_address, uint16_t server_port, std::string name, Playout& playout) :
		connection(io_context, std::move(server_address), server_port, std::move(name), playout) {}
};

// =================================================================================================

NetworkClient::NetworkClient(std::string server_address, uint16_t server_port, std::string name, Playout& playout) :
	self(std::make_unique<ImplNetworkClient>(std::move(server_address), server_port, std::move(name), playout)) {
}

NetworkClient::~NetworkClient() {
	// For the sake of forward declared unique_ptr
}

// -------------------------------------------------------------------------------------------------

void NetworkClient::send(std::vector<std::byte> message) {
	self->connection->write(message);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
