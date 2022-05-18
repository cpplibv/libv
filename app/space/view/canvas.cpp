// Project: libv, File: app/space/view/canvas.cpp

// hpp
#include <space/view/canvas.hpp>
// ext
#include <fmt/format.h>
#include <fmt/ostream.h>
// libv
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/queue.hpp>
#include <libv/math/step.hpp>
#include <libv/noise/noise.hpp>
// std
#include <cassert>
// pro
#include <space/game/game_session.hpp>
#include <space/sim/faction.hpp>
#include <space/sim/universe.hpp>
#include <space/view/canvas_control.hpp>
#include <space/view/render/renderer.hpp>


//#include <libv/log/log.hpp>
//#include <libv/color/space.hpp>
#include <libv/ctrl/controls.hpp>

#include <space/message/internal_events.hpp>
#include <libv/utility/nexus.hpp>


namespace space {

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

RendererCommandArrow::ArrowStyle convert_to_arrow_style(FleetCommandType type) {
	//	const vec4 base_color_c = vec4(0.48, 0.65, 0.70, 0.5);
	//	const vec4 base_color_r = vec4(0.80, 0.30, 0.30, 0.5);
	//	const vec4 base_color_g = vec4(0.42, 0.75, 0.40, 0.5);
	//	const vec4 base_color_p = vec4(0.38, 0.38, 0.40, 0.3);
	switch (type) {
//	case FleetCommandType::attack:   return {libv::vec4f(1, 0, 0, 1), libv::vec4f(0.7, 0.4, 0, 1)};
//	case FleetCommandType::movement: return {libv::vec4f(0, 1, 0, 1), libv::vec4f(0, 0, 1, 1)};
	case FleetCommandType::attack:
		return {libv::vec4f(0.80f, 0.30f, 0.30f, 0.5f), libv::vec4f(0.80f, 0.30f, 0.30f, 0.5f)};
	case FleetCommandType::movement:
		return {libv::vec4f(0.48f, 0.65f, 0.70f, 0.5f), libv::vec4f(0.48f, 0.65f, 0.70f, 0.5f)};
	}

	assert(false && "Invalid FleetCommandType enum value");
	return {libv::vec4f(1, 0, 1, 1), libv::vec4f(1, 0, 1, 1)};
}

//void test_fleet_trajectory(Renderer& renderer) {
//	const auto targetPosition = libv::vec3f(0, 0, 0);
//	const auto speed = 1.f;
//	auto count = 0;
//	auto success = 0;
//	const auto angleSpeed = libv::deg_to_rad(30.f) / 60.f;
//
//	for (float y = -1.f; y <= 1.f; y += 0.05f)
//		for (float x = -1.f; x <= 1.f; x += 0.05f) {
//			count++;
//			auto position = libv::vec3f(x + y * 0.0001f, y, 0);
//			auto orientation = libv::quatf::look_at({0, 1, 0}, {0, 0, 1});
//
//			for (int j = 0; j < 600; ++j) {
//
//				const auto[len, dir] = (targetPosition - position).length_and_dir();
//				const auto targetOrientation = libv::quatf::look_at(dir, libv::vec3f(0, 0, 1.f));
//
//				orientation = libv::rotate_towards(orientation, targetOrientation, angleSpeed);
//
//				if (len < speed / 60.f) {
//					const auto nextPosition = targetPosition;
//					renderer.debug.lines.emplace_back(position, nextPosition, libv::vec4f{1, 1, 0, 0.4f});
//					renderer.debug.spheres.emplace_back(libv::vec3f{x, y, 0}, 0.01f, libv::vec4f{libv::color::hue_to_rgb(1.f/3.f - j / 600.f / 3.0f), 1.f}, 3, 3);
//					success++;
//					break;
//				} else {
//					// -------------------------------------------------------------------------------------------------
//
////					float matching = abs(dot(orientation, targetOrientation)); //[0,1] similar
//					float matching = std::max(0.f, dot(orientation.forward(), targetOrientation.forward())); //[0,1] similar
//
//					matching = std::max(0.f, matching * 1.5f - 0.5f);
////					const auto matchingPow = std::pow(matching, 4.f);
////					const auto s = 0.2f * std::min(1.f, len) * speed + 0.8f * matchingPow * speed;
//					const auto s = std::min(1.f, len + matching) * speed * matching;
////				const auto s = speed;
//
//
//
//					// -------------------------------------------------------------------------------------------------
//					const auto nextPosition = position + orientation.forward() * s / 60.f;
////				renderer.debug.lines.emplace_back(position, nextPosition, libv::vec4f{xy(libv::color::hue_to_rgb(s / 3.0f)), y * 0.5f + 0.5f, 1.f});
//					renderer.debug.lines.emplace_back(position, nextPosition, libv::vec4f{libv::color::hue_to_rgb(s / 3.0f), 1.f});
//					position = nextPosition;
//				}
//			}
//		}
//	log_space.info("Success rate: {}", static_cast<float>(success)/count);
//}

// -------------------------------------------------------------------------------------------------

SpaceCanvas::SpaceCanvas(libv::ctrl::Controls& controls, Renderer& renderer, GameSession& game_session, bool main_canvas) :
		main_canvas(main_canvas),
		game_session(game_session),
		//universe(*game_session.playout.simulation->universe),
		playout(game_session.playout),
//		camera(camera),
		controls(controls),
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

