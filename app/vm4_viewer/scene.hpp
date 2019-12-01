// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue_fwd.hpp>
#include <libv/glr/uniform.hpp>
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

	bool display_BS = true;
	bool display_AABB = true;

	libv::glr::Mesh debug_cube;
	libv::glr::Mesh debug_gizmo;
	libv::glr::Mesh debug_sphere;
	libv::glr::Program debug_shader;
	libv::glr::Uniform_vec4f debug_uniform_color;
	libv::glr::Uniform_mat4f debug_uniform_MVPmat;

public:
	Scene();
	void render(libv::glr::Queue& gl, libv::vec2f canvas_size);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
