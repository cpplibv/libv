// Project: libv, File: app/update/server_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/utility/parse_number.hpp>
// std
#include <iostream>
// pro
#include <update/common/config.hpp>
#include <update/common/log.hpp>
#include <update/common/server.hpp>


// -------------------------------------------------------------------------------------------------

libv::net::mtcp::Endpoint parse_endpoint(std::string_view address, uint16_t port) {
	size_t p0 = address.find('.', 0);
	if (p0 == std::string_view::npos)
		throw std::invalid_argument("\"" + std::string(address) + "\" is not a valid endpoint address");

	size_t p1 = address.find('.', p0 + 1);
	if (p1 == std::string_view::npos)
		throw std::invalid_argument("\"" + std::string(address) + "\" is not a valid endpoint address");

	size_t p2 = address.find('.', p1 + 1);
	if (p2 == std::string_view::npos)
		throw std::invalid_argument("\"" + std::string(address) + "\" is not a valid endpoint address");

	try {
		const auto a0 = libv::parse_number_or_throw<uint8_t>(address.substr(0, p0));
		const auto a1 = libv::parse_number_or_throw<uint8_t>(address.substr(p0 + 1, p1 - p0 - 1));
		const auto a2 = libv::parse_number_or_throw<uint8_t>(address.substr(p1 + 1, p2 - p1 - 1));
		const auto a3 = libv::parse_number_or_throw<uint8_t>(address.substr(p2 + 1, address.size() - p2));
		return libv::net::mtcp::Endpoint(a0, a1, a2, a3, port);
	} catch (const std::exception& e) {
		throw std::invalid_argument("\"" + std::string(address) + "\" is not a valid endpoint address: " + e.what());
	}
}

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Updater server", "LIBV Updater server daemon");

//	const auto config_file = args.require<std::string>
//			("-c", "--config")
//			("config_file", "File path of the config file");
//
//	const auto manifest_file = args.require<std::string>
//			("-m", "--manifest")
//			("manifest_file", "File path of the manifest file");

	const auto address = args.require<std::string>
			("-a", "--address")
			("address", "Listening IP address")
			= "0.0.0.0";

	const auto port = args.require<uint16_t>
			("-p", "--port")
			("port", "Listening TCP port")
			= app::default_port;

	const auto num_net_thread = args.require<uint16_t>
			("-t", "--net_thread")
			("net_thread", "Number of network IO threads")
			= app::default_net_thread;

	args.require_no_unused();

	if (!args.parse(argc, argv)) {
		std::cerr << args.error_message(100) << args.usage(100);
		return EXIT_FAILURE;
	}

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	app::log_app.info("{}", args.report(100));

	const auto endpoint = parse_endpoint(address.value(), port.value());
	app::log_app.info("Listening on: {}...", endpoint);

	app::UpdateServer server = {endpoint, num_net_thread.value()};

	for (std::string line; std::getline(std::cin, line);) {
		if (line == "quit")
			break;
	}

	return EXIT_SUCCESS;
}