	game_session.nexus.connect<mc::ChangeControlledFaction>(this, [this] {
		// !!!
		controlVar.reset();
	});
}

SpaceCanvas::~SpaceCanvas() {
	// !!!
	game_session.nexus.disconnect_all(this);
}

void SpaceCanvas::enableControls(libv::ctrl::Controls& controls) {
	if (!controlVar)
		// !!!
		controlVar.emplace(
				*this,
				playout,
				playout.simulation->universe->galaxy,
//				player,
				game_session.player
		);

	controls.context_enter<CanvasControl>(&*controlVar);
	controls.context_enter<BaseCameraOrbit>(&camera);
}

void SpaceCanvas::disableControls(libv::ctrl::Controls& controls) {
	controls.context_leave_if_matches<BaseCameraOrbit>(&camera);
	controls.context_leave_if_matches<CanvasControl>(&*controlVar);
}

void SpaceCanvas::attach() {
	if (main_canvas)
		focus();
}

void SpaceCanvas::update(libv::ui::time_duration delta_time) {
	const auto dtf = static_cast<float>(delta_time.count());
//	angle = std::fmod(angle + 5.0f * dtf, 360.0f);
//	time += dtf;
	global_time += dtf;

//	if (global_test_mode != 0) {
//		test_sin_time += dtf;
//		auto t = (std::sin(test_sin_time / 10.f) * 0.5f + 0.5f);
//		if (global_test_mode == 1) {
//			camera.pitch(-t * libv::pi_f * 0.5f);
//		} else if (global_test_mode == 2) {
//			t = t > 0.5f ? 1.f - t : t;
//			camera.pitch(-t * libv::pi_f * 0.5f);
//		} else if (global_test_mode == 3) {
//			const float part = 4;
//			auto t = (std::sin(test_sin_time / 10.f * part) * 0.5f + 0.5f);
//			t = t > 0.5f ? 1.f - t : t;
//			camera.pitch(-t * libv::pi_f * 0.5f / part);
//		}
//	}

	if (main_canvas) {
		// <<< P1: Game_session update shouldnt come from the canvas, or should it?
		game_session.update(delta_time);

//		CanvasControl::update(*this, delta_time);
		if (!controlVar)
			// !!!
			controlVar.emplace(
					*this,
					playout,
					playout.simulation->universe->galaxy,
	//				player,
					game_session.player
			);

		CanvasControl::update(*controlVar, delta_time);
	}
}

