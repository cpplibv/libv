// Project: libv, File: app/space/canvas.cpp

// hpp
#include <space/canvas.hpp>
// libv
#include <libv/glr/queue.hpp>
// std
#include <cassert>
// pro
#include <space/game_session.hpp>
#include <space/renderer.hpp>
#include <space/universe/universe.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------

RendererCommandArrow::ArrowStyle convert_to_arrow_style(Fleet::CommandType type) {
	//	const vec4 base_color_c = vec4(0.48, 0.65, 0.70, 0.5);
	//	const vec4 base_color_r = vec4(0.80, 0.30, 0.30, 0.5);
	//	const vec4 base_color_g = vec4(0.42, 0.75, 0.40, 0.5);
	//	const vec4 base_color_p = vec4(0.38, 0.38, 0.40, 0.3);
	switch (type) {
//	case Fleet::CommandType::attack:   return {libv::vec4f(1, 0, 0, 1), libv::vec4f(0.7, 0.4, 0, 1)};
//	case Fleet::CommandType::movement: return {libv::vec4f(0, 1, 0, 1), libv::vec4f(0, 0, 1, 1)};
	case Fleet::CommandType::attack:
		return {libv::vec4f(0.80f, 0.30f, 0.30f, 0.5f), libv::vec4f(0.80f, 0.30f, 0.30f, 0.5f)};
	case Fleet::CommandType::movement:
		return {libv::vec4f(0.48f, 0.65f, 0.70f, 0.5f), libv::vec4f(0.48f, 0.65f, 0.70f, 0.5f)};
	}

	assert(false && "Invalid Fleet::CommandType enum value");
	return {libv::vec4f(1, 0, 1, 1), libv::vec4f(1, 0, 1, 1)};
}

// -------------------------------------------------------------------------------------------------

SpaceCanvas::SpaceCanvas(Renderer& renderer, GameSession& game_session, bool main_canvas) :
		main_canvas(main_canvas),
		game_session(game_session),
		universe(game_session.universe),
		playout(game_session.playout),
//		camera(camera),
		screen_picker(camera.picker({100, 100})),
		// <<< screen_picker ctor: This line is wrong, canvas_size is not initialized at this point
		//			Component shall not receive any event before onLayout gets called
		renderer(renderer) {

	camera.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});
//	screen_picker = camera.picker(?, ?); // <<< ?
}

void SpaceCanvas::update(libv::ui::time_duration delta_time) {
	const auto dtf = static_cast<float>(delta_time.count());
	angle = std::fmod(angle + 5.0f * dtf, 360.0f);
	time += dtf;
	global_time += dtf;

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

	if (main_canvas)
		// <<< P1: Game_session update shouldnt come from the canvas, or should it?
		game_session.update(delta_time);
}

void SpaceCanvas::render(libv::glr::Queue& gl) {
	renderer.prepare_for_render(gl);

	// NOTE: screen_picker update has to be placed around render, as canvas_size is only set after layout
	// TODO P1: Move screen_picker update into a layout post hook so mouse event use the update value
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
//	gl.state.polygonModeLine();

	gl.projection = camera.projection(canvas_size);
	gl.view = camera.view();
	gl.model = libv::mat4f::identity();

	// --- Render EditorBackground/Sky ---

	if (!main_canvas) {
		const auto s2_guard = gl.state.push_guard();
		// Clear the depth data for the background of the mini display
		gl.state.depthFunctionAlways();
		renderer.editorBackground.render(gl, canvas_size);
	}

	// --- Render Opaque ---

	for (const auto& fleet : universe.fleets) {
		const auto m_guard = gl.model.push_guard();
		gl.model.translate(fleet.position);
		gl.model.scale(0.2f);

		const auto isSelected = universe.selectedFleetIDList.contains(fleet.id);
		renderer.fleet.render(gl, renderer.resource_context.uniform_stream, isSelected);
	}

	// --- Render EditorBackground/Sky ---

	if (main_canvas) {
		const auto s2_guard = gl.state.push_guard();
		// No need to write depth data for the main background
		gl.state.disableDepthMask();
		renderer.editorBackground.render(gl, canvas_size);
	}

	// --- Render Transparent ---

	renderer.arrow.add_debug_spiral();
	renderer.arrow.add_debug_view01();
	renderer.arrow.add_debug_view02();
	renderer.arrow.add_debug_view03();
	renderer.arrow.add_debug_view04();
	renderer.arrow.add_debug_view05();

	for (const auto& fleet : universe.fleets) {
		renderer.arrow.restart_chain(fleet.animation_offset());

		auto prev = fleet.position;
		for (const auto& command : fleet.commands) {
			renderer.arrow.add_arrow(prev, command.target, convert_to_arrow_style(command.type));
			prev = command.target;
		}
	}

	renderer.arrow.render(gl, canvas_size, renderer.resource_context.uniform_stream);

	// --- Render UI/HUD ---

	{
		{ // Grid
			const auto s2_guard = gl.state.push_guard();
			gl.state.disableDepthMask();

			renderer.editorGrid.render(gl, renderer.resource_context.uniform_stream);
		}

		{ // Camera orbit point
			const auto s2_guard = gl.state.push_guard();
			gl.state.disableDepthMask();

			const auto m_guard = gl.model.push_guard();
			gl.model.translate(camera.orbit_point());
			gl.model.scale(0.2f);
			renderer.gizmo.render(gl, renderer.resource_context.uniform_stream);
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
			gl.view = camera.orientation_view().translate(-1, 0, 0);
			gl.model.scale(orientation_gizmo_size * 0.5f);

			renderer.gizmo.render(gl, renderer.resource_context.uniform_stream);
		}
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
