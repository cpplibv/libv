// Created by Vader on 2023.01.09..

#pragma once

// libv
#include <libv/gl/gl_fwd.hpp>
#include <libv/utility/chrono.hpp>
// pro
#include <libv/rev/engine/canvas.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class Scene {
	// Camera camera;
	// ParticleSet particles;
	// std::vector<ParticleEffect> particles;

	// std::vector<Layer> layers;

public:
	void create(libv::gl::GL& gl) {

	}
	void destroy(libv::gl::GL& gl) {

	}
	// void render(libv::gl::GL& gl) {
	//
	// }
	void render(libv::rev::Canvas&, libv::gl::GL&) {

	}
	void async_update(libv::time_duration deltaTime) {

	}
	// void update(libv::time_duration deltaTime) {
	//
	// }
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
