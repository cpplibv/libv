// Project: libv, File: app/update/common/config.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstdint>
#include <string_view>


namespace app {

// -------------------------------------------------------------------------------------------------

static constexpr std::string_view default_address = "localhost";
static constexpr uint16_t default_port = 25090;
static constexpr uint16_t default_net_thread = 4;

// -------------------------------------------------------------------------------------------------

struct Config {
//	uint16_t port = 25446;
//	size_t num_client = 3;
//	size_t num_client_io_thread = 2;
//	size_t num_server_io_thread = 2;
//	std::string request = "request";
//	std::string response = "response";

//	size_t num_io_thread = 4;

	size_t max_active_client = 100;
	size_t max_queue_client = 10000;

	size_t max_upload_bps = 10000;
	size_t max_upload_bps_per_client = 10000;
};

const Config config{};

// -------------------------------------------------------------------------------------------------

} // namespace app
