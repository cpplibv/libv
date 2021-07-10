// Project: libv, File: app/space/render.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/render.hpp>
// libv
#include <libv/algo/adjacent_pairs.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/math/noise/white.hpp>
//#include <libv/glr/texture.hpp>
// pro
#include <space/camera.hpp>
#include <space/command.hpp>
#include <space/playout.hpp>
#include <space/state.hpp>
//#include <space/camera_behaviour.hpp>


#include <iostream>


namespace app {

// -------------------------------------------------------------------------------------------------

Background::Background() {
	// TODO P1: Switch to blue noise once implemented
	//  		| It will not be implemented anytime soon so burn in a couple of textures from it
	const auto tex_data = libv::noise_white_2D_3uc(0x5EED, noise_size.x, noise_size.y);

	background_texture_pattern.storage(1, noise_size);
	background_texture_pattern.image(0, {0, 0}, noise_size, tex_data.data());
	background_texture_pattern.set(libv::gl::MagFilter::Nearest);
	background_texture_pattern.set(libv::gl::MinFilter::Nearest);
	background_texture_pattern.set(libv::gl::Wrap::Repeat, libv::gl::Wrap::Repeat);

	{
		auto position = mesh_background.attribute(attribute_position);
		auto index = mesh_background.index();

		position(-1, -1, 1);
		position( 1, -1, 1);
		position( 1,  1, 1);
		position(-1,  1, 1);

		index.quad(0, 1, 2, 3);
	}
}

void Background::render(libv::glr::Queue& gl, libv::vec2f canvas_size) {
	gl.program(shader.program());
	const auto bg_noise = libv::vec4f(1, 1, 1, 0) * (5.f / 255.f);
	const auto bg_color = libv::vec4f(0.098f, 0.2f, 0.298f, 1.0f);
	gl.uniform(shader.uniform().render_resolution, canvas_size);
	gl.uniform(shader.uniform().noise_scale, bg_noise);
	gl.uniform(shader.uniform().base_color, bg_color);
	gl.texture(background_texture_pattern, textureChannel_pattern);
	gl.render(mesh_background);
}

// -------------------------------------------------------------------------------------------------

CommandArrow::CommandArrow() {
	std::vector<libv::vec3f> points{{0, 0, 0}, {1, 0.5, 0.5}, {1, 1, 0}, {1, 2, 2}, {-1, -1, -1}};
	for (int i = 0; i < 60; i++) {
		const auto r = i / 30.0;
		const auto x = std::sin(libv::deg_to_rad(i * 15.0)) * r;
		const auto y = std::cos(libv::deg_to_rad(i * 15.0)) * r;
		points.emplace_back(x, y, 0);
	}
	for (int i = 60; i < 120; i++) {
		const auto r = 2.0 - (i - 60) / 30.0 * 0.5;
		const auto x = std::sin(libv::deg_to_rad(i * 15.0)) * r;
		const auto y = std::cos(libv::deg_to_rad(i * 15.0)) * r;
		const auto z = std::sin(libv::deg_to_rad((i - 60) * 30.0)) * 0.25;
		points.emplace_back(x, y, z);
	}

	draw_arrow(mesh, points);
}

void CommandArrow::draw_arrow(libv::glr::Mesh& mesh, std::vector<libv::vec3f> points) {
	if (points.size() < 2)
		return;

	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto sp_ss_tp_ts = mesh.attribute(attribute_custom0); // SegmentPosition, SegmentSize, TotalPosition, TotalSize
	auto index = mesh.index();

	float total_length = 0;
	float current_length = 0;

	libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
		total_length += (b - a).length();
	});

	libv::glr::VertexIndex i = 0;
	libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
		if (a == b) // Sanity check
			return;

		const auto length = (b - a).length();

		position(a);
		position(b);

		color0(1, 1, 1, 1);
		color0(1, 1, 1, 1);

		sp_ss_tp_ts(0, length, current_length, total_length);
		sp_ss_tp_ts(length, length, current_length, total_length);

		index.line(i + 0, i + 1);
		i += 2;

		current_length += length;
	});
}

