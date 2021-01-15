// Project: libv, File: app/update/client_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/net/address.hpp>
// std
#include <iostream>
// pro
#include <update/common/client.hpp>
#include <update/common/config.hpp>
#include <update/common/log.hpp>

//

//#include <update/patch/updater.hpp>
#include <update/patch/update_client.hpp>


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
			("address", "Server IP address to connect to")
			= app::default_address;

	const auto port = args.require<uint16_t>
			("-p", "--port")
			("port", "Server TCP port to connect to")
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

//	app::UpdateClient client;
//	client.update_from(libv::net::Address(address.value(), port.value()));
//
//	for (std::string line; std::getline(std::cin, line);) {
//		if (line == "quit")
//			break;
//
//		client.send(line);
//	}


	{
		static constexpr auto current_version = libv::update::version_number{1};

		const auto path_executable = std::filesystem::path(argv[0]);
		const auto path_root_dir = path_executable.parent_path();
		const auto update_servers = std::vector<libv::net::Address>{
			{"rs0.corruptedai.com", 25090},
			{"rs1.corruptedai.com", 25091},
			{"rs2.corruptedai.com", 25092},
			{"rs3.corruptedai.com", 25093},
			{"rs4.corruptedai.com", 25094},

//			{"89.135.41.137", 25090},
//			{"192.168.0.100", 25090},
//			{"127.0.0.1", 25090},
//			{"localhost", 25090},
		};

		libv::update::UpdateClient update_client;
		update_client.init(path_root_dir, path_executable, update_servers, current_version);
		update_client.check_for_update();

	}

	return EXIT_SUCCESS;
}

