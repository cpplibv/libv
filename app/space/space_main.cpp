// Project: libv, File: app/space/space_main.cpp

// std
#include <filesystem>
#include <iostream>
// pro
#include <space/game_instance.hpp>
#include <space/log.hpp>


// -------------------------------------------------------------------------------------------------

// TODO P1: Shortcut to save camera position and reload it upon restart
//          > Requires persistence
// TODO P1: Auto runtime hook option for random uniform variables
// TODO P1: Persist auto runtime hook options
//          > Requires persistence

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	(void) argc;
	(void) argv;

	libv::logger_stream.deny_below("libv.gl", libv::Logger::Severity::Info);
	libv::logger_stream.deny_below("libv.frame", libv::Logger::Severity::Info);
	libv::logger_stream.deny_below("libv.rev", libv::Logger::Severity::Info);
	libv::logger_stream.deny_below("libv.ui", libv::Logger::Severity::Info);

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	try {
		space::log_space.info("Hello Space!");

		// Change working directory
		if (std::filesystem::exists("app/space/"))
			std::filesystem::current_path("app/space/");
		else
			std::filesystem::current_path("../app/space/"); // During development artifact created under /bin

		// Run the game
		space::GameInstance game;
		game.enterSinglePlayer();
		game.execute();

	} catch (const std::exception& e) {
		space::log_space.fatal("Execution failed with an exception: {}", e.what());
		std::cerr << "Execution failed with an exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
