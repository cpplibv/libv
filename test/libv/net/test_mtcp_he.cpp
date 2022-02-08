// Project: libv.net, File: test/libv/net/mtcp_test.cpp

// test
#include <catch/catch.hpp>
#include <boost/asio/error.hpp>
// libv
#include <libv/log/log.hpp>
//#include <libv/mt/binary_latch.hpp>
//#include <libv/utility/to_underlying.hpp>
// std
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <vector>
//#include <sstream>
// pro
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/acceptor_he.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/net/mtcp/message.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

std::size_t num_server_thread = 4;
std::size_t num_client_thread = 2;
uint16_t test_port = 1800;

const std::string msg_32 = "123456789_123456789_123456789_12";
const std::string msg_64 = "123456789_123456789_123456789_123456789_123456789_123456789_1234";

class TestEventLog {
	mutable std::mutex queue_m;
	mutable std::condition_variable queue_cv;
	std::vector<std::string> queue;
//	std::size_t head = 0;

public:
	void add(std::string event) {
		auto lock = std::unique_lock(queue_m);
		queue.emplace_back(std::move(event));
		queue_cv.notify_all();
	}

	std::size_t queue_size() {
		auto lock = std::unique_lock(queue_m);
		return queue.size();
	}

	std::size_t consume_queue() {
		auto lock = std::unique_lock(queue_m);
		const auto n = queue.size();
		queue.clear();
		return n;
	}

	std::optional<std::string> consume_first() {
		auto lock = std::unique_lock(queue_m);

		std::optional<std::string> result;

		if (!queue.empty()) {
			result = queue.front();
			queue.erase(queue.begin());
		}

		return result;
	}

	template <typename Durr = std::chrono::seconds>
	std::optional<std::string> consume_first_wait_for(Durr timeout = std::chrono::seconds{5}) {
		auto lock = std::unique_lock(queue_m);
		queue_cv.wait_for(lock, timeout, [&](){ return !queue.empty(); });

		std::optional<std::string> result;

		if (!queue.empty()) {
			result = queue.front();
			queue.erase(queue.begin());
		}

		return result;
	}

	bool empty() const {
		auto lock = std::unique_lock(queue_m);
		return queue.empty();
	}

//	const std::string& next() {
//		auto lock = std::unique_lock(queue_m);
//		return queue.at(head++);
//	}
};

class TestClient : public libv::net::mtcp::ConnectionHandler<TestClient> {
	TestEventLog& connectionLog;

public:
	explicit TestClient(TestEventLog& connectionLog) : connectionLog(connectionLog) {}

private:
	virtual void on_connect() override {
		connectionLog.add("on_connect");
	}
	virtual void on_connect_error(error_code ec) override {
		connectionLog.add("on_connect:" + libv::net::to_string(ec));
	}
	virtual void on_receive(message&& m) override {
		connectionLog.add("on_receive:" + m.copy_str());
	}
	virtual void on_receive_error(error_code ec, message&& m) override {
		(void) m;
		connectionLog.add("on_receive:" + libv::net::to_string(ec));
	}
	virtual void on_send(message&& m) override {
		connectionLog.add("on_send:" + m.copy_str());
	}
	virtual void on_send_error(error_code ec, message&& m) override {
		(void) m;
		connectionLog.add("on_send:" + libv::net::to_string(ec));
	}
	virtual void on_disconnect(error_code ec) override {
		if (ec)
			connectionLog.add("on_disconnect:" + libv::net::to_string(ec));
		else
			connectionLog.add("on_disconnect");
	}
};
using TestConnection = libv::net::mtcp::Connection<TestClient>;

class TestServerHandler : public libv::net::mtcp::AcceptorHandler<TestServerHandler> {
	TestEventLog& serverLog;
	TestEventLog& peerLog;

public:
	std::vector<TestConnection> peers;

public:
	explicit TestServerHandler(TestEventLog& serverLog, TestEventLog& peerLog) : serverLog(serverLog), peerLog(peerLog) {}

private:
	virtual libv::net::mtcp::Connection<void> on_accept(io_context& io_context) override {
		serverLog.add("on_accept");
		auto conn = TestConnection{io_context, peerLog};
		peers.emplace_back(conn);
		return conn;
	}

	virtual void on_accept(error_code ec) override {
		serverLog.add("on_accept:" + libv::net::to_string(ec));
	}
};
using TestServer = libv::net::mtcp::Acceptor<TestServerHandler>;

//template <typename Acceptor>
//struct TestServer {
//	using Connection = typename Acceptor::Connection;
//	Acceptor acc;
//
//	std::vector<std::error_code> errors{};
//	std::vector<TestClient> accepts{};
//	libv::BinaryLatch on_accept;
//
//	TestServer(libv::net::IOContext& io_context) :
//		acc(io_context) {
//		acc.handle_accept([this](Connection conn) {
//			libv::log.info("Accepted");
//			accepts.emplace_back(std::move(conn));
//			accepts.back().conn.start();
//
//			on_accept.raise();
//		});
//		acc.handle_error([this](auto operation, std::error_code ec) {
//			(void) operation;
//
//			libv::log.error("Server " + libv::net::to_string(ec));
//			errors.emplace_back(ec);
//		});
//	}
//};

} // namespace

