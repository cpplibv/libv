// Project: libv.update, File: app/update/server_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
//#include <libv/utility/parse_number.hpp>
#include <libv/update/common/server.hpp>
// std
#include <iostream>
// pro
#include <update/common/config.hpp>
#include <update/common/log.hpp>


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

//	const auto enable_resource_server = args.flag
//			("-r", "--resource_server")
//			("resource_server", "Enable built in resource server")
//			= app::default_port;

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

	const auto endpoint = libv::net::mtcp::parse_endpoint_or_throw(address.value(), port.value());
	app::log_app.info("Listening on: {}...", endpoint);

//	app::UpdateServer us(update_server_settings);
//	if (enable_resource_server.value()) {
//		app::ResourceServer rs(resource_server_settings);
//		us.add_resource_server(rs);
//
//		for (const auto path : libv::wilcard_filepath("app/update/res/p*.*.patch"))
//			rs.add_files(path.generic_path, path);
//		rs.add_files("app.update::patch::{}", "app/update/res/", "p*.*.patch");
//
//      Resource ID:
//          app.update::patch::{filename}
//                             ^^^^^^^^^^ filename template
//                    ^^     ^^ separators
//                      ^^^^^ resource namespace
//          ^^^^^^^^^^ program / program url
//	}

//	us.peer.on_resource_request([]{
//		rs.adopt(peer);
//	});

	app::UpdateServer server = {endpoint, num_net_thread.value()};

	for (std::string line; std::getline(std::cin, line);) {
		if (line == "quit")
			break;

		server.broadcast(line);
	}

	return EXIT_SUCCESS;
}
