// Project: libv, File: app/space/canvas_behaviour.cpp

// hpp
#include <space/canvas_behaviour.hpp>
// libv
#include <libv/ctrl/controls.hpp> // TODO P0: temporary for default binds
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/distance/distance.hpp>
#include <libv/math/distance/intersect.hpp>
// std
#include <optional>
// pro
#include <space/canvas.hpp>
#include <space/cto.hpp>
#include <space/log.hpp>
#include <space/playout.hpp>
#include <space/universe/universe.hpp>


namespace app {
namespace {

// -------------------------------------------------------------------------------------------------

std::optional<FleetID> calculate_hit_fleet(SpaceCanvas& ctx) {
	// TODO P2: Use a ScreenPickableType based dynamic values for screen picking
	constexpr auto SECONDARY_HIT_MIN_PIXEL_DISTANCE = 100.f;

	bool direct_hit = false;
	std::optional<FleetID> fleet_id;

	const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
	const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
	const auto mouse_ray_pos = ctx.camera.eye();

	auto hover_distance = std::numeric_limits<float>::max();

	for (Fleet& fleet : ctx.universe.fleets) {
		auto[hit, distance] = libv::distanceTestLineToSphere(mouse_ray_pos, mouse_ray_dir, fleet.position, 0.2f);

		if (hit && (!direct_hit || distance < hover_distance)) {
			direct_hit = true;
			fleet_id = fleet.id;
			hover_distance = distance;

		} else if (!direct_hit) {
			const auto objectSPosition = ctx.screen_picker.to_screen(fleet.position);
			distance = (mouse_local_coord - objectSPosition).length();

			if (distance < hover_distance && distance < SECONDARY_HIT_MIN_PIXEL_DISTANCE) {
				fleet_id = fleet.id;
				hover_distance = distance;
			}
		}
	}
	return fleet_id;
}

libv::vec3f calculate_world_coord(SpaceCanvas& ctx) {
	const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
	const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
	const auto mouse_ray_pos = ctx.camera.eye();
	const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

	return world_coord;
}

} // namespace -------------------------------------------------------------------------------------

void CanvasBehaviour::register_controls(libv::ctrl::FeatureRegister controls) {
	controls.feature_action<app::SpaceCanvas>("space.spawn_fleet_at_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);

		// <<< should controls use nexus or playout directly? Think about the console and the lua scripts too.
//		nexus.broadcast<mc::RequestCommandFleetSpawn>(world_coord);

		ctx.playout.process<CTO_FleetSpawn>(world_coord);
	});

	controls.feature_action<app::SpaceCanvas>("space.select_fleet", [](const auto&, app::SpaceCanvas& ctx) {
		std::optional<FleetID> fleet_id = calculate_hit_fleet(ctx);
		if (fleet_id)
			ctx.playout.process<CTO_FleetSelect>(*fleet_id);
	});

	controls.feature_action<app::SpaceCanvas>("space.select_fleet_add", [](const auto&, app::SpaceCanvas& ctx) {
		std::optional<FleetID> fleet_id = calculate_hit_fleet(ctx);
		if (fleet_id)
			ctx.playout.process<CTO_FleetSelectAdd>(*fleet_id);
	});

	controls.feature_action<app::SpaceCanvas>("space.move_fleet_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);
		ctx.playout.process<CTO_FleetMove>(world_coord);
	});

	controls.feature_action<app::SpaceCanvas>("space.queue_move_fleet_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);
		ctx.playout.process<CTO_FleetQueueMove>(world_coord);
	});

	controls.feature_action<app::SpaceCanvas>("space.selection_box_start", [](const auto&, app::SpaceCanvas& ctx) {
		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));
		log_space.info("mouse_ray_pos: {}, mouse_ray_dir: {}", mouse_ray_pos, mouse_ray_dir);

		ctx.start_line = SpaceCanvas::Line{mouse_ray_pos, world_coord};
	});

	///VERSION 2: Box selection in real 2.5D (on grid)
	controls.feature_analog<app::SpaceCanvas>("space.selection_box_end_25D", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);
		const auto eye = ctx.camera.eye();
		libv::vec3f A = ctx.start_line.b;
		libv::vec3f C = world_coord;

		// Project the up and right vectors to the z=0 plane
		const auto plane_up = libv::vec3f(xy(ctx.camera.up()), 0.f);
		const auto plane_right = libv::vec3f(xy(ctx.camera.right()), 0.f);

		// TODO P2: Handle some corner cases when the camera is on the z=0 plane and the users clicks to +/- inf
		const float x2 = ((C.y - A.y) * plane_up.x - (C.x - A.x) * plane_up.y) / (plane_right.y * plane_up.x - plane_right.x * plane_up.y);
		// const float x1 = (B.x - A.x - x2 * plane_right.x) / plane_up.x; // No need for it