// =================================================================================================

// <<< Place it
//libv::vec3f formation_vic(int shipIndex, int numShips) {
////	(void) numShips;
//
//	const auto vicMaxSize = 13;
//	const auto vicAngle = libv::deg_to_rad(135.f);
//	const auto halfVicAngle = vicAngle / 2.f;
//	const auto vicLevelHeight = 0.5f;
////		const auto shipDistance = 1.f; in future
//	const auto vicCellWidth = tan(halfVicAngle) * vicLevelHeight;
//
//	const auto shipIndexInVic = shipIndex % vicMaxSize;
//
//	libv::vec3f posInVic = pos_in_vic(vicMaxSize, shipIndexInVic, vicCellWidth, vicLevelHeight, numShips);
//
//	const auto shipVic = shipIndex / vicMaxSize;
//	const auto vicDistance = 0.5f;
//
//	libv::vec3f pos = {posInVic.x, posInVic.y + shipVic * vicDistance, posInVic.z};
//	return pos;
//}

libv::vec3f pos_in_vic(int vicMaxSize, int indexInVic, float vicCellWidth, float vicLevelHeight) {
	(void) vicMaxSize;

//	const auto shipIndexInVic = indexInVic % vicMaxSize;
	const auto inLeftWing = indexInVic % 2 == 1 ? -1 : 1;
	const auto level = (indexInVic + 1) / 2; //ceiling
	libv::vec3f posInVic = {static_cast<float>(level) * vicLevelHeight, (static_cast<float>(inLeftWing * level) * vicCellWidth), 0};
	return posInVic;
}

libv::vec3f formation(int shipIndex, int numShips) {
	(void) numShips;

//	const auto vicMaxSize1 = 1000;
//	const auto vicMaxSize2 = 100;
	const auto vicMaxSize = 7;
	const auto vicAngle = libv::deg_to_rad(120.f);
	const auto halfVicAngle = vicAngle / 2.f;
	const auto vicLevelHeight = 0.3f;
	const auto vicDistance = 0.5f;
	const auto vicCellWidth = std::tan(halfVicAngle) * vicLevelHeight;
	//-------------------
//	const auto vic_position = xzy(formation_vic(vicIndex, vicNumber));
//	const auto ship_in_vic_position = formation_vic(..., ...)

	const auto shipIndexInVic3 = shipIndex % vicMaxSize;
	const auto vicLevel3 = shipIndex / vicMaxSize;
	libv::vec3f posInVic3 = pos_in_vic(vicMaxSize, shipIndexInVic3, vicCellWidth, vicLevelHeight); //detailed

	const auto shipIndexInVic2 = vicLevel3 % vicMaxSize;
	const auto vicLevel2 = vicLevel3 / vicMaxSize;
	libv::vec3f posInVic2 = xzy(pos_in_vic(vicMaxSize, shipIndexInVic2, vicCellWidth, vicLevelHeight)); //z

	const auto shipIndexInVic1 = vicLevel2 % vicMaxSize;
	const auto vicLevel1 = vicLevel2 / vicMaxSize;
	libv::vec3f posInVic1 = pos_in_vic(vicMaxSize, shipIndexInVic1, vicCellWidth * 10, vicLevelHeight * 10); //blobs

	libv::vec3f pos1 = {posInVic1.x + static_cast<float>(vicLevel1) * vicDistance, posInVic1.y, 0};
	libv::vec3f pos2 = {posInVic2.y + static_cast<float>(vicLevel2) * vicDistance, 0, posInVic2.z};
	libv::vec3f pos3 = {posInVic3.x, posInVic3.y, 0};
	libv::vec3f pos = pos1 + pos2 + pos3;

	return -pos;
}

