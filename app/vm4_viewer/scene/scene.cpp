// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

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


namespace app {

// -------------------------------------------------------------------------------------------------

Scene::Scene() :
	mesh_AABB(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw),
	mesh_gizmo(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw),
	mesh_grid(libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw),
	mesh_BS(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw),

	line_shader("app/vm4_viewer/res/line_shader.vs", "app/vm4_viewer/res/line_shader.fs"),
	model_shader("app/vm4_viewer/res/model_shader.vs", "app/vm4_viewer/res/model_shader.fs") {

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

	sun.type = LightType::dir;
	sun.direction = libv::vec3f(2, 1, 4).normalize();
	sun.diffuse = libv::vec3f(1, 1, 1);
	sun.specular = libv::vec3f(1, 1, 1);
}

void Scene::focus_camera() {
	if (!model)
		return;

	camera.position(model->vm4.BS_origin);
	camera.set_zoom(-model->vm4.BS_radius);
}

void Scene::reset_camera() {
	if (!model)
		return;

	camera.rotation({0, 0, 0});
	camera.position(model->vm4.BS_origin);
	camera.set_zoom(-model->vm4.BS_radius);
	camera.rotateX(libv::to_rad(45.f));
	camera.rotateZ(libv::to_rad(-135.f));
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

	if (model) {
		model->render(gl, model_shader);
	}

	if (model && display_AABB) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.state.disableCullFace();
		gl.state.polygonModeLine();
		gl.model.translate((model->vm4.AABB_max + model->vm4.AABB_min) * 0.5f);
		gl.model.scale(libv::vec::abs(model->vm4.AABB_max - model->vm4.AABB_min) * 0.5f);

		gl.program(line_shader);
		gl.uniform(line_shader.uniform_matMVP, gl.mvp());
		gl.uniform(line_shader.uniform_color, color_AABB);
		gl.render(mesh_AABB);
	}

	if (model && display_grid) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.program(line_shader);
		gl.uniform(line_shader.uniform_matMVP, gl.mvp());
		gl.uniform(line_shader.uniform_color, color_grid);
		gl.render(mesh_grid);
	}

	if (model && display_BS) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.state.disableCullFace();
		gl.state.polygonModeLine();
		gl.model.translate(model->vm4.BS_origin);
		gl.model.scale(model->vm4.BS_radius);

		gl.program(line_shader);
		gl.uniform(line_shader.uniform_matMVP, gl.mvp());
		gl.uniform(line_shader.uniform_color, color_BS);
		gl.render(mesh_BS);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