namespace std {

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& var) {
	if (var)
		return os << *var;
	else
		return os << "<<nullopt>>";
}

} // namespace std


namespace error = boost::asio::error;

template <typename T>
std::string err_str(T error_enum) {
	return libv::net::to_string(boost::asio::error::make_error_code(error_enum));
}

// =================================================================================================
// =================================================================================================
// =================================================================================================

TEST_CASE("AAA reg logger", "[libv.net.mtcp]") {
	std::cout << libv::logger_stream;
//	libv::logger_stream.setFormat("{severity} {thread_id:2} {module}: {message}\n");
}

TEST_CASE("MTCP Connection without any connect should handle incorrect calls", "[libv.net.mtcp]") {
	libv::log.info("------------------------------------------------");

	TestEventLog events;
	libv::net::IOContext context{num_client_thread};

	SECTION("IO context without any connection") {
		// noop
	}

	SECTION("io context will not hang if every client is marked for disconnect") {
		TestConnection client0{context, events};

		SECTION("with complete") {
			client0.connection().complete_and_disconnect_async();
		}
		SECTION("with cancel") {
			client0.connection().cancel_and_disconnect_async();
		}
		SECTION("with clear (aka dtor)") {
			client0.clear();
		}
		context.join();
		CHECK(events.queue_size() == 0);
	}

	SECTION("0 operation") {
		{
			TestConnection client0{context, events};
			// Has to call dtor to reset the work guard (implicitly via cancel_and_disconnect_async)
		}
		context.join();

		CHECK(events.queue_size() == 0);
	}

	SECTION("1 operation") {
		TestConnection client0{context, events};

		SECTION("1 operation: complete") {
			client0.connection().complete_and_disconnect_async();
		}
		SECTION("1 operation: cancel") {
			client0.connection().cancel_and_disconnect_async();
		}
		SECTION("1 operation: resume_receive_async") {
			client0.connection().resume_receive_async();
		}
		SECTION("1 operation: send_async") {
			client0.connection().send_copy_async(msg_64);
		}
		client0.clear();

		context.join();
		CHECK(events.queue_size() == 0);
	}

	SECTION("2 operation") {
		TestConnection client0{context, events};

		SECTION("2 operation: a") {
			client0.connection().send_copy_async(msg_64);
			client0.connection().complete_and_disconnect_async();
		}
		SECTION("2 operation: b") {
			client0.connection().complete_and_disconnect_async();
			client0.connection().send_copy_async(msg_64);
		}
		SECTION("2 operation: c") {
			client0.connection().send_copy_async(msg_64);
			client0.connection().cancel_and_disconnect_async();
		}
		SECTION("2 operation: d") {
			client0.connection().cancel_and_disconnect_async();
			client0.connection().send_copy_async(msg_64);
		}
		SECTION("2 operation: e") {
			client0.connection().resume_receive_async();
			client0.connection().send_copy_async(msg_64);
		}
		SECTION("2 operation: f") {
			client0.connection().send_copy_async(msg_64);
			client0.connection().resume_receive_async();
		}
		client0.clear();

		context.join();
		CHECK(events.queue_size() == 0);
	}
}

// NOTE: Long running test due to TCP timeouts
//TEST_CASE("MTCP Connection should handle failed connection attempts", "[!hide][libv.net.mtcp]") {
TEST_CASE("MTCP Connection should handle failed connection attempts", "[libv.net.mtcp]") {
	libv::log.info("------------------------------------------------");

	TestEventLog events;
	libv::net::IOContext context{num_client_thread};

	TestConnection client0{context, events};

	SECTION("When: Host not found") {
		client0.connection().connect_async(libv::net::Address("unknown host", test_port));

		CHECK(events.consume_first_wait_for() == "on_connect:" + err_str(error::host_not_found));
	}
	SECTION("When: Connection refused") {
		client0.connection().connect_async(libv::net::Address("127.0.0.1", test_port));

		CHECK(events.consume_first_wait_for() == "on_connect:" + err_str(error::connection_refused));
	}
	SECTION("When: Connection timed out") {
		client0.connection().connect_async(libv::net::Address("192.50.50.50", test_port));

		CHECK(events.consume_first_wait_for(std::chrono::seconds{65}) == "on_connect:" + err_str(error::timed_out));
	}
	SECTION("When: The service is not supported for the given socket type") {
		client0.connection().connect_async(libv::net::Address("198.51.100.1", "no such service"));

		CHECK(events.consume_first_wait_for() == "on_connect:" + err_str(error::service_not_found));
	}

	client0.clear();
	context.join();
	CHECK(events.empty());
}