// =================================================================================================

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

	const auto eye = glr.eye();

	// Set framebuffer to the post-processing target
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
	// TODO P1: Find a better way of managing text (and debug shape) lifetimes
	renderer.text.clear_texts();

	render_opaque(glr, eye, playout.simulation->universe->galaxy);

	// --- Render EditorBackground/Sky ---
	render_background(glr, playout.simulation->universe->galaxy);

	// --- Render Transparent ---
//	renderer.arrow.add_debug_spiral();
//	renderer.arrow.add_debug_view01();
//	renderer.arrow.add_debug_view02();
//	renderer.arrow.add_debug_view03();
//	renderer.arrow.add_debug_view04();
//	renderer.arrow.add_debug_view05();
	render_transparent(glr, playout.simulation->universe->galaxy);

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
//		renderer.debug.spheres.clear();
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

	// --- Post Processing ---

	const auto& mainTexture = renderTarget.resolve(glr);
	postProcessing.pass(glr, mainTexture);

	// Post-Processing sets the framebuffer back to the default
	// glr.framebuffer_draw_default();

	// --- Text ---

	renderer.text.add_debug_coordinates_if_nothing_else();
	renderer.text.render(glr, renderer.resource_context.uniform_stream, screen_picker);
}

void SpaceCanvas::render_opaque(libv::glr::Queue& glr, libv::vec3f eye, Galaxy& galaxy) {
	for (const auto& fleet_ptr : galaxy.fleets) {
		const auto& fleet = *fleet_ptr;
		const auto m_guard = glr.model.push_guard();
		const auto idf = static_cast<float>(+fleet.id);

		glr.model.translate(fleet.position);
		glr.model *= fleet.orientation.to_mat4();
//		glr.model.scale(0.05f);
		glr.model.scale(0.2f);

		const auto numShipRendered = static_cast<int>(std::log(static_cast<float>(fleet.number_of_ships)) / std::log(2.f) * 2.f);
//		const auto numShipRendered = fleet.number_of_ships;
		for (int i = 0; i < numShipRendered; ++i) {
			const auto fi = static_cast<float>(i);

			const auto m2_guard = glr.model.push_guard();
			const auto noiseX = libv::noise_perlin(0x5EED, time / 20 + fleet.distance_travelled / 5, fi * libv::pi / 15.f + idf * libv::e);
			const auto noiseY = libv::noise_perlin(0x5EED, time / 20 + 2 + fleet.distance_travelled / 5, fi * libv::pi / 15.f + idf * libv::e + 5);
			const auto noiseZ = libv::noise_perlin(0x5EED, time / 20 + 4 + fleet.distance_travelled / 5, fi * libv::pi / 15.f + idf * libv::e + 10);
			const auto noise = libv::vec3f{noiseX, noiseY, noiseZ} * 0.2f - 0.1f;
			libv::vec3f ship_pos = formation(i, fleet.number_of_ships) + noise;

			glr.model.translate(ship_pos);
			// glr.model.scale(Fleet::pickingType.radius_universe);
			glr.model.scale(0.05f);

			renderer.fleet.render(glr, renderer.resource_context.uniform_stream, fleet.selectionStatus);
		}

		const auto a = libv::smoothstep((eye - fleet.position).length(), 160.f, 80.f);
		if (a > 0.02f) { // Skip barely and not visible texts
			std::string fleetLabel;
			if (!fleet.commands.empty()) {
				fleetLabel = fmt::format("Fleet {}\n{}\nOri:{}\nI:{}\n{}\n{}", +fleet.id, fleet.faction->name, fleet.orientation, +fleet.commands.front().type, fleet.number_of_ships, fleet.distance_travelled);
			} else {
				fleetLabel = fmt::format("Fleet {}\n{}\nOri:{}\n{}\n{}", +fleet.id, fleet.faction->name, fleet.orientation, fleet.number_of_ships, fleet.distance_travelled);
			}
			renderer.text.add_text(
					fleet.position,
					libv::vec2f{0, -15.f},
					std::move(fleetLabel),
//					libv::vec4f{1.f, 1.f, 1.f, a}
					fleet.faction->colorPrimary * libv::vec4f{1.f, 1.f, 1.f, a}
			);
		}
	}

	for (const auto& planet_ptr : galaxy.planets) {
		const auto& planet = *planet_ptr;
		const auto m_guard = glr.model.push_guard();
		glr.model.translate(planet.position);

		renderer.planet.render(glr, renderer.resource_context.uniform_stream, planet);

		const auto a = libv::smoothstep((eye - planet.position).length(), 100.f, 20.f);
		if (a > 0.02f) // Skip barely and not visible texts
			renderer.text.add_text(
					planet.position,
					libv::vec2f{0, -15.f},
					fmt::format("Planet {}\n{}", +planet.id, planet.faction->name),
//					libv::vec4f{1.f, 1.f, 1.f, a}
					planet.faction->colorPrimary * libv::vec4f{1.f, 1.f, 1.f, a}
			);
	}
}

