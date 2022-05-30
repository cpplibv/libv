// Project: libv, File: app/star/star_main.cpp

//// ext
//#include <fmt/chrono.h>
//#include <fmt/ostream.h>
// libv
#include <libv/arg/arg.hpp>
#include <libv/utility/last_write_time.hpp>
// std
#include <chrono>
#include <filesystem>
#include <iostream>
// pro
#include <star/game/game_client.hpp>
#include <star/log.hpp>
#include <star/version.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {

	auto args = libv::arg::Parser("Star", "Star - System integration demo");

//	const auto arg_config = args.require<std::string>
//			("-c", "--config")
//			("config", "Path to the config file")
//			= "config_dunes.lua";

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

//	if (!arg_verbose.value()) {
//		libv::logger_stream.deny_below(libv::Logger::Severity::Info);
////		libv::logger_stream.deny_below("libv.gl", libv::Logger::Severity::Info);
////		libv::logger_stream.deny_below("libv.frame", libv::Logger::Severity::Info);
////		libv::logger_stream.deny_below("libv.rev", libv::Logger::Severity::Info);
////		libv::logger_stream.deny_below("libv.ui", libv::Logger::Severity::Info);
////		libv::logger_stream.deny_below("libv.net", libv::Logger::Severity::Info);
//	}

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	try {
//		static constexpr auto DEFAULT_CONFIG_FILENAME = "app_vm4_viewer_config.json";
//		static constexpr auto DEFAULT_PROJECT_FILENAME = "vm4_viewer_default.json";
//		static constexpr auto DEFAULT_PROJECT_FOLDER = "project";
//
//		const auto path = std::filesystem::path(argv[0]);
//		const auto path_bin = path.filename();
//		const auto path_dir = path.parent_path();
//		const auto config_filename = std::filesystem::path(DEFAULT_CONFIG_FILENAME);
//		const auto config_path = path_dir / config_filename;
//		const auto lwt = libv::last_write_time(path);
//		app::log_app.info("Current path  {}", std::filesystem::current_path().generic_string());
//		app::log_app.info("Executable    {}/{}", path_dir.generic_string(), path_bin.generic_string());
//		app::log_app.info("Last modified {:%Y.%m.%d %H:%M:%S}", lwt);
		star::log_star.info("Star {}", star::version);

		// Change working directory
		if (std::filesystem::exists("app/star/"))
			std::filesystem::current_path("app/star/");
		else
			std::filesystem::current_path("../app/star/"); // During development binary artifacts created under /bin

		// Run the game
		star::GameClient game("config.json");
		game.run();

	} catch (const std::exception& e) {
		star::log_star.fatal("Execution failed with an exception: {}", e.what());
		std::cerr << "Execution failed with an exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
