// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/glr/queue_fwd.hpp>
// std
#include <optional>
// pro
#include <vm4_viewer/camera.hpp>
#include <vm4_viewer/model.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Scene {
//	std::optional<app::Model> environment;
	std::optional<app::Model> model;
	app::Camera camera;

	void render(libv::glr::Queue& gl, libv::vec2f canvas_size);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
