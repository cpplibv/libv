// Project: libv.net, File: test/libv/net/mtcp_test.cpp, Author: Császár Mátyás [Vader]

// test
#include <catch/catch.hpp>
// libv
#include <libv/log/log.hpp>
#include <libv/mt/binary_latch.hpp>
#include <libv/utility/enum.hpp>
// std
#include <iostream>
#include <sstream>
#include <vector>
// pro
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/acceptor.hpp>
#include <libv/net/mtcp/connection_cb.hpp>
#include <libv/net/mtcp/endpoint.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

size_t num_server_thread = 1;
size_t num_client_thread = 1;
uint16_t test_port = 1800;

const auto msg_32 = "123456789_123456789_123456789_12";
const auto msg_64 = "123456789_123456789_123456789_123456789_123456789_123456789_1234";

template <typename Connection>
struct TestClient {
	Connection conn;

	std::vector<std::error_code> errors{};
	std::vector<libv::net::mtcp::Endpoint> connects{};
	std::vector<int> disconnects{};
	std::vector<libv::net::mtcp::Message> receives{};
	std::vector<libv::net::mtcp::Message> sends{};

	explicit TestClient(Connection&& conn) : conn(std::move(conn)) {
		init();
	}
	explicit TestClient(libv::net::IOContext& context) : conn(context) {
		init();
	}
	void init() {
		conn.handle_connect([&](libv::net::mtcp::Endpoint local_endpoint, libv::net::mtcp::Endpoint remote_endpoint) {
			(void) local_endpoint;

			libv::log.info("Connected");
			connects.emplace_back(remote_endpoint);
		});
		conn.handle_disconnect([&]() {
			libv::log.info("Disconnected");
			disconnects.emplace_back(0);
		});
		conn.handle_error([&](auto operation, std::error_code ec) {
			(void) operation;

			libv::log.error("Client " + libv::net::to_string(ec));
			errors.emplace_back(ec);
		});
		conn.handle_receive([&](libv::net::mtcp::Message&& packet) {
			libv::log.info("Message received with {} byte", packet.size());
			receives.emplace_back(std::move(packet));
		});
		conn.handle_send([&](libv::net::mtcp::Message&& packet) {
			libv::log.info("Message sent");
			sends.emplace_back(std::move(packet));
		});
	}
};

template <typename Acceptor>
struct TestServer {
	using Connection = typename Acceptor::Connection;
	Acceptor acc;

	std::vector<std::error_code> errors{};
	std::vector<TestClient<Connection>> accepts{};
	libv::BinaryLatch on_accept;

	TestServer(libv::net::IOContext& io_context) :
		acc(io_context) {
		acc.handle_accept([this](Connection conn) {
			libv::log.info("Accepted");
			accepts.emplace_back(std::move(conn));
			accepts.back().conn.start();

			on_accept.raise();
		});
		acc.handle_error([this](auto operation, std::error_code ec) {
			(void) operation;

			libv::log.error("Server " + libv::net::to_string(ec));
			errors.emplace_back(ec);
		});
	}
};

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("AAA reg logger") {
	std::cout << libv::logger_stream;
//	libv::logger_stream.setFormat("{severity} {thread_id:2} {module}: {message}\n");
}

TEST_CASE("MTCP Connection without any connect should handle errors") {
	libv::log.info("------------------------------------------------");

	libv::net::IOContext context{num_client_thread};
	TestClient<libv::net::mtcp::ConnectionAsyncCB> test{context};

	SECTION("If no operation is called") {
		context.join();
		CHECK(test.errors.size() == 0);
	}

	SECTION("when one operation of:") {
		SECTION("receive") {
			test.conn.receive();
		}
		SECTION("send") {
			test.conn.send(msg_64);
		}
		SECTION("disconnect") {
			test.conn.disconnect();
		}

		context.join();
		REQUIRE(test.errors.size() == 1);
		CHECK(test.errors[0] == boost::asio::error::make_error_code(boost::asio::error::not_connected));
	}

	SECTION("when two operation of:") {
		SECTION("receive send") {
			test.conn.receive();
			test.conn.send(msg_64);
		}
		SECTION("send receive") {
			test.conn.send(msg_64);
			test.conn.receive();
		}
		SECTION("receive disconnect") {
			test.conn.receive();
			test.conn.disconnect();
		}
		SECTION("disconnect receive") {
			test.conn.disconnect();
			test.conn.receive();
		}
		SECTION("disconnect send") {
			test.conn.disconnect();
			test.conn.send(msg_64);
		}
		SECTION("send disconnect") {
			test.conn.send(msg_64);
			test.conn.disconnect();
		}

		context.join();
		REQUIRE(test.errors.size() == 2);
		CHECK(test.errors[0] == boost::asio::error::make_error_code(boost::asio::error::not_connected));
		CHECK(test.errors[1] == boost::asio::error::make_error_code(boost::asio::error::not_connected));
	}

	SECTION("when three operation of:") {
		SECTION("d r s") {
			test.conn.disconnect();
			test.conn.receive();
			test.conn.send(msg_64);
		}
		SECTION("d s r") {
			test.conn.disconnect();
			test.conn.send(msg_64);
			test.conn.receive();
		}
		SECTION("r d s") {
			test.conn.receive();
			test.conn.disconnect();
			test.conn.send(msg_64);
		}
		SECTION("r s d") {
			test.conn.receive();
			test.conn.send(msg_64);
			test.conn.disconnect();
		}
		SECTION("s d r") {
			test.conn.send(msg_64);
			test.conn.disconnect();
			test.conn.receive();
		}
		SECTION("s r d") {
			test.conn.send(msg_64);
			test.conn.receive();
			test.conn.disconnect();
		}

		context.join();
		REQUIRE(test.errors.size() == 3);
		CHECK(test.errors[0] == boost::asio::error::make_error_code(boost::asio::error::not_connected));
		CHECK(test.errors[1] == boost::asio::error::make_error_code(boost::asio::error::not_connected));
		CHECK(test.errors[2] == boost::asio::error::make_error_code(boost::asio::error::not_connected));
	}

	CHECK(test.connects.size() == 0);
	CHECK(test.disconnects.size() == 0);
	CHECK(test.receives.size() == 0);
	CHECK(test.sends.size() == 0);
}

