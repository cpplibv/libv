// File: libv_net_main.cpp, Created on 2018. 05. 07. 11:59, Author: Vader

// libv
#include <libv/log/log.hpp>
//#include <libv/sml.hpp>
//#include <libv/thread/binary_latch.hpp>
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

inline libv::LoggerModule log_sandbox{libv::logger, "sandbox"};

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
	inline static int nextID = 0;
	std::string id = fmt::format("Session-{}", nextID++);
	libv::net::mtcp::ConnectionAsnycCB conn;

	Session(libv::net::mtcp::ConnectionAsnycCB&& conn_) :
		conn(std::move(conn_)) {
		conn.handle_connect([this](const libv::net::mtcp::Endpoint endpoint) {
			log_sandbox.info("{} connected to: {}", id, endpoint);
		});
		conn.handle_disconnect([this]() {
			log_sandbox.info("{} disconnected", id);
		});
		conn.handle_error([this](const std::error_code ec) {
			log_sandbox.error("{} error: {}", id, libv::net::to_string(ec));
			if (ec == netts::error::make_error_code(netts::error::eof))
				return;
			if (ec == netts::error::make_error_code(netts::error::operation_aborted))
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
	inline static int nextID = 0;
	std::string id = fmt::format("Server-{}", nextID++);
	libv::net::mtcp::AcceptorAsyncCB acceptor;
	std::vector<std::shared_ptr<Session>> sessions;

	Server(libv::net::IOContext& io_context) :
		acceptor(io_context) {

		acceptor.handle_accept([this](libv::net::mtcp::ConnectionAsnycCB conn) {

			log_sandbox.info("{} accepted", id);
			sessions.emplace_back(std::make_shared<Session>(std::move(conn)))->run();

			if (sessions.size() >= config.num_client)
				acceptor.cancel();
		});

		acceptor.handle_error([this](const std::error_code ec) {
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
	inline static int nextID = 0;
	std::string id = fmt::format("Client-{}", nextID++);
	libv::net::mtcp::ConnectionAsnycCB conn;

	Client(libv::net::IOContext& io_context) : conn(io_context) {
		conn.handle_connect([this](const libv::net::mtcp::Endpoint endpoint) {
			log_sandbox.info("{} connected to: {}", id, endpoint);
		});
		conn.handle_disconnect([this]() {
			log_sandbox.info("{} disconnected", id);
		});
		conn.handle_error([this](const std::error_code ec) {
			if (ec == netts::error::make_error_code(netts::error::eof))
				return;
			if (ec == netts::error::make_error_code(netts::error::operation_aborted))
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
	libv::logger.setFormat("{severity} {thread_id:2} {module:8}: {message}\n");
	std::cout << libv::logger;
	std::vector<std::thread> threads;

	threads.emplace_back(main_server);
	for (size_t i = 0; i < config.num_client; ++i)
		threads.emplace_back(main_client);

	for (auto& thread : threads)
		thread.join();

	return 0;
}
