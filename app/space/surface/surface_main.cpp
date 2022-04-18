// Project: libv, File: app/space/space_main.cpp

//std
#include <filesystem>

// space
#include <space/log.hpp>
#include <space/surface/surface_viewer.hpp>

#include <iostream>

//#include <libv/color/space.hpp>
//#include <libv/noise/noise.hpp>
//#include <libv/utility/random/xoroshiro128.hpp>
//#include <libv/utility/random/uniform_distribution.hpp>
//#include <space/view/camera.hpp>
//#include <libv/meta/resolve.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	(void) argc;
	(void) argv;


	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	try {
		space::log_space.info("Hello Surface!");

		// Change working directory
		if (std::filesystem::exists("app/space/"))
			std::filesystem::current_path("app/space/");
		else
			std::filesystem::current_path("../app/space/"); // During development binary artifacts created under /bin

		// Run the game
		surface::SurfaceViewer surfaceViewer;
		surfaceViewer.execute();

	} catch (const std::exception& e) {
		space::log_space.fatal("Execution failed with an exception: {}", e.what());
		std::cerr << "Execution failed with an exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
