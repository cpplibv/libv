// File: libv_net_server_main.hpp - Created on 2018.05.03. 10:51 - Author: Vader

// libv
#include <libv/log/log.hpp>
#include <libv/utility/timer.hpp>
// std
#include <iostream>
// pro
//#include <libv/net/packetizer.hpp>
//#include <libv/net/resolver.hpp>
//#include <libv/net/acceptor.hpp>
#include <libv/net/address.hpp>
//#include <libv/net/connection.hpp>
#include <libv/net/io_context.hpp>


// https://www.boost.org/doc/libs/1_67_0/doc/html/boost_asio/net_ts.html
// https://www.youtube.com/watch?v=dZdTOH9bFvs

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	std::cout << libv::logger;

//	const auto address = libv::net::Address("localhost", 29446);
//
//	libv::net::IOContext io_context{2};
//	libv::net::Acceptor acceptor{io_context, address.port};
//	libv::Timer timer;
//
//	//	io_context.resolve_async(address, [&timer](const auto& ec, auto endpoints) {
//	//		if (ec)
//	//			return;
//	//
//	//		LIBV_LOG_LIBV_INFO("Resolved {} endpoint in {:03}ms", endpoints.size(), timer.time_us().count() / 1000.0);
//	//		for (auto endpoint : endpoints)
//	//			LIBV_LOG_LIBV_INFO("{}:{} - {}", endpoint.host_name(), endpoint.service_name(), endpoint.endpoint());
//	//	});
//
//	auto f = [](auto ec, auto connection) {
//		if (ec)
//			return LIBV_LOG_LIBV_TRACE("Failed to accept connection: {}", ec.message());
//
////		connection
//		LIBV_LOG_LIBV_TRACE("All good");
//	};
//	acceptor.accept_continous_async(f);
////	acceptor.accept_continous_async([](auto ec, auto connection) {
////		if (ec)
////			return LIBV_LOG_LIBV_TRACE("Failed to accept connection: {}", ec.message());
////
////		LIBV_LOG_LIBV_TRACE("All good");
////	});
//
//	io_context.join();

	// =============================================================================================



//	libv::net::IOContext io_context{2};
//	libv::net::Address address("0.0.0.0", 29446);
//	libv::net::Acceptor acceptor{io_context, address};

//	acceptor.accept_continous_async([](auto& result) {
//
//	});

//	io_context.join();








	return 0;
}
