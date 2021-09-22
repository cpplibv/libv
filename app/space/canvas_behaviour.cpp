// Project: libv, File: app/space/canvas_behaviour.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/canvas_behaviour.hpp>
// libv
#include <libv/ctrl/controls.hpp> // TODO P0: temporary for binds
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/distance/distance.hpp>
#include <libv/math/distance/intersect.hpp>
// pro
#include <space/canvas.hpp>
#include <space/command.hpp>
#include <space/log.hpp>
#include <space/playout.hpp>


namespace app {
namespace {

// -------------------------------------------------------------------------------------------------

std::optional<FleetID> calculate_hit_fleet(SpaceCanvas& ctx) {
	constexpr auto SECONDARY_HIT_MIN_PIXEL_DISTANCE = 100.f;

	bool direct_hit = false;
	std::optional<FleetID> fleet_id;

	const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
	const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
	const auto mouse_ray_pos = ctx.camera.eye();
	const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));
	log_space.info("mouse_local_coord: {}, world_coord: {}", mouse_local_coord, world_coord);

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
	log_space.info("mouse_local_coord: {}, world_coord: {}", mouse_local_coord, world_coord);

	return world_coord;
}

} // namespace -------------------------------------------------------------------------------------

void CanvasBehaviour::register_controls(libv::ctrl::FeatureRegister controls) {
	controls.feature_action<app::SpaceCanvas>("space.spawn_fleet_at_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);

		// <<< should controls use nexus or playout directly? Think about the console and the lua scripts too.
//		nexus.broadcast<mc::RequestCommandFleetSpawn>(world_coord);

		ctx.playout.queue<app::CommandFleetSpawn>(world_coord);
	});

	controls.feature_action<app::SpaceCanvas>("space.select_fleet", [](const auto&, app::SpaceCanvas& ctx) {
		std::optional<FleetID> fleet_id = calculate_hit_fleet(ctx);
		if (fleet_id)
			ctx.playout.queue<app::CommandFleetSelect>(
					*fleet_id
			);
	});

	controls.feature_action<app::SpaceCanvas>("space.select_fleet_add", [](const auto&, app::SpaceCanvas& ctx) {
		std::optional<FleetID> fleet_id = calculate_hit_fleet(ctx);
		if (fleet_id)
			ctx.playout.queue<app::CommandFleetSelectAdd>(
					*fleet_id
			);
	});

	controls.feature_action<app::SpaceCanvas>("space.move_fleet_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);
		ctx.playout.queue<app::CommandFleetMove>(
				world_coord
		);
	});

	controls.feature_action<app::SpaceCanvas>("space.queue_move_fleet_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);
		ctx.playout.queue<app::CommandFleetQueueMove>(
				world_coord
		);
	});

	controls.feature_action<app::SpaceCanvas>("space.warp_camera_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);
		const auto playerID = app::PlayerID{0};
		ctx.playout.queue<app::CommandCameraWarpTo>(playerID, world_coord);

		ctx.camera.warp_to(world_coord); // <<< Move this line to CommandCameraWarpTo apply
	});

	controls.feature_action<app::SpaceCanvas>("space.dev0", [](const auto&, app::SpaceCanvas& ctx) {
		log_space.info("Forward {}", ctx.camera.forward());
		log_space.info("Right {}", ctx.camera.right());
		log_space.info("Up {}", ctx.camera.up());

		log_space.info("Dot Forward Right {:+1.6f}", dot(ctx.camera.forward(), ctx.camera.right()));
		log_space.info("Dot Forward Up    {:+1.6f}", dot(ctx.camera.forward(), ctx.camera.up()));
		log_space.info("Dot Right   Up    {:+1.6f}", dot(ctx.camera.right(), ctx.camera.up()));
	});
}

void CanvasBehaviour::bind_default_controls(libv::ctrl::Controls& controls) {
	// TODO P1: libv.ctrl: analog feature (on time update) bypasses the accidental collusion resolution system (specialization) with an action feature

	controls.bind("space.spawn_fleet_at_mouse", "Ctrl + LMB [press]");
	controls.bind("space.select_fleet", "LMB [press]");
	controls.bind("space.select_fleet_add", "Shift + LMB [press]");
	controls.bind("space.move_fleet_to_mouse", "Ctrl + RMB [press]");
	controls.bind("space.queue_move_fleet_to_mouse", "Shift + RMB [press]");
	controls.bind("space.warp_camera_to_mouse", "Z");

	controls.bind("space.dev0", "kp0");
}

// -------------------------------------------------------------------------------------------------

} // namespace app
