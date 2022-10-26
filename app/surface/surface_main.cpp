// Project: libv, File: app/surface/surface_main.cpp

// libv
#include <libv/arg/arg.hpp>
// std
#include <filesystem>
#include <iostream>
// pro
#include <surface/log.hpp>
#include <surface/surface/surface_viewer.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Surface", "Surface generation and visualization tool");

	const auto arg_config = args.require<std::string>
			("-c", "--config")
			("config", "Path to the config file")
			= "config_dunes.lua";

	const auto arg_verbose = args.flag
			("-v", "--verbose")
			("verbose", "Enables verbose mode");

	args.positional(arg_config);

	if (!args.standard_validate(argc, argv, std::cerr, 100))
		return EXIT_FAILURE;

	if (arg_verbose.value())
		std::cout << args.report(100);

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	if (!arg_verbose.value()) {
		libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}\n");
		libv::logger_stream.deny_below("libv.frame", libv::Logger::Severity::Info);
		libv::logger_stream.deny_below("libv.rev", libv::Logger::Severity::Info);
		libv::logger_stream.deny_below("libv.ui", libv::Logger::Severity::Info);
	}

	try {
		const auto devMode =
				std::filesystem::exists("CMakeLists.txt") ||
				std::filesystem::exists("../CMakeLists.txt") ||
				std::filesystem::exists("../../CMakeLists.txt");

		if (!devMode)
			libv::logger_stream.color(false);

		surface::log_surface.info("Hello Surface!{}", devMode ? " DevMode" : " DistMode");

		const auto configFolder = devMode ? "app/surface/config/" : "config/";

		// Run the game
		surface::SurfaceViewer surfaceViewer(configFolder + arg_config.value(), configFolder);
		surfaceViewer.execute();

	} catch (const std::exception& e) {
		surface::log_surface.fatal("Execution failed with an exception: {}", e.what());
		std::cerr << "Execution failed with an exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
