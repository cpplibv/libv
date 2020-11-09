// Project: libv.net, File: sandbox/libv_net_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/log/log.hpp>
//#include <libv/sml.hpp>
//#include <libv/mt/binary_latch.hpp>
#include <libv/utility/hex_dump.hpp>
//#include <libv/utility/timer.hpp>
// std
#include <iostream>
//#include <atomic>
// pro
#include <libv/net/address.hpp>
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/acceptor.hpp>
#include <libv/net/mtcp/connection.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

struct Config {
	uint16_t port = 25446;
	size_t num_client = 3;
	size_t num_client_io_thread = 2;
	size_t num_server_io_thread = 2;
	std::string request = "request";
	std::string response = "response";
} const config;

// -------------------------------------------------------------------------------------------------

struct Session {
	static inline int nextID = 0;
	std::string id = fmt::format("Session-{}", nextID++);
	libv::net::mtcp::ConnectionAsyncCB conn;

	Session(libv::net::mtcp::ConnectionAsyncCB&& conn_) :
		conn(std::move(conn_)) {
		conn.handle_connect([this](libv::net::mtcp::Endpoint endpoint) {
			log_sandbox.info("{} connected to: {}", id, endpoint);
		});
		conn.handle_disconnect([this]() {
			log_sandbox.info("{} disconnected", id);
		});
		conn.handle_error([this](auto operation, std::error_code ec) {
			log_sandbox.error("{} error: {}", id, libv::net::to_string(ec));
			if (ec == boost::asio::error::make_error_code(boost::asio::error::eof))
				return;
			if (ec == boost::asio::error::make_error_code(boost::asio::error::operation_aborted))
				return;
		});
		conn.handle_send([this](libv::net::mtcp::Message&& response) {
			(void) response;
			log_sandbox.info("{} sent {}", id, response);
		});
		conn.handle_receive([this](libv::net::mtcp::Message&& request) {
			log_sandbox.info("{} received: {}", id, request);
			conn.send(config.response);
			conn.receive();
		});
	}

	void run() {
		conn.start();
		conn.receive();
	}
};

struct Server {
	static inline int nextID = 0;
	std::string id = fmt::format("Server-{}", nextID++);
	libv::net::mtcp::AcceptorAsyncCB acceptor;
	std::vector<std::shared_ptr<Session>> sessions;

	Server(libv::net::IOContext& io_context) :
		acceptor(io_context) {

		acceptor.handle_accept([this](libv::net::mtcp::ConnectionAsyncCB conn) {

			log_sandbox.info("{} accepted", id);
			sessions.emplace_back(std::make_shared<Session>(std::move(conn)))->run();

			if (sessions.size() >= config.num_client)
				acceptor.cancel();
		});

		acceptor.handle_error([this](auto operation, std::error_code ec) {
			log_sandbox.error("{} error: {}", id, libv::net::to_string(ec));
		});
	}

	void run() {
		log_sandbox.info("{} run", id);
		acceptor.listen(config.port);
		acceptor.accept();
	}
};

// -------------------------------------------------------------------------------------------------

struct Client {
	static inline int nextID = 0;
	std::string id = fmt::format("Client-{}", nextID++);
	libv::net::mtcp::ConnectionAsyncCB conn;

	Client(libv::net::IOContext& io_context) : conn(io_context) {
		conn.handle_connect([this](libv::net::mtcp::Endpoint endpoint) {
			log_sandbox.info("{} connected to: {}", id, endpoint);
		});
		conn.handle_disconnect([this]() {
			log_sandbox.info("{} disconnected", id);
		});
		conn.handle_error([this](auto operation, std::error_code ec) {
			if (ec == boost::asio::error::make_error_code(boost::asio::error::eof))
				return;
			if (ec == boost::asio::error::make_error_code(boost::asio::error::operation_aborted))
				return;
			log_sandbox.error("{} error: {}", id, libv::net::to_string(ec));
		});
		conn.handle_send([this](libv::net::mtcp::Message&& request) {
			(void) request;
			log_sandbox.info("{} sent", id);
		});
		conn.handle_receive([this](libv::net::mtcp::Message&& request) {
			(void) request;
			log_sandbox.info("{} received: {}", id, config.response);
		});
	}

	void run() {
		log_sandbox.info("{} run", id);
		conn.connect(libv::net::Address{"localhost", config.port});
//		conn.connect("localhost", config.port); // TODO P5: more connect overload
		conn.send(config.request);
		conn.receive();
		conn.disconnect();
	}
};

// -------------------------------------------------------------------------------------------------

void main_server() {
	auto io_context = libv::net::IOContext{config.num_server_io_thread};
	auto server = Server{io_context};
	server.run();
	io_context.join();
}

void main_client() {
	auto io_context = libv::net::IOContext{config.num_client_io_thread};
	auto client = Client{io_context};
	client.run();
	io_context.join();
}

int main(int, char**) {
	libv::logger_stream.setFormat("{severity} {thread_id:2} {module:8}: {message}\n");
	std::cout << libv::logger_stream;
	std::vector<std::thread> threads;

	threads.emplace_back(main_server);
	for (size_t i = 0; i < config.num_client; ++i)
		threads.emplace_back(main_client);

	for (auto& thread : threads)
		thread.join();

	return 0;
}
