// Project: libv.net, File: sandbox/libv_net_client_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/log/log.hpp>
#include <libv/utility/timer.hpp>
// std
#include <iostream>
// pro
#include <libv/net/address.hpp>
//#include <libv/net/connection.hpp>
#include <libv/net/io_context.hpp>

#include <netts/socket.hpp>

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	std::cout << libv::logger_stream;

////	const auto address = libv::net::Address("ipon.hu", 80);
////	const auto address = libv::net::Address("localhost", 29446);
//	libv::net::Address address("127.0.0.1", 29446);
//
//	libv::net::IOContext io_context{2};
//	libv::Timer timer;
//
//	libv::net::Connection conn(boost::asio::ip::tcp::socket{io_context.context()}); // TODO P2: I don't like this socket{}
//
//	io_context.resolve_async(address, [&](auto ec, const auto& endpoints) {
//		if (ec)
//			return LIBV_LOG_LIBV_TRACE("Failed to resolve endpoint: {}", ec.message());
//
//		LIBV_LOG_LIBV_INFO("Resolved {} endpoint in {:.03f}ms", endpoints.size(), timer.time_us().count() / 1000.0);
//		for (auto endpoint : endpoints)
//			LIBV_LOG_LIBV_INFO("{}:{} - {}", endpoint.host_name(), endpoint.service_name(), endpoint.endpoint());
//
//		boost::asio::async_connect(conn.socket(), endpoints, [&](auto ec, const auto& next) {
//			if (ec)
//				LIBV_LOG_LIBV_TRACE("Failed connect condition: {}", ec.message());
//
////			std::cout << "Trying: " << next << std::endl;
//			LIBV_LOG_LIBV_INFO("Attempting connection...");
////			LIBV_LOG_LIBV_INFO("Attempting connection to {}...", next);
////			LIBV_LOG_LIBV_INFO("Attempting connection to {}:{} - {}...", next.host_name(), next.service_name(), next.endpoint());
////			LIBV_LOG_LIBV_INFO("Attempting connection to {}:{}", endpoint->address(), endpoint.port());
//			return true;
//
//		}, [&](auto ec, const auto& endpoint) {
//			if (ec)
//				return LIBV_LOG_LIBV_TRACE("Failed to connect: {}", ec.message());
//
//			LIBV_LOG_LIBV_INFO("Connected to {}:{} in {:.03f}ms", endpoint.address(), endpoint.port(), timer.time_us().count() / 1000.0);
//		});
//
//	});
//
//	io_context.join();

	// =============================================================================================



//	libv::net::IOContext io_context{2};
//	libv::net::Address address("127.0.0.1", 29446);
//	libv::net::Connection connection(io_context);

//	connection.connect_async(address,
//			[](auto ec, auto it) {
//				if (ec)
//					LIBV_LOG_LIBV_INFO("Failed to connect to: {} reason: {}", std::prev(it), ec.message());
//				LIBV_LOG_LIBV_INFO("Attempting to connect to: {}...", it);
//			},
//			[](auto ec, auto connection) {
//				if (ec)
//					LIBV_LOG_LIBV_INFO("Failed to connect: {}", ec.message());
//				else
//					LIBV_LOG_LIBV_INFO("Successfully connected to: {}", connection.endpoint());
//			}
//	);

//	io_context.join();








	return 0;
}
