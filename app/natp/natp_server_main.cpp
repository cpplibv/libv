// Project: libv, File: app/codegen/codegen_main.cpp

// libv
#include <libv/arg/arg.hpp>
#include <libv/utility/last_write_time.hpp>
// std
#include <filesystem>
#include <iostream>
// pro
#include <natp/server/log.hpp>
#include <natp/server/server.hpp>


// -------------------------------------------------------------------------------------------------

constexpr inline std::string_view natps_version = "v1.0.0";
constexpr inline uint16_t natps_default_port = 16100;
constexpr inline std::string_view natps_default_interface_ipv4 = "127.0.0.1";

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("NATPS", "NATPS - NAT Punching Server");

	const auto arg_interface = args.require<std::string>
			("-i", "--interface")
			("interface", "Specifies the interface that is listened to")
			= natps_default_interface_ipv4;

	const auto arg_port = args.require<uint16_t>
			("-p", "--port")
			("port", "Specifies the port that is listened to")
			= natps_default_port;

	const auto arg_debug = args.flag
			("-d", "--debug")
			("debug", "Enables debug mode")
			= true; // Force debug mode, Remove this later

	const auto arg_help = args.flag
			("-h", "--help", "?", "-?")
			("help", "Display the help message");

	if (!args.standard_validate(argc, argv, std::cerr, 120))
		return EXIT_FAILURE;

	if (arg_debug.value())
		args.report(std::cout, 120);

	if (arg_help.value()) {
		args.usage(std::cout, 120);
		return EXIT_SUCCESS;
	}

	// -------------------------------------------------------------------------------------------------

	if (arg_debug.value()) {
		libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	} else {
		libv::logger_stream.setFormat("{severity} {module}: {message}\n");
		libv::logger_stream.deny_below(libv::Logger::Severity::Info);
	}

	std::cout << libv::logger_stream;

	const auto path = std::filesystem::path(argv[0]);
	const auto path_bin = path.filename();
	const auto path_dir = path.parent_path();
	const auto lwt = libv::last_write_time(path);

	natp::log_natps.info("--- NAT Punching Server {} ---", natps_version);
	natp::log_natps.info_if(arg_debug.value(), "Running in debug mode");
	natp::log_natps.info("Current path  {}", std::filesystem::current_path().generic_string());
	natp::log_natps.info("Executable    {}/{}", path_dir.generic_string(), path_bin.generic_string());
	natp::log_natps.info("Last modified {:%Y.%m.%d %H:%M:%S}", lwt);

	try {
		// Run the server
		natp::Server server(libv::net::mtcp::parse_endpoint_or_throw(arg_interface.value(), arg_port.value()), 4);
		server.run();

	} catch (const std::exception& e) {
		natp::log_natps.fatal("Execution failed with an exception: {}", e.what());
		std::cerr << "Execution failed with an exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
