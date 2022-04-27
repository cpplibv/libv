// Project: libv, File: app/space/space_main.cpp

//std
#include <filesystem>

// space
#include <surface/log.hpp>
#include <surface/surface/surface_viewer.hpp>

//libv
#include <libv/arg/arg.hpp>

#include <iostream>

//#include <libv/color/space.hpp>
//#include <libv/noise/noise.hpp>
//#include <libv/utility/random/xoroshiro128.hpp>
//#include <libv/utility/random/uniform_distribution.hpp>
//#include <surface/view/camera.hpp>
//#include <libv/meta/resolve.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Surface", "Surface generation and visualization tool");

	const auto arg_config = args.optional<std::string>
			("-c", "--config")
			("config", "Path to the config file");

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

	try {
		surface::log_surface.info("Hello Surface!");

		// Change working directory
		if (std::filesystem::exists("app/surface/"))
			std::filesystem::current_path("app/surface/");
		else
			std::filesystem::current_path("../app/surface/"); // During development binary artifacts created under /bin

		// Run the game
		surface::SurfaceViewer surfaceViewer;
		surfaceViewer.execute();

	} catch (const std::exception& e) {
		surface::log_surface.fatal("Execution failed with an exception: {}", e.what());
		std::cerr << "Execution failed with an exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