void SpaceCanvas::render_background(libv::glr::Queue& glr, Galaxy& galaxy) {
	if (!main_canvas)
		return;

	(void) galaxy;

	const auto s_guard = glr.state.push_guard();
	// No need to write depth data for the main background
	glr.state.disableDepthMask();
	renderer.editorBackground.render(glr, canvas_size);
}

void SpaceCanvas::render_transparent(libv::glr::Queue& glr, Galaxy& galaxy) {
	for (const auto& fleet : galaxy.fleets) {
		renderer.arrow.restart_chain(fleet->animation_offset());

		auto prev = fleet->position;
		for (const auto& command : fleet->commands) {
			const auto targetPosition = command.target();
			renderer.arrow.add_arrow(prev, targetPosition, convert_to_arrow_style(command.type));
			prev = targetPosition;
		}
	}

	renderer.arrow.render(glr, canvas_size, renderer.resource_context.uniform_stream);
}

// -------------------------------------------------------------------------------------------------
// TODO P5: Implement style flag modes

void SpaceCanvas::add_debug_point(libv::vec3f a, libv::vec4f color, StyleFlag mode) {
	(void) mode;
	renderer.debug.add_debug_point(a, color);
}

void SpaceCanvas::add_debug_line(libv::vec3f a, libv::vec3f b, libv::vec4f color, StyleFlag mode) {
	(void) mode;
	renderer.debug.add_debug_line(a, b, color);
}

void SpaceCanvas::add_debug_triangle(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec4f color, StyleFlag mode) {
	(void) mode;
	renderer.debug.add_debug_triangle(a, b, c, color);
}

void SpaceCanvas::add_debug_circle(libv::vec3f center, float radius, libv::vec3f normal, libv::vec4f color, StyleFlag mode) {
	(void) mode;
	renderer.debug.add_debug_circle(center, radius, normal, color);
}

void SpaceCanvas::add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count, int segment_count, StyleFlag mode) {
	(void) mode;
	renderer.debug.add_debug_sphere(center, radius, color, ring_count, segment_count);
}

void SpaceCanvas::add_debug_frustum(
		libv::vec3f nbl, libv::vec3f nbr, libv::vec3f ntr, libv::vec3f ntl,
		libv::vec3f fbl, libv::vec3f fbr, libv::vec3f ftr, libv::vec3f ftl,
		libv::vec4f color_sides, libv::vec4f color_wire , StyleFlag mode) {
	(void) mode;
	renderer.debug.add_debug_frustum(nbl, nbr, ntr, ntl, fbl, fbr, ftr, ftl, color_sides, color_wire);
}

void SpaceCanvas::add_debug_quad(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec3f d, libv::vec4f color, StyleFlag mode) {
	(void) mode;
	renderer.debug.add_debug_quad(a, b, c, d, color);
}

void SpaceCanvas::clear_debug_shapes() {
	renderer.debug.clear_debug_shapes();
}

// -------------------------------------------------------------------------------------------------

} // namespace space
