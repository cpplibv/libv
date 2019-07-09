// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/vm4/model.hpp>
// pro
#include <vm4_viewer/camera.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Scene {
	libv::vm4::Model model;
	app::Camera camera;
	//	libv::vm4::Model environment;
	//	libv::glr::Program shader;

//	void changeModel() {
		//	libv::erase(config.recent_models, model);
		//	config.recent_models.emplace_back(model);
//
//	}
//		model.materials[0].name;
//		model.materials[0].properties[0];
//		model.materials[0].shader;
//		model.vertices[0].
};

// -------------------------------------------------------------------------------------------------

} // namespace app
