// Project: libv, File: app/star/star_main.cpp

// libv
#include <libv/arg/arg.hpp>
#include <libv/sys/os_version.hpp>
#include <libv/utility/last_write_time.hpp>
//#include <libv/utility/read_file.hpp>
// std
#include <filesystem>
#include <iostream>
// pro
//#include <star/game/config/client_config.hpp>
#include <star/game/game_client.hpp>
#include <star/log.hpp>
#include <star/version.hpp>


// -------------------------------------------------------------------------------------------------

constexpr inline auto default_config_filepath = "config.json";

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {

	auto args = libv::arg::Parser("Star", "Star - System integration environment");

	const auto arg_config = args.require<std::string>
			("-c", "--config")
			("config", "Path to the config file")
			= default_config_filepath;

	const auto arg_verbose = args.flag
			("-v", "--verbose")
			("verbose", "Enables verbose mode");

	const auto arg_help = args.flag
			("-h", "--help", "?", "-?")
			("help", "Display the help message");

//	args.positional(arg_config);

	if (!args.standard_validate(argc, argv, std::cerr, 120))
		return EXIT_FAILURE;

	if (arg_verbose.value())
		args.report(std::cout, 120);

	if (arg_help.value()) {
		args.usage(std::cout, 120);
		return EXIT_SUCCESS;
	}

	// -------------------------------------------------------------------------------------------------

	const auto devMode = std::filesystem::exists("../../CMakeLists.txt");

	if (arg_verbose.value()) {
		libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	} else {
		libv::logger_stream.setFormat(devMode ?
				"{severity} {module}: {message}, {file}:{line}\n" :
				"{severity} {module}: {message}\n");
		libv::logger_stream.allow("star");
		libv::logger_stream.deny_below(libv::Logger::Severity::Info);
//		libv::logger_stream.deny_below("libv.gl", libv::Logger::Severity::Info);
//		libv::logger_stream.deny_below("libv.frame", libv::Logger::Severity::Info);
//		libv::logger_stream.deny_below("libv.rev", libv::Logger::Severity::Info);
//		libv::logger_stream.deny_below("libv.ui", libv::Logger::Severity::Info);
//		libv::logger_stream.deny_below("libv.net", libv::Logger::Severity::Info);
	}

	std::cout << libv::logger_stream;

	try {
		// Change working directory
		if (devMode) // During development, binary artifacts created under /bin/build_type
			std::filesystem::current_path("../../app/star/");

		const auto path = std::filesystem::path(argv[0]);
		const auto path_bin = path.filename();
		const auto path_dir = path.parent_path();
//		const auto path_resource = "../../res";
		const auto lwt = libv::last_write_time(path);

		star::log_star.info("--- Star {} ---", star::build.version_number);
		star::log_star.info_if(devMode, "Running in developer mode");
		star::log_star.info("Current path:  {}", std::filesystem::current_path().generic_string());
		star::log_star.info("Executable:    {}/{}", path_dir.generic_string(), path_bin.generic_string());
		star::log_star.info("Last modified: {:%Y.%m.%d %H:%M:%S}", lwt);
		star::log_star.info("Config file:   {}", arg_config.value());
//		star::log_star.info("Resource path: {}", path_resource);
		star::log_star.info("System:        {}", libv::sys::os_version());

		// Run the game
		auto game = star::GameClient(devMode, arg_config.value());
		game.run();

	} catch (const std::exception& e) {
		star::log_star.fatal("Execution failed with an exception: {}", e.what());
		std::cerr << "Execution failed with an exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
