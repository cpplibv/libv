// Project: libv, File: app/vm4_viewer/main.cpp

// libv
#include <libv/utility/last_write_time.hpp>
// ext
#include <fmt/chrono.h>
#include <fmt/ostream.h>
// std
#include <filesystem>
#include <iostream>
#include <chrono>
// pro
#include <vm4_viewer/config.hpp>
#include <vm4_viewer/frame.hpp>
#include <vm4_viewer/log.hpp>


// -------------------------------------------------------------------------------------------------

// TODO P5: app.vm4_viewer: show model wireframe (GS based wireframe)
// TODO P5: app.vm4_viewer: vm4 LOD support
// TODO P5: app.vm4_viewer: pointless loading bar
// TODO P5: app.vm4_viewer: thumbnail support, and auto generation (async operation)
// TODO P5: app.vm4_viewer: display vm4 node hierarchy
// TODO P5: app.vm4_viewer: show material information
// TODO P5: app.vm4_viewer: show model texture information
// TODO P5: app.vm4_viewer: show model textured
// TODO P5: app.vm4_viewer: vm4 slot/attachment support
// TODO P5: app.vm4_viewer: save model
// TODO P5: app.vm4_viewer: skybox
// TODO P5: app.vm4_viewer: animations
// TODO P5: app.vm4_viewer: lighting
// TODO P5: app.vm4_viewer: multiple light
// TODO P5: app.vm4_viewer: shaders
// TODO P5: app.vm4_viewer: Forward+
// TODO P5: app.vm4_viewer: AA
// TODO P5: app.vm4_viewer: full game render engine (what ?!)
// TODO P5: app.vm4_viewer: billboard LOD texture

// -------------------------------------------------------------------------------------------------

static constexpr auto DEFAULT_CONFIG_FILENAME = "app_vm4_viewer_config.json";
static constexpr auto DEFAULT_PROJECT_FILENAME = "vm4_viewer_default.json";
static constexpr auto DEFAULT_PROJECT_FOLDER = "project";

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	std::cout << libv::logger_stream;
//	libv::logger_stream.deny_below(libv::Logger::Severity::Info);

	const auto path = std::filesystem::path(argv[0]);
	const auto path_bin = path.filename();
	const auto path_dir = path.parent_path();
	const auto config_filename = std::filesystem::path(DEFAULT_CONFIG_FILENAME);
	const auto config_path = path_dir / config_filename;
	const auto lwt = libv::last_write_time(path);

	app::log_app.info("Current path  {}", std::filesystem::current_path().generic_string());
	app::log_app.info("Executable    {}/{}", path_dir.generic_string(), path_bin.generic_string());
	app::log_app.info("Last modified {:%Y.%m.%d %H:%M:%S}", lwt);
//	app::log_app.info("Last modified {:%Y.%m.%d %H:%M:%S}", fmt::localtime(std::chrono::system_clock::to_time_t(lwt)));

	app::ConfigViewer config(config_path);

	{
		app::VM4ViewerFrame frame{config};
		frame.show();
		frame.join();
	}

	return EXIT_SUCCESS;
}
