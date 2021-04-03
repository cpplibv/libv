// Project: libv.update, File: app/update/resource_server_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/update/resource_server/resource_server.hpp>
#include <libv/range/glob_generic_path.hpp>
//#include <libv/update/net/server.hpp>
//#include <libv/update/net/updater_network_server.hpp>
//#include <libv/update/server/server.hpp>
//#include <libv/utility/parse_number.hpp>
// std
#include <iostream>
// pro
#include <update/common/config.hpp>
#include <update/common/log.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Update server", "LIBV Update server daemon");

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
			= app::default_resource_port;

//	const auto num_net_thread = args.require<uint16_t>
//			("-t", "--net_thread")
//			("net_thread", "Number of network IO threads")
//			= app::default_net_thread;

	if (!args.standard_validate(argc, argv, std::cerr, 100))
		return EXIT_FAILURE;

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	app::log_app.info("{}", args.report(100));

	// -------------------------------------------------------------------------------------------------

	const auto endpoint = libv::net::mtcp::parse_endpoint_or_throw(address.value(), port.value());

	std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path().parent_path());

	{
		auto resource_settings = libv::update::ResourceServer::Settings{};
		resource_settings.endpoint = endpoint;
		resource_settings.num_thread_net = libv::mt::hardware_concurrency_or(2) + 2;
		resource_settings.num_thread_fs = 2;
		resource_settings.num_accept_backlog = 4;

		auto server = libv::update::ResourceServer{resource_settings};

		for (const auto& path : libv::view::glob_generic_path("app/update/res/update", "test-dev-*-*.update")) {
			const auto id = "app.update::update::dev::" + path.generic_string();
			const auto file = "app/update/res/update/" + path.generic_string();
			const auto cache = libv::update::resource_cache_policy::always;

			server.add_file(id, file, cache);
		}

		for (const auto& path : libv::view::glob_generic_path("app/update/res/version", "v*/**")) {
			const auto id = "app.update::version::dev::" + path.generic_string();
			const auto file = "app/update/res/version/" + path.generic_string();
			const auto is_file = std::filesystem::is_regular_file(file);
			const auto cache = libv::update::resource_cache_policy::never;

			if (is_file)
				server.add_file(id, file, cache);
		}

		server.start();

		for (std::string line; std::getline(std::cin, line);) {
			if (line == "quit")
				break;
			//		if (line.starts_with("create"))
			//			break;
		}
	}
//
////	app::UpdateServer us(update_server_settings);
////	if (enable_resource_server.value()) {
////		app::ResourceServer rs(resource_server_settings);
////		us.add_resource_server(rs);
////	}
//
////	us.peer.on_resource_request([]{
////		rs.adopt(peer);
////	});



	return EXIT_SUCCESS;
}