TEST_CASE("MTCP Acceptor without any incoming connection should handle its lifetime", "[libv.net.mtcp]") {
	libv::log.info("------------------------------------------------");

	TestEventLog events;

	auto context_server = libv::net::IOContext(num_server_thread);

	SECTION("Without any operation") {
		auto server = TestServer(context_server, events, events);
		(void) server;
	}

	SECTION("With setup, but without teardown and accept") {
		auto server = TestServer(context_server, events, events);
		const auto listen_ec = server.acceptor().listen(test_port);
		CHECK(!listen_ec);
	}

	SECTION("With setup and teardown, but without accept") {
		auto server = TestServer(context_server, events, events);
		const auto listen_ec = server.acceptor().listen(test_port);
		CHECK(!listen_ec);
		server.acceptor().cancel();
	}

	SECTION("With: accept, cancel, dtor") {
		auto server = TestServer(context_server, events, events);
		server.acceptor().accept_async();
		server.acceptor().cancel();
	}

	SECTION("With: listen, accept, dtor") {
		auto server = TestServer(context_server, events, events);
		const auto listen_ec = server.acceptor().listen(test_port);
		CHECK(!listen_ec);
		server.acceptor().accept_async();
	}

	SECTION("With full operation: listen, accept, cancel, dtor") {
		auto server = TestServer(context_server, events, events);
		const auto listen_ec = server.acceptor().listen(test_port);
		CHECK(!listen_ec);
		server.acceptor().accept_async();
		server.acceptor().cancel();
	}

	context_server.join();
}

TEST_CASE("MTCP simple test for Connection - Acceptor pair", "[libv.net.mtcp]") {
	libv::log.info("------------------------------------------------");

	TestEventLog events_server;
	TestEventLog events_peers;
	TestEventLog events_client;

	auto context_server = libv::net::IOContext(num_server_thread);
	auto context_client = libv::net::IOContext(num_client_thread);

	{
		auto server = TestServer(context_server, events_server, events_peers);

		const auto listen_ec = server.acceptor().listen(test_port);
		CHECK(!listen_ec);
		server.acceptor().accept_async();

		SECTION("Client: connect, send, complete, clear") {
			auto client0 = TestConnection{context_client, events_client};

			client0.connection().connect_async(libv::net::Address("127.0.0.1", test_port));
			client0.connection().send_copy_async(msg_64);
			client0.connection().complete_and_disconnect_async();
			client0.clear();

			CHECK(events_client.consume_first_wait_for() == "on_connect");
			CHECK(events_client.consume_first_wait_for() == "on_send:" + msg_64);
			CHECK(events_client.consume_first_wait_for() == "on_disconnect");

			CHECK(events_peers.consume_first_wait_for() == "on_connect");
			CHECK(events_peers.consume_first_wait_for() == "on_receive:" + msg_64);
			CHECK(events_peers.consume_first_wait_for() == "on_disconnect");
		}

		SECTION("Client: connect, send, receive, complete, clear") {
			auto client0 = TestConnection{context_client, events_client};

			client0.connection().connect_async(libv::net::Address("127.0.0.1", test_port));
			client0.connection().send_copy_async(msg_64);

			CHECK(events_client.consume_first_wait_for() == "on_connect");
			CHECK(events_client.consume_first_wait_for() == "on_send:" + msg_64);

			CHECK(events_peers.consume_first_wait_for() == "on_connect");
			CHECK(events_peers.consume_first_wait_for() == "on_receive:" + msg_64);

			REQUIRE(server->peers.size() == 1);
			server->peers[0].connection().send_copy_async(msg_32);

			client0.connection().complete_and_disconnect_async();
			client0.clear();

			CHECK(events_client.consume_first_wait_for() == "on_receive:" + msg_32);
			CHECK(events_client.consume_first_wait_for() == "on_disconnect");

			CHECK(events_peers.consume_first_wait_for() == "on_send:" + msg_32);
			CHECK(events_peers.consume_first_wait_for() == "on_disconnect");
		}

		SECTION("Error with client: connect, send, cancel, receive, complete, clear") {
			auto client0 = TestConnection{context_client, events_client};

			client0.connection().connect_async(libv::net::Address("127.0.0.1", test_port));
			client0.connection().send_copy_async(msg_64);

			CHECK(events_client.consume_first_wait_for() == "on_connect");
			CHECK(events_client.consume_first_wait_for() == "on_send:" + msg_64);

			CHECK(events_peers.consume_first_wait_for() == "on_connect");
			CHECK(events_peers.consume_first_wait_for() == "on_receive:" + msg_64);

			client0.connection().cancel_and_disconnect_async();
			client0.clear();

			CHECK(events_client.consume_first_wait_for() == "on_receive:" + err_str(error::operation_aborted));
			CHECK(events_client.consume_first_wait_for() == "on_disconnect:" + err_str(error::connection_aborted));

			REQUIRE(server->peers.size() == 1);
			server->peers[0].connection().send_copy_async(msg_32);

			// Send doesn't happen, as the connection already disconnected
			CHECK(events_peers.consume_first_wait_for() == "on_disconnect");
		}

		CHECK(events_server.consume_first_wait_for() == "on_accept");

		server.acceptor().cancel();
	}

	context_client.join();
	context_server.join();

	CHECK(events_server.empty());
	CHECK(events_peers.empty());
	CHECK(events_client.empty());
}
