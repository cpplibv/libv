// Project: libv, File: app/space/view/canvas.cpp

// hpp
#include <space/view/canvas.hpp>
// libv
#include <libv/glr/framebuffer.hpp>
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
		renderTarget({100, 100}, 4),
		postProcessing(renderer.resource_context.shader_loader, {100, 100}),
		// <<< screen_picker ctor: This line is wrong, canvas_size is not initialized at this point
		//			Component shall not receive any event before onLayout gets called
		// <<< renderTarget ctor: ^same
		// <<< postProcessing ctor: ^same
		renderer(renderer) {

	camera.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});
//	screen_picker = camera.picker(?, ?); // <<< ?
	postProcessing.vignetteIntensity(0.15f);
}

void SpaceCanvas::attach() {
	if (main_canvas)
		focus();
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

void SpaceCanvas::render(libv::glr::Queue& glr) {
	renderer.prepare_for_render(glr);

	// NOTE: screen_picker update has to be placed around render, as canvas_size is only set after layout
	// TODO P1: Move screen_picker update into a layout post hook so mouse event use the update value
	screen_picker = camera.picker(canvas_size);
	renderTarget.size(canvas_size.cast<int32_t>());
	postProcessing.size(canvas_size.cast<int32_t>());
	//

	const auto s_guard = glr.state.push_guard();

	glr.state.enableDepthTest();
	glr.state.depthFunctionLess();
	glr.state.enableDepthMask();

	glr.state.enableBlend();
	glr.state.blendSrc_SourceAlpha();
	glr.state.blendDst_One_Minus_SourceAlpha();

	glr.state.cullBackFace();
	glr.state.enableCullFace();
	glr.state.frontFaceCCW();

	glr.state.clipPlanes(0);
	glr.state.polygonModeFill();
//	gl.state.polygonModeLine();

	glr.projection = camera.projection(canvas_size);
	glr.view = camera.view();
	glr.model = libv::mat4f::identity();

	glr.framebuffer_draw(renderTarget.framebuffer());

	glr.clearColor();
	glr.clearDepth();

	// --- Render EditorBackground/Sky ---

	if (!main_canvas) {
		const auto s2_guard = glr.state.push_guard();
		// Clear the depth data for the background of the mini display
		glr.state.depthFunctionAlways();
		renderer.editorBackground.render(glr, canvas_size);
	}

	// --- Render Opaque ---

	for (const auto& fleet : universe.fleets) {
		const auto m_guard = glr.model.push_guard();
		glr.model.translate(fleet.position);
		glr.model.scale(0.2f);

		if (!fleet.commands.empty()) {
			const auto direction = normalize(fleet.commands.front().target - fleet.position);

			float angle = std::atan2(direction.y, direction.x);
			float angleZ = -std::asin(direction.z);

			angle += libv::pi * 0.5f; // <<< Model is incorrectly oriented

			glr.model.rotate(libv::Radian(angle), libv::vec3f{0.0f, 0.0f, 1.0f});
			glr.model.rotate(libv::Radian(angleZ), libv::vec3f{0.0f, 1.0f, 0.0f});
		}

		const auto isSelected = universe.selectedFleetIDList.contains(fleet.id);
		renderer.fleet.render(glr, renderer.resource_context.uniform_stream, isSelected);
	}

	// --- Render EditorBackground/Sky ---

	if (main_canvas) {
		const auto s2_guard = glr.state.push_guard();
		// No need to write depth data for the main background
		glr.state.disableDepthMask();
		renderer.editorBackground.render(glr, canvas_size);
	}

	// --- Render Transparent ---

//	renderer.arrow.add_debug_spiral();
//	renderer.arrow.add_debug_view01();
//	renderer.arrow.add_debug_view02();
//	renderer.arrow.add_debug_view03();
//	renderer.arrow.add_debug_view04();
//	renderer.arrow.add_debug_view05();

	for (const auto& fleet : universe.fleets) {
		renderer.arrow.restart_chain(fleet.animation_offset());

		auto prev = fleet.position;
		for (const auto& command : fleet.commands) {
			renderer.arrow.add_arrow(prev, command.target, convert_to_arrow_style(command.type));
			prev = command.target;
		}
	}

	renderer.arrow.render(glr, canvas_size, renderer.resource_context.uniform_stream);

	// --- Render Debug shapes ---
	{
		const auto s2_guard = glr.state.push_guard();
		glr.state.disableDepthMask();
//		gl.state.polygonModeLine();
//		for (int i = 0 ; i < 20 ; ++i) {
//			const auto fi = static_cast<float>(i);
//			for (int j = 0 ; j < 20 ; ++j) {
//				const auto fj = static_cast<float>(j);
//				add_debug_sphere({-fi, -fj, 0.4f}, 0.4f, {fi / 20.0f, fj / 20.0f, 0, 0.8f}, i, j);
//			}
//		}
		renderer.debug.render(glr, renderer.resource_context.uniform_stream);
		renderer.debug.spheres.clear();
	}

	// --- Render UI/HUD ---

	{
		{ // Grid
			const auto s2_guard = glr.state.push_guard();
			glr.state.disableDepthMask();
			renderer.editorGrid.render(glr, renderer.resource_context.uniform_stream);
		}

		{ // Camera orbit point
			const auto s2_guard = glr.state.push_guard();
			glr.state.disableDepthMask();

			const auto m_guard = glr.model.push_guard();
			glr.model.translate(camera.orbit_point());
			glr.model.scale(0.2f);
			renderer.gizmo.render(glr, renderer.resource_context.uniform_stream);
		}

		{ // Camera orientation gizmo in top right
			const auto s2_guard = glr.state.push_guard();
			glr.state.disableDepthTest();
			glr.state.disableDepthMask();

			const auto p_guard = glr.projection.push_guard();
			const auto v_guard = glr.view.push_guard();
			const auto m_guard = glr.model.push_guard();

			const auto orientation_gizmo_size = 64.f; // The axes of the gizmo will be half of this size
			const auto orientation_gizmo_margin = 4.f;

			glr.projection = libv::mat4f::ortho(
					-canvas_size + orientation_gizmo_size * 0.5f + orientation_gizmo_margin,
					canvas_size,
					-orientation_gizmo_size,
					+orientation_gizmo_size);
			glr.view = camera.orientation_view().translate(-1, 0, 0);
			glr.model.scale(orientation_gizmo_size * 0.5f);

			renderer.gizmo.render(glr, renderer.resource_context.uniform_stream);
		}
	}

	const auto& mainTexture = renderTarget.resolve(glr);
	postProcessing.pass(glr, mainTexture);

//	glr.framebuffer_draw_default();
}

// -------------------------------------------------------------------------------------------------
// TODO P5: Implement style flag modes

void SpaceCanvas::add_debug_point(libv::vec3f a, libv::vec4f color, StyleFlag mode) {
	(void) mode;
	renderer.debug.points.emplace_back(a, color);
}

void SpaceCanvas::add_debug_line(libv::vec3f a, libv::vec3f b, libv::vec4f color, StyleFlag mode) {
	(void) mode;
	renderer.debug.lines.emplace_back(a, b, color);
}

void SpaceCanvas::add_debug_triangle(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec4f color, StyleFlag mode) {
	(void) mode;
	renderer.debug.triangles.emplace_back(a, b, c, color);
}

void SpaceCanvas::add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count, int segment_count, StyleFlag mode) {
	(void) mode;
	renderer.debug.spheres.emplace_back(center, radius, color, ring_count, segment_count);
}

void SpaceCanvas::add_debug_frustum(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec3f d, libv::vec3f e, libv::vec4f color_wire, libv::vec4f color_sides, StyleFlag mode) {
	(void) mode;
	renderer.debug.frustums.push_back(RendererDebug::Frustum{{a, b, c, d, e}, color_sides, color_wire});
}

void SpaceCanvas::add_debug_quad(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec3f d, libv::vec4f color, StyleFlag mode) {
	(void) mode;
	renderer.debug.quads.emplace_back(a, b, c, d, color);
}

void SpaceCanvas::clear_debug_shapes() {
	renderer.debug.points.clear();
	renderer.debug.lines.clear();
	renderer.debug.triangles.clear();
	renderer.debug.quads.clear();
	renderer.debug.frustums.clear();
	renderer.debug.spheres.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace app
