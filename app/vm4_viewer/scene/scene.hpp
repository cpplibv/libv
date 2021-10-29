// Project: libv, File: app/vm4_viewer/scene/scene.hpp

#pragma once

// libv
#include <libv/glr/fwd.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/uniform.hpp>
// std
#include <optional>
// pro
#include <vm4_viewer/scene/camera.hpp>
#include <vm4_viewer/scene/camera_2.hpp>
#include <vm4_viewer/scene/light.hpp>
#include <vm4_viewer/scene/model.hpp>
#include <vm4_viewer/scene/shader_color.hpp>
#include <vm4_viewer/scene/shader_line.hpp>
#include <vm4_viewer/scene/shader_model.hpp>
#include <vm4_viewer/scene/shader_grid.hpp>
#include <vm4_viewer/shader_loader.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Scene {
//	std::unique_ptr<Node> root;

//	std::optional<app::Model> environment;
	std::optional<app::Model> model;
//	app::Camera camera;
	app::Camera2 camera;
//	app::Camera2 camera2;

	app::Light sun;

	app::ShaderColor shader_color;
	app::ShaderLine shader_line;
	app::ShaderModel shader_model;
	app::ShaderGrid shader_grid;

	static constexpr libv::vec4f color_AABB = {0.3f, 1.0f, 0.3f, 0.4f};
	static constexpr libv::vec4f color_BS = {0.3f, 0.3f, 1.0f, 0.4f};
	static constexpr libv::vec4f color_gizmo = {1.0f, 1.0f, 1.0f, 1.0f};
	static constexpr libv::vec4f color_grid = {0.8f, 0.8f, 0.8f, 0.15f};

	bool display_AABB = true;
	bool display_BS = true;
	bool display_gizmo = false;
	bool display_grid = true;

	libv::glr::Mesh mesh_AABB;
	libv::glr::Mesh mesh_BS;
	libv::glr::Mesh mesh_gizmo;
	libv::glr::Mesh mesh_grid;
	libv::glr::Mesh mesh_temp;

public:
	Scene(ShaderLoader& shader_loader);
	void focus_camera();
	void reset_camera();

public:
	void render(libv::glr::Queue& gl, libv::vec2f canvas_size);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