//		libv::vec3f C = A + x1 * plane_up;
		libv::vec3f B = C - x2 * plane_right;
		libv::vec3f D = A + x2 * plane_right;

		// A---------D
		// |         |
		// |         |
		// B---------C

		std::vector<FleetID> selected_fleet_ids;
		const auto AD = D - A;
		const auto AB = B - A;
		const auto dot_AD = dot(AD, AD);
		const auto dot_AB = dot(AB, AB);
		for (const auto& fleet : ctx.universe.fleets) {
			const auto M = libv::vec3f(xy(fleet.position), 0.f);
//			(0<AM⋅AB<AB⋅AB)∧(0<AM⋅AD<AD⋅AD)
			const auto AM = M - A;
			if (0 < dot(AM, AD) && dot(AM, AD) < dot_AD && 0 < dot(AM, AB) && dot(AM, AB) < dot_AB)
				selected_fleet_ids.emplace_back(fleet.id);
		}
		ctx.playout.process<CTO_FleetBoxSelect>(selected_fleet_ids);

		// Debug visualization
		if (true) {
			const auto axis = normalize((A + C) * 0.5f - eye);

			const auto line_a = SpaceCanvas::Line{eye + axis * ctx.camera.near() * 1.5f, A};
			const auto line_b = SpaceCanvas::Line{eye + axis * ctx.camera.near() * 1.5f, B};
			const auto line_c = SpaceCanvas::Line{eye + axis * ctx.camera.near() * 1.5f, C};
			const auto line_d = SpaceCanvas::Line{eye + axis * ctx.camera.near() * 1.5f, D};

			const auto line_color = libv::vec4f{0.9f, 0.2f, 0, 0.9f};
			ctx.clear_debug_shapes();
			ctx.add_debug_line(line_a.a, line_a.b, line_color);
			ctx.add_debug_line(line_b.a, line_b.b, line_color);
			ctx.add_debug_line(line_c.a, line_c.b, line_color);
			ctx.add_debug_line(line_d.a, line_d.b, line_color);
			ctx.add_debug_frustum(line_a.b, line_b.b, line_c.b, line_d.b, line_a.a, {1, 0, 0, 1}, {0.2f, 0, 0.8f, 0.6f});
			ctx.add_debug_quad(line_a.b, line_b.b, line_c.b, line_d.b, {0, 0.8f, 0.8f, 0.6f});
		}
	});

	controls.feature_action<app::SpaceCanvas>("space.warp_camera_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);
//		const auto userID = app::UserID{0};
//		ctx.playout.process<CTO_CameraWarpTo>(userID, world_coord);

		ctx.camera.warp_to(world_coord); // <<< Move this line to CommandCameraWarpTo apply for tracking
	});
}

void CanvasBehaviour::bind_default_controls(libv::ctrl::Controls& controls) {
	// TODO P1: libv.ctrl: analog feature (on time update) bypasses the accidental collusion resolution system (specialization) with an action feature

	controls.bind("space.move_fleet_to_mouse", "Ctrl + RMB [press]");
	controls.bind("space.queue_move_fleet_to_mouse", "Shift + RMB [press]");
	controls.bind("space.select_fleet", "LMB [press]");
	controls.bind("space.select_fleet_add", "Shift + LMB [press]");
//	controls.bind("space.selection_box_end", "O [release]");
	controls.bind("space.selection_box_start", "O [press]");
	controls.bind("space.selection_box_start", "I [press]");
	controls.bind("space.selection_box_end_3D", "O [auto]");
	controls.bind("space.selection_box_end_25D", "I [auto]");
	controls.bind("space.spawn_fleet_at_mouse", "Ctrl + LMB [press]");
	controls.bind("space.warp_camera_to_mouse", "Z");
}

// -------------------------------------------------------------------------------------------------

} // namespace app
