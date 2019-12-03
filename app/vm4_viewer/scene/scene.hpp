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
#include <vm4_viewer/scene/camera.hpp>
#include <vm4_viewer/scene/light.hpp>
#include <vm4_viewer/scene/model.hpp>
#include <vm4_viewer/scene/shader_line.hpp>
#include <vm4_viewer/scene/shader_model.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Scene {
//	std::unique_ptr<Node> root;

//	std::optional<app::Model> environment;
	std::optional<app::Model> model;
	app::Camera camera;

	Light sun;

	bool display_BS = true;
	bool display_AABB = true;
	bool display_grid = true;

	libv::glr::Mesh mesh_AABB;
	libv::glr::Mesh mesh_gizmo;
	libv::glr::Mesh mesh_grid;
	libv::glr::Mesh mesh_BS;

	app::ShaderLine line_shader;
	app::ShaderModel model_shader;

	static constexpr libv::vec4f color_AABB = {0.3f, 1.0f, 0.3f, 0.4f};
	static constexpr libv::vec4f color_gizmo = {1.0f, 1.0f, 1.0f, 1.0f};
	static constexpr libv::vec4f color_grid = {0.8f, 0.8f, 0.8f, 0.8f};
	static constexpr libv::vec4f color_BS = {0.3f, 0.3f, 1.0f, 0.4f};

public:
	Scene();
	void focus_camera();
	void reset_camera();
	void render(libv::glr::Queue& gl, libv::vec2f canvas_size);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