void CommandArrow::render(libv::glr::Queue& gl, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream) const {
	// TODO P2: This will require to re upload to VAO every render
	//  		but it could be optimized if we give it a 'pretend' start offset
	//  		single uniform value that adjust the 'fake' starting point (not only the first section could have this)
	//			This also means VAO update only required on NEW command (SUPER GOOD), every normal render only changes some uniform
	//			| MAJOR ISSUE: Command arrows that move, like follow would break this system
	//				Resolution ideas: indirections?, uniform array as coordinates?, VBA update is okey?
	//			and/or Command arrows could be merged into a single VAO and use sub-meshes to render

	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = gl.mvp();
	uniforms[layout_matrices.matM] = gl.model;
	uniforms[layout_matrices.matP] = gl.projection;
	uniforms[layout_matrices.eye] = gl.eye();

	gl.program(shader_arrow.program());
	gl.uniform(std::move(uniforms));
	gl.uniform(shader_arrow.uniform().color, libv::vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	gl.uniform(shader_arrow.uniform().render_resolution, canvas_size);
	gl.uniform(shader_arrow.uniform().test_mode, global_test_mode);
	gl.uniform(shader_arrow.uniform().time, global_time);
	gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

Gizmo::Gizmo() {
	draw_gizmo_lines(mesh);
}

void Gizmo::draw_gizmo_lines(libv::glr::Mesh& mesh) {
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();

	position(0, 0, 0);
	position(1, 0, 0);
	position(0, 0, 0);
	position(0, 1, 0);
	position(0, 0, 0);
	position(0, 0, 1);

	color0(1, 0, 0, 1);
	color0(1, 0, 0, 1);
	color0(0, 1, 0, 1);
	color0(0, 1, 0, 1);
	color0(0, 0, 1, 1);
	color0(0, 0, 1, 1);

	index.line(0, 1);
	index.line(2, 3);
	index.line(4, 5);
}

void Gizmo::render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = gl.mvp();
	uniforms[layout_matrices.matM] = gl.model;
	uniforms[layout_matrices.matP] = gl.projection;
	uniforms[layout_matrices.eye] = gl.eye();

	gl.program(shader.program());
	gl.uniform(std::move(uniforms));
	gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

Grid::Grid() {
	{
		auto position = mesh_grid.attribute(attribute_position);
		auto index = mesh_grid.index();

		position(-1, -1, 0);
		position(+1, -1, 0);
		position(+1, +1, 0);
		position(-1, +1, 0);

		index.quad(0, 1, 2, 3); // Front face quad
		index.quad(0, 3, 2, 1); // Back face quad
	}
}

void Grid::render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = gl.mvp();
	uniforms[layout_matrices.matM] = gl.model;
	uniforms[layout_matrices.matP] = gl.projection;
	uniforms[layout_matrices.eye] = gl.eye();

	gl.program(shader.program());
	gl.uniform(std::move(uniforms));
	gl.render(mesh_grid);
}

// -------------------------------------------------------------------------------------------------

FleetRender::FleetRender() {
	draw_mesh(mesh);
}

void FleetRender::draw_mesh(libv::glr::Mesh& mesh) {
	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto texture0 = mesh.attribute(attribute_texture0);
	auto index = mesh.index();

	libv::glr::generateSpherifiedCube(8, position, normal, texture0, index);
//		libv::glr::generateCube(position, normal, texture0, index);
}

void FleetRender::render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = gl.mvp();
	uniforms[layout_matrices.matM] = gl.model;
	uniforms[layout_matrices.matP] = gl.projection;
	uniforms[layout_matrices.eye] = gl.eye();

	gl.program(shader.program());
	gl.uniform(shader.uniform().base_color, libv::vec4f(0.7f, 0.7f, 0.7f, 1.0f));
	gl.uniform(std::move(uniforms));
	gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

SpaceCanvas::SpaceCanvas(app::SpaceState& state, app::SpaceSession& session, app::PlayoutDelayBuffer& playout_delay_buffer, app::CameraPlayer& camera, bool main_canvas) :
		main_canvas(main_canvas),
		state(state),
		session(session),
		playout_delay_buffer(playout_delay_buffer),
		camera(camera),
		screen_picker(camera.picker({100, 100})) {
	// <<< screen_picker ctor: This line is wrong, canvas_size is not initialized at this point
	//			Component shall not receive any event before onLayout gets called
}

void SpaceCanvas::update(libv::ui::time_duration delta_time) {
	if (main_canvas) {
		playout_delay_buffer.update(state, session);
		state.update(delta_time);
	}

	const auto dtf = static_cast<float>(delta_time.count());
	angle = std::fmod(angle + 5.0f * dtf, 360.0f);
	time += dtf;
	global_time += dtf;

	// TODO P2: Value tracking UI component for debugging
//		libv::ui::value_tracker tracker(600 /*sample*/, 0.15, 0.85);
//		value_tracker(160);
//		value_tracker.pause();
//		value_tracker.resume();
//		value_tracker("camera.orbit_point", camera.orbit_point());
//		value_tracker("camera.orbit_distance", camera.orbit_distance());
//		value_tracker("camera.rotations", camera.rotations());
//		value_tracker.differential("camera.orbit_point", camera.orbit_point());
//		value_tracker.differential_focused("camera.orbit_point", camera.orbit_point(), 0.15, 0.85);
//		value_tracker.differential_focused_timed("camera.orbit_point", camera.orbit_point(), 0.15, 0.85);

	if (global_test_mode != 0) {
		test_sin_time += dtf;
		auto t = (std::sin(test_sin_time / 10.f) * 0.5f + 0.5f);
		if (global_test_mode == 1) {
			camera.pitch(-t * libv::pi_f * 0.5f);
		} else if (global_test_mode == 2) {
			t = t > 0.5f ? 1.f - t : t;
			camera.pitch(-t * libv::pi_f * 0.5f);
		} else if (global_test_mode == 3) {
			const float part = 4;
			auto t = (std::sin(test_sin_time / 10.f * part) * 0.5f + 0.5f);
			t = t > 0.5f ? 1.f - t : t;
			camera.pitch(-t * libv::pi_f * 0.5f / part);
		}
	}
}

void SpaceCanvas::render(libv::glr::Queue& gl) {
	// NOTE: Screen_picker update has to be placed around render, as canvas_size is only set after layout
	screen_picker = camera.picker(canvas_size);
	//

	const auto s_guard = gl.state.push_guard();

	gl.state.enableDepthTest();
	gl.state.depthFunctionLess();
	gl.state.enableDepthMask();

	gl.state.enableBlend();
	gl.state.blendSrc_SourceAlpha();
	gl.state.blendDst_One_Minus_SourceAlpha();

	gl.state.cullBackFace();
	gl.state.enableCullFace();
	gl.state.frontFaceCCW();

	gl.state.clipPlanes(0);
	gl.state.polygonModeFill();

	gl.projection = camera.projection(canvas_size);
	gl.view = camera.view();
	gl.model = libv::mat4f::identity();

	// --- Render Background/Sky ---

	if (!main_canvas) {
		const auto s2_guard = gl.state.push_guard();
		// Clear the depth data for the background of the mini display
		gl.state.depthFunctionAlways();
		background.render(gl, canvas_size);
	}

	// --- Render Opaque ---

	for (const auto& fleet : state.fleets) {
		const auto m_guard = gl.model.push_guard();
		gl.model.translate(fleet.position);
		gl.model.scale(0.2f);
		render_fleet.render(gl, uniform_stream);
	}

	// --- Render Background/Sky ---

	if (main_canvas) {
		const auto s2_guard = gl.state.push_guard();
		// No need to write depth data for the main background
		gl.state.disableDepthMask();
		background.render(gl, canvas_size);
	}

	// --- Render Transparent ---

	arrow.render(gl, canvas_size, uniform_stream);

	// --- Render UI/HUD ---

	{
		{ // Grid
			const auto s2_guard = gl.state.push_guard();
			gl.state.disableDepthMask();

			grid.render(gl, uniform_stream);
		}

		{ // Camera orbit point
			const auto s2_guard = gl.state.push_guard();
			gl.state.disableDepthMask();

			const auto m_guard = gl.model.push_guard();
			gl.model.translate(camera.orbit_point());
			gl.model.scale(0.2f);
			origin_gizmo.render(gl, uniform_stream);
		}

		{ // Camera orientation gizmo in top right
			const auto s2_guard = gl.state.push_guard();
			gl.state.disableDepthTest();
			gl.state.disableDepthMask();

			const auto p_guard = gl.projection.push_guard();
			const auto v_guard = gl.view.push_guard();
			const auto m_guard = gl.model.push_guard();

			const auto orientation_gizmo_size = 64.f; // The axes of the gizmo will be half of this size
			const auto orientation_gizmo_margin = 4.f;

			gl.projection = libv::mat4f::ortho(
					-canvas_size + orientation_gizmo_size * 0.5f + orientation_gizmo_margin,
					canvas_size,
					-orientation_gizmo_size,
					+orientation_gizmo_size);
			gl.view = camera.orientation().translate(-1, 0, 0);
			gl.model.scale(orientation_gizmo_size * 0.5f);

			origin_gizmo.render(gl, uniform_stream);
		}
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
