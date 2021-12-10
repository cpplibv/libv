// Project: libv, File: app/space/internal_events.hpp

#pragma once

// std
#include <string>


namespace space {
namespace mc {

// -------------------------------------------------------------------------------------------------

struct RequestNameChange {
	std::string name;
};
struct OnNameChange {};

// -------------------------------------------------------------------------------------------------

struct RequestCreateClient {
	std::string server_address;
	uint16_t server_port;
};
struct OnCreateClient {};

struct RequestCreateServer {
	uint16_t server_port;
};
struct OnCreateServer {};

struct RequestDestroyClient {};
struct OnDestroyClient {};

struct RequestDestroyServer {};
struct OnDestroyServer {};

// -------------------------------------------------------------------------------------------------

struct RequestShuffle {
};

struct RequestClearFleets {
//	std::string to_console_command() const {
//		return "clear_fleets";
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mc
} // namespace space
