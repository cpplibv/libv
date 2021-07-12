// Project: libv, File: app/space/space_main.cpp, Author: Császár Mátyás [Vader]

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

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	try {
		app::log_space.info("Hello Space!");

		std::filesystem::current_path("app/space/"); // Change working directory

		app::GameInstance game;
		game.enter_single_player();
		game.execute();

	} catch (const std::exception& e) {
		app::log_space.fatal("Execution failed with an exception: {}", e.what());
		std::cerr << "Execution failed with an exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
