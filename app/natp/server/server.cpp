// Project: libv, File: app/natps/server/server.cpp

// hpp
#include <natp/server/server.hpp>
// std
#include <thread>
// pro
#include <natp/server/log.hpp>


//// libv
#include <libv/net/address.hpp>
#include <libv/utility/timer.hpp>
// ext
#include <boost/asio/ip/udp.hpp>


namespace natp {

// -------------------------------------------------------------------------------------------------


Server::Server(Endpoint endpoint_ipv4, int num_thread_io) :
	endpoint_ipv4(endpoint_ipv4),
	io_context(num_thread_io),
//	socket_(io_context.context()) {
	socket_(io_context.context(), boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), endpoint_ipv4.port)) {
//	socket_(io_context.context(),
//			boost::asio::ip::udp::endpoint(
//				boost::asio::ip::address_v4(endpoint_ipv4.address_ipv4_uint()),
//				endpoint_ipv4.port)) {
//	natp::log_natps.info("Start server on {}...", endpoint_ipv4);

	mappings.reserve(1024);
}

void Server::run() {
	natp::log_natps.info("Start server on {}...", endpoint_ipv4);
	do_receive();

	register_room(100'000, {127, 0, 0, 1, 55000});
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	register_room(100'001, {127, 0, 0, 1, 55001});
	register_room(100'002, {2, 2, 2, 2, 55000});
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	register_room(100'003, {127, 0, 0, 1, 55002});

//	socket.listen(interface, port);

	for (int i = 0; i < 20000; ++i) {
		std::this_thread::sleep_for(natps_default_gc_interval);

		remove_dead_rooms();
	}
}

void Server::register_room(RoomID room, Endpoint endpoint) {
	const auto now = std::chrono::steady_clock::now();
	const auto end_of_life = now + mapping_lifetime;

	natp::log_natps.trace("Register room {} for {}", room, endpoint);
	mappings.emplace(room, Entry{end_of_life, endpoint});
}

std::optional<Endpoint> Server::query_room(RoomID room) {
	const auto it = mappings.find(room);
	if (it == mappings.end())
		return it->second.endpoint;

	return std::nullopt;
}

void Server::remove_dead_rooms() {
	const auto now = std::chrono::steady_clock::now();

	const auto num_expired = std::erase_if(mappings, [&](const auto& pair) {
		return now > pair.second.end_of_life;
	});

	natp::log_natps.trace_if(num_expired > 0, "Removed {} expired room", num_expired);
}

// -------------------------------------------------------------------------------------------------

void Server::do_receive() {
	socket_.async_receive_from(
			boost::asio::buffer(data_, max_length),
			sender_endpoint_,
			[this](boost::system::error_code ec, std::size_t bytes_recvd) {

				natp::log_natps.trace("ec {} - {} bytes_recvd {} sender {}:{}", ec, ec.message(), bytes_recvd, sender_endpoint_.address().to_string(), sender_endpoint_.port());

				if (ec == boost::system::errc::operation_canceled)
					return;

				if (!ec && bytes_recvd > 0) {

					if (request_to_open_room) {
						register_room(...);
						socket_??.async_send_to(
								"requested_room_was_created_its_alive_for_{}_seconds", sender_endpoint_,
								[](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {});
					} else if (request_to_join_room) {
						...
					} else {
						handle_unkown_message
					}

//					do_send(bytes_recvd);
					do_receive();
				} else {
					do_receive();
				}
			});
}

void Server::do_send(std::size_t length) {
//	socket_.async_send_to(
//			boost::asio::buffer(data_, length), sender_endpoint_,
//			[this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
//				do_receive();
//			});
}

// -------------------------------------------------------------------------------------------------

} // namespace natp


//static void nanosleep(long long microseconds) {
//	time_t seconds = microseconds / 1000000; // second part
//	long int nanoSeconds = (microseconds % 1000000) * 1000; // nano seconds part
//	if (seconds > 0)
//		Sleep(seconds * 1000 + nanoSeconds / 1000000); // If more than one second
//
//	else {
//		static double frequency; // ticks per second
//		if (frequency == 0) {
//			LARGE_INTEGER freq;
//			if (!QueryPerformanceFrequency(&freq)) {
//				/* Cannot use QueryPerformanceCounter. */
//				Sleep(nanoSeconds / 1000000);
//				return;
//			}
//			frequency = (double) freq.QuadPart / 1000000000.0; // ticks per nanosecond
//		}
//
//		long long counter_difference = nanoSeconds * frequency;
//		int sleep_part = (int) nanoSeconds / 1000000 - 10;
//		LARGE_INTEGER start;
//		QueryPerformanceCounter(&start);
//
//		long long expected_counter = start.QuadPart + counter_difference;
//		if (sleep_part > 0)     // for milliseconds part
//			Sleep(sleep_part);
//
//		while (true) {
//			LARGE_INTEGER stop;
//			QueryPerformanceCounter(&stop);
//			if (stop.QuadPart >= expected_counter)
//				break;
//		}
//	}
//}
