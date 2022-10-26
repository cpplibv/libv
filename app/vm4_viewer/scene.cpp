// Project: libv, File: app/vm4_viewer/scene.cpp

// hpp
#include <vm4_viewer/scene/scene.hpp>
// libv
#include <libv/glr/procedural/cube.hpp>
#include <libv/glr/procedural/grid.hpp>
#include <libv/glr/procedural/ignore.hpp>
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/queue.hpp>
#include <libv/math/angle.hpp>
// pro
#include <vm4_viewer/attribute.hpp>
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/ui/keyboard.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

Scene::Scene(ShaderLoader& shader_loader) :
	shader_color(shader_loader,
			libv::gl::ShaderType::Vertex, "color.vs",
			libv::gl::ShaderType::Fragment, "color.fs"
	),
	shader_line(shader_loader,
			libv::gl::ShaderType::Vertex, "line.vs",
			libv::gl::ShaderType::Fragment, "line.fs"
	),
	shader_model(shader_loader,
			libv::gl::ShaderType::Vertex, "model.vs",
			libv::gl::ShaderType::Fragment, "model.fs"
	),
	shader_grid(shader_loader,
			libv::gl::ShaderType::Vertex, "grid.vs",
			libv::gl::ShaderType::Fragment, "grid.fs"
	),

	mesh_AABB(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw),
	mesh_BS(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw),
	mesh_gizmo(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw),
	mesh_grid(libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw),
	mesh_temp(libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw) {

	{
		auto pos = mesh_AABB.attribute(attribute_position);
		auto index = mesh_AABB.index();
		libv::glr::generateCube(pos, libv::glr::ignore, libv::glr::ignore, index);
	}

	{
		auto pos = mesh_gizmo.attribute(attribute_position);
		auto index = mesh_gizmo.index();
		libv::glr::generateCube(pos, libv::glr::ignore, libv::glr::ignore, index);
	}

	{
		auto pos = mesh_grid.attribute(attribute_position);
		auto index = mesh_grid.index();
		libv::glr::generateGrid(25, 25, pos, libv::glr::ignore, index);
	}

	{
		auto pos = mesh_BS.attribute(attribute_position);
		auto index = mesh_BS.index();
		libv::glr::generateSpherifiedCube(6, pos, libv::glr::ignore, libv::glr::ignore, index);
	}

	{
		// TODO P0: Remove
		ui::Keyboard keyboard{mesh_temp};
		keyboard.foo();
	}

//	sun.type = LightType::point;
//	sun.position = libv::vec3f(4, 5, 8);

//	sun.type = LightType::spot;
//	sun.position = libv::vec3f(4, 5, 8);
//	sun.direction = libv::vec3f(-2, -1, -4).normalize();

	sun.type = LightType::directional;
	sun.direction = libv::vec3f(-2, -1, -4).normalize();

	sun.ambient = libv::vec3f(0.05f, 0.05f, 0.05f);
	sun.diffuse = libv::vec3f(1, 1, 1);
	sun.specular = libv::vec3f(1, 1, 1);
}

void Scene::focus_camera() {
	if (!model)
		return;

	camera.position(model->vm4.BS_origin);
	camera.orbit_to(model->vm4.BS_radius);
}

void Scene::reset_camera() {
	if (!model)
		return;

	camera.rotation({0, 0, 0});
	camera.position(model->vm4.BS_origin);
	camera.orbit_to(model->vm4.BS_radius);
	camera.rotate_x(libv::to_rad(45.f));
	camera.rotate_z(libv::to_rad(-135.f));
}

void Scene::render(libv::glr::Queue& gl, libv::vec2f canvas_size) {
	const auto guard_p = gl.projection.push_guard();
	const auto guard_v = gl.view.push_guard();
	const auto guard_m = gl.model.push_guard();
	const auto guard_s = gl.state.push_guard();

	gl.projection = camera.projection(canvas_size);
	gl.view = camera.view();
	gl.model = libv::mat4f::identity();

	gl.state.enableBlend();
	gl.state.blendSrc_SourceAlpha();
	gl.state.blendDst_One_Minus_SourceAlpha();

	//gl.state.disableCullFace();
	gl.state.enableCullFace();
	gl.state.frontFaceCCW();
	gl.state.cullBackFace();

	gl.state.polygonModeFill();
	//gl.state.polygonModeLine();

	gl.state.enableDepthTest();
	gl.state.depthFunctionLess();

	if (!model) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.program(shader_line.program);
		gl.uniform(shader_line.uniform.matMVP, gl.mvp());
		gl.uniform(shader_line.uniform.color, color_grid);
		gl.render(mesh_temp);
	}

	if (!model)
		return;

	{
		gl.program(shader_model.program);
		shader_model.uniform.sun.set(gl, sun);
		gl.uniform(shader_model.uniform.eyeW, gl.eye());
		model->render(gl, shader_model);
	}

	if (display_AABB) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.state.disableCullFace();
		gl.state.polygonModeLine();
		gl.model.translate((model->vm4.AABB_max + model->vm4.AABB_min) * 0.5f);
		gl.model.scale(libv::vec::abs(model->vm4.AABB_max - model->vm4.AABB_min) * 0.5f);

		gl.program(shader_line.program);
		gl.uniform(shader_line.uniform.matMVP, gl.mvp());
		gl.uniform(shader_line.uniform.color, color_AABB);

		gl.render(mesh_AABB);
	}

	if (display_BS) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.state.disableCullFace();
		gl.state.polygonModeLine();
		gl.model.translate(model->vm4.BS_origin);
		gl.model.scale(model->vm4.BS_radius);

		gl.program(shader_line.program);
		gl.uniform(shader_line.uniform.matMVP, gl.mvp());
		gl.uniform(shader_line.uniform.color, color_BS);
		gl.render(mesh_BS);
	}

	if (display_grid) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.program(shader_grid.program);
		gl.uniform(shader_grid.uniform.matMVP, gl.mvp());
		gl.uniform(shader_grid.uniform.color, color_grid);
		gl.render(mesh_grid);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
