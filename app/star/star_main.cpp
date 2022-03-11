// Project: libv, File: app/star/star_main.cpp

// std
#include <filesystem>
#include <iostream>
// pro
#include <star/game/game_client.hpp>
#include <star/log.hpp>
#include <star/version.hpp>


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

//	libv::logger_stream.deny_below("libv.gl", libv::Logger::Severity::Info);
//	libv::logger_stream.deny_below("libv.frame", libv::Logger::Severity::Info);
//	libv::logger_stream.deny_below("libv.rev", libv::Logger::Severity::Info);
//	libv::logger_stream.deny_below("libv.ui", libv::Logger::Severity::Info);
//	libv::logger_stream.deny_below("libv.net", libv::Logger::Severity::Info);

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	try {
		star::log_star.info("Hello Space! {}", star::version);

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
