// Project: libv.update, File: app/update/server_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/update/server/update_server.hpp>
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

using vn = libv::update::version_number;
using us = libv::update::update_signature;

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
			= app::default_port;

//	const auto enable_resource_server = args.flag
//			("-r", "--resource_server")
//			("resource_server", "Enable built in resource server")
//			= app::default_port;

//	const auto num_net_thread = args.require<uint16_t>
//			("-t", "--net_thread")
//			("net_thread", "Number of network IO threads")
//			= app::default_net_thread;

	args.require_no_unused();

	if (!args.parse(argc, argv)) {
		std::cerr << args.error_message(100) << args.usage(100);
		return EXIT_FAILURE;
	}

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	app::log_app.info("{}", args.report(100));

	// -------------------------------------------------------------------------------------------------

	const auto endpoint = libv::net::mtcp::parse_endpoint_or_throw(address.value(), port.value());

	auto server_settings = libv::update::UpdateServerSettings{};
	server_settings.endpoint = endpoint;
	server_settings.num_thread_net = 8;
	server_settings.num_accept_backlog = 4;
	server_settings.resource_servers = std::vector<libv::net::Address>{
			{"rs0.corruptedai.com", 25090},
			{"rs1.corruptedai.com", 25091},
			{"rs2.corruptedai.com", 25092},
//			{"rs3.corruptedai.com", 25093},
//			{"rs4.corruptedai.com", 25094},
	};

	{
		auto server = libv::update::UpdateServer{server_settings};

		server.register_update("app.update", "dev", vn{1}, vn{2}, 99999, us{12345});
		server.register_update("app.update", "dev", vn{2}, vn{3}, 99999, us{12345});
		server.register_update("app.update", "dev", vn{3}, vn{6}, 99999, us{12345});
		server.register_update("app.update", "dev", vn{4}, vn{6}, 99999, us{12345});
		server.register_update("app.update", "dev", vn{5}, vn{6}, 99999, us{12345});

		server.register_update("app.update", "stable", vn{2}, vn{3}, 99999, us{12345});
		server.register_update("app.update", "stable", vn{3}, vn{6}, 99999, us{12345});
		server.register_update("app.update", "stable", vn{4}, vn{6}, 99999, us{12345});

		server.start();

		for (std::string line; std::getline(std::cin, line);) {
			if (line == "quit")
				break;
	//		if (line.starts_with("create"))
	//			break;
		}
	}

//	app::UpdateServer us(update_server_settings);
//	if (enable_resource_server.value()) {
//		app::ResourceServer rs(resource_server_settings);
//		us.add_resource_server(rs);
//
//		for (const auto path : libv::wilcard_filepath("app/update/res/p*.*.patch"))
//			rs.add_files(path.generic_path, path);
//		rs.add_files("app.update::patch::{}", "app/update/res/", "p*.*.patch");
//	}

//	us.peer.on_resource_request([]{
//		rs.adopt(peer);
//	});



	return EXIT_SUCCESS;
}
