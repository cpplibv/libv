// Project: libv, File: app/fsw.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/log/log.hpp>
#include <libv/net/address.hpp>
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/acceptor.hpp>
#include <libv/net/mtcp/connection.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
// std
//#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>


// -------------------------------------------------------------------------------------------------

struct Manifest {
	uint64_t manifest_version;
	std::string manifest_name;

	uint64_t version;

	struct Entry {
		std::string name;
		uint64_t version;
	};

	std::vector<Entry> entries;
};

struct UpdateSession {
	libv::net::mtcp::ConnectionAsnycCB connection;

public:
	explicit UpdateSession(libv::net::mtcp::ConnectionAsnycCB connection_) :
		connection(std::move(connection_)) {

		const auto connect_cb = [](auto local_endpoint, auto remote_endpoint) {
		};

		const auto disconnect_cb = []() {
		};

		const auto error_cb = [](auto operation, std::error_code ec) noexcept {
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

	~UpdateSession() {
		connection.receive_stop();
		connection.disconnect();
	}
};

struct UpdateServer {
	libv::net::IOContext io_context;
	libv::net::mtcp::AcceptorAsyncCB acceptor;

	std::vector<UpdateSession> clients;
//	std::vector<libv::net::mtcp::ConnectionAsnycCB> client_queue;
	std::mutex clients_m;

	UpdateServer(uint16_t port, uint16_t num_net_thread) :
		io_context(num_net_thread),
		acceptor(io_context) {

		const auto accept_backlog_size = 4;

		const auto accept_cb = [this](libv::net::mtcp::ConnectionAsnycCB connection) noexcept {
			const auto lock = std::unique_lock(clients_m);
			clients.emplace_back(std::move(connection));
		};

		const auto error_cb = [this](auto operation, std::error_code ec) noexcept {
			// log
		};

		acceptor.handle_accept(accept_cb);
		acceptor.handle_error(error_cb);
		acceptor.listen(port, accept_backlog_size);
		acceptor.accept();
	}

	~UpdateServer() {
	}
};

// -------------------------------------------------------------------------------------------------

struct UpdateClient {
	libv::net::IOContext io_context{1};
	libv::net::mtcp::ConnectionAsnycCB connection{io_context};

public:
	explicit UpdateClient() {
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

		connection.connect(*libv::net::Address::parse("localhost:25090"));
//		connection.receive(1);
		connection.receive_repeat();
	}

	void send(std::string m) {
		connection.send(m);
	}

	~UpdateClient() {
		connection.disconnect();
		io_context.join();
	}
};

// -------------------------------------------------------------------------------------------------

static constexpr uint16_t default_port = 25090;
static constexpr uint16_t default_net_thread = 4;

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Updater server", "LIBV Updater server daemon");

	const auto config_file = args.require<std::string>
			("-c", "--config")
			("config_file", "File path of the config file");

	const auto manifest_file = args.require<std::string>
			("-m", "--manifest")
			("manifest_file", "File path of the manifest file");

	const auto address = args.require<std::string>
			("-a", "--address")
			("address", "Listening IP address")
			= "0.0.0.0";

	const auto port = args.require<uint16_t>
			("-p", "--port")
			("port", "Listening TCP port")
			= default_port;

	const auto num_net_thread = args.require<uint16_t>
			("-t", "--net_thread")
			("net_thread", "Number of network IO threads")
			= default_net_thread;

	const auto server_mode = args.flag
			("-s", "--server")
			("server", "Start in server mode");

	const auto max_active_client = 100;
	const auto max_queue_client = 10000;
	const auto max_upload_bps = 10000;
	const auto max_upload_bps_per_client = 10000;

	args.require_no_unused();

	if (!args.parse(argc, argv)) {
		std::cerr << args.error_message(100) << args.usage(100);
		return EXIT_FAILURE;
	}

	std::cout << args.report(100);

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	if (server_mode.value()) {
		UpdateServer server = {port.value(), num_net_thread.value()};
		while (true)
			std::this_thread::sleep_for(std::chrono::seconds(1));
	} else {
		UpdateClient client;
		for (std::string line; std::getline(std::cin, line);) {
			client.send(line);
		}
	}

	return EXIT_SUCCESS;
}