// NOTE: remove hide from this test long-term (Disabled it because of TCP timeouts)
TEST_CASE("MTCP Connection should stay inactive with a failed connection attempts", "[!hide]") {
//TEST_CASE("MTCP Connection should stay inactive with a failed connection attempts") {
	libv::log.info("------------------------------------------------");

	libv::net::IOContext context{num_client_thread};
	TestClient<libv::net::mtcp::ConnectionAsyncCB> test{context};

	SECTION("When: Host not found") {
		test.conn.connect(libv::net::Address("unknown host", test_port));

		context.join();
		REQUIRE(test.errors.size() == 1);
		CHECK(test.errors[0] == boost::asio::error::make_error_code(boost::asio::error::host_not_found));
	}
	SECTION("When: Connection refused") {
		test.conn.connect(libv::net::Address("127.0.0.1", test_port));

		context.join();
		REQUIRE(test.errors.size() == 1);
		CHECK(test.errors[0] == boost::asio::error::make_error_code(boost::asio::error::connection_refused));
	}
	SECTION("When: Connection timed out") {
		test.conn.connect(libv::net::Address("192.50.50.50", test_port));

		context.join();
		REQUIRE(test.errors.size() == 1);
		CHECK(test.errors[0] == boost::asio::error::make_error_code(boost::asio::error::timed_out));
	}
	SECTION("When: The service is not supported for the given socket type") {
		test.conn.connect(libv::net::Address("198.51.100.1", "no such service"));

		context.join();
		REQUIRE(test.errors.size() == 1);
		CHECK(test.errors[0] == boost::asio::error::make_error_code(boost::asio::error::service_not_found));
	}

	CHECK(test.connects.size() == 0);
	CHECK(test.disconnects.size() == 0);
	CHECK(test.receives.size() == 0);
	CHECK(test.sends.size() == 0);
}

TEST_CASE("MTCP Connection should handle successful connect-send-disconnect") {
	libv::log.info("------------------------------------------------");

	auto context_client = libv::net::IOContext(num_client_thread);
	auto client = TestClient<libv::net::mtcp::ConnectionAsyncCB>(context_client);

	auto context_server = libv::net::IOContext(num_server_thread);
	auto server = TestServer<libv::net::mtcp::AcceptorAsyncCB>(context_server);

	server.acc.listen(test_port);
	server.acc.accept();

	client.conn.connect(libv::net::Address("127.0.0.1", test_port));
	client.conn.send(msg_64);
	client.conn.disconnect();

	REQUIRE(server.on_accept.wait_for(std::chrono::seconds(3)));
	REQUIRE(server.accepts.size() == 1);

	server.accepts[0].conn.receive_repeat();
	server.acc.cancel();

	context_client.join();
	context_server.join();

	REQUIRE(server.accepts[0].errors.size() == 1);
	CHECK(server.accepts[0].errors[0] == boost::asio::error::make_error_code(boost::asio::error::eof));
	CHECK(server.accepts[0].connects.size() == 1);
	CHECK(server.accepts[0].disconnects.size() == 1);
	REQUIRE(server.accepts[0].receives.size() == 1);
	CHECK(server.accepts[0].receives[0] == msg_64);
	CHECK(server.accepts[0].sends.size() == 0);

	CHECK(client.errors.size() == 0);
	CHECK(client.connects.size() == 1);
	CHECK(client.disconnects.size() == 1);
	CHECK(client.receives.size() == 0);
	REQUIRE(client.sends.size() == 1);
	CHECK(client.sends[0] == msg_64);
}
