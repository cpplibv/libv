// Project: libv, File: app/natps/server/server.hpp

// std
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
// libv
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/net/io_context.hpp>


#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>



namespace natp {

// -------------------------------------------------------------------------------------------------

//constexpr inline std::chrono::steady_clock::duration natps_default_lifetime = std::chrono::seconds(60);
//constexpr inline std::chrono::steady_clock::duration natps_default_gc_interval = std::chrono::seconds(1);
constexpr inline std::chrono::steady_clock::duration natps_default_lifetime = std::chrono::milliseconds(50);
constexpr inline std::chrono::steady_clock::duration natps_default_gc_interval = std::chrono::milliseconds(1);

// -------------------------------------------------------------------------------------------------

using RoomID = uint64_t;

using Endpoint = libv::net::mtcp::Endpoint;

// -------------------------------------------------------------------------------------------------

// Protocol
//		Query {RoomID}
//		...

struct RequestCreateRoom {
	RoomID roomID;
	/*Endpoint implicit*/
};

struct ResponseCreateRoomSuccess {
	std::chrono::duration<uint32_t, std::chrono::seconds::period> keep_alive_interval;
};

struct RequestJoinRoom {
	RoomID roomID;
	/*Endpoint implicit*/
};

struct ResponseRoomInformation {
	Endpoint endpoint;
};

// -------------------------------------------------------------------------------------------------

class Server {
private:
	struct Entry {
		std::chrono::steady_clock::time_point end_of_life;
		Endpoint endpoint;
	};

private:
	Endpoint endpoint_ipv4;

	std::chrono::steady_clock::duration mapping_lifetime = natps_default_lifetime;

	std::unordered_map<RoomID, Entry> mappings;


private:
	boost::asio::ip::udp::endpoint sender_endpoint_;
	enum { max_length = 1024 };
	char data_[max_length];

	libv::net::IOContext io_context;
	// ! Ordering matters

	boost::asio::ip::udp::socket socket_;

public:
	Server(Endpoint endpoint, int num_thread_io);

public:
	void run();

public:
	void register_room(RoomID room, Endpoint endpoint);
	std::optional<Endpoint> query_room(RoomID room);

private:
	void remove_dead_rooms();

private:
	void do_receive();
	void do_send(std::size_t length);
};

// -------------------------------------------------------------------------------------------------

} // namespace natp
