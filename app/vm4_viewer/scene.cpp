// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/scene.hpp>
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
	debug_cube(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw),
	debug_gizmo(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw),
	debug_grid(libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw),
	debug_sphere(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw) {

	debug_shader.vertex(R"(
		#version 330 core

		layout(location = 0) in vec3 vertexPosition;

		uniform mat4 MVPmat;

		void main() {
			gl_Position = MVPmat * vec4(vertexPosition, 1);
		}
	)");
	debug_shader.fragment(R"(
		#version 330 core

		out vec4 output;

		uniform vec4 color;

		void main() {
			output = color;
		}
	)");
	debug_shader.assign(debug_uniform_color, "color");
	debug_shader.assign(debug_uniform_MVPmat, "MVPmat");

	{
		auto pos = debug_cube.attribute(attribute_position);
		auto index = debug_cube.index();
		libv::glr::generateCube(pos, libv::glr::ignore, libv::glr::ignore, index);
	}

	{
		auto pos = debug_gizmo.attribute(attribute_position);
		auto index = debug_gizmo.index();
		libv::glr::generateCube(pos, libv::glr::ignore, libv::glr::ignore, index);
	}

	{
		auto pos = debug_grid.attribute(attribute_position);
		auto index = debug_grid.index();
		libv::glr::generateGrid(25, 25, pos, libv::glr::ignore, index);
	}

	{
		auto pos = debug_sphere.attribute(attribute_position);
		auto index = debug_sphere.index();
		libv::glr::generateSpherifiedCube(6, pos, libv::glr::ignore, libv::glr::ignore, index);
	}
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
	camera.rotateZ(libv::to_rad(45.f));
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
		model->render(gl);
	}

	if (model && display_AABB) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.state.disableCullFace();
		gl.state.polygonModeLine();
		gl.model.translate((model->vm4.AABB_max + model->vm4.AABB_min) * 0.5f);
		gl.model.scale(libv::vec::abs(model->vm4.AABB_max - model->vm4.AABB_min) * 0.5f);

		gl.program(debug_shader);
		gl.uniform(debug_uniform_MVPmat, gl.mvp());
		gl.uniform(debug_uniform_color, libv::vec4f(0.3f, 1.0f, 0.3f, 0.4f));
		gl.render(debug_cube);
	}

	if (model && display_grid) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.program(debug_shader);
		gl.uniform(debug_uniform_MVPmat, gl.mvp());
		gl.uniform(debug_uniform_color, libv::vec4f(0.8f, 0.8f, 0.8f, 0.8f));
		gl.render(debug_grid);
	}

	if (model && display_BS) {
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.state.disableCullFace();
		gl.state.polygonModeLine();
		gl.model.translate(model->vm4.BS_origin);
		gl.model.scale(model->vm4.BS_radius);

		gl.program(debug_shader);
		gl.uniform(debug_uniform_MVPmat, gl.mvp());
		gl.uniform(debug_uniform_color, libv::vec4f(0.3f, 0.3f, 1.0f, 0.4f));
		gl.render(debug_sphere);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
