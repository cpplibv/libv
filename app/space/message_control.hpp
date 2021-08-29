// Project: libv, File: app/space/message_control.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string>


namespace app {
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
//struct OnClearFleets {};

// -------------------------------------------------------------------------------------------------

} // namespace mc
} // namespace app
