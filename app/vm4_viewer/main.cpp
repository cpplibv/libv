// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// libv
#include <libv/utility/generic_path.hpp>
// std
#include <iostream>
// pro
#include <vm4_viewer/config.hpp>
#include <vm4_viewer/frame.hpp>
#include <vm4_viewer/log.hpp>


// -------------------------------------------------------------------------------------------------

// TODO P5: app.vm4_viewer: list models
// TODO P5: app.vm4_viewer: load model
// TODO P5: app.vm4_viewer: import model
// TODO P5: app.vm4_viewer: display statistics
// TODO P5: app.vm4_viewer: show model grey
// TODO P5: app.vm4_viewer: show model wireframe
// TODO P5: app.vm4_viewer: vm4 LOD support
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
// TODO P5: app.vm4_viewer: AA
// TODO P5: app.vm4_viewer: full game render engine (what ?!)
// TODO P5: app.vm4_viewer: texture LOD.scale change

// -------------------------------------------------------------------------------------------------

const std::string DEFAULT_CONFIG_FILENAME = "app_vm4_viewer_config.json";
const std::string DEFAULT_PROJECT_FILENAME = "vm4_viewer_default.json";
const std::string DEFAULT_PROJECT_FOLDER = "project";

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	std::cout << libv::logger;

	(void) argc;
	(void) argv;

	const auto path = std::filesystem::path(argv[0]);
	const auto path_bin = path.filename();
	const auto path_dir = path.parent_path();
	const auto config_filename = std::filesystem::path(DEFAULT_CONFIG_FILENAME);
	const auto config_path = path_dir / config_filename;

	app::log_app.info("Started {}/{}", libv::generic_path(path_dir), libv::generic_path(path_bin));

	app::ConfigViewer config(config_path);

	{
		app::VM4ViewerFrame frame{config};
		frame.show();
		frame.join();
	}

	return EXIT_SUCCESS;
}
