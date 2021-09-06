// Project: libv, File: app/space/canvas_behaviour.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/canvas_behaviour.hpp>
// libv
#include <libv/ctrl/controls.hpp> // TODO P0: temporary for binds
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/distance/intersect.hpp>
// pro
#include <space/canvas.hpp>
#include <space/command.hpp>
#include <space/log.hpp>
#include <space/playout.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void CanvasBehaviour::register_controls(libv::ctrl::FeatureRegister controls) {
	controls.feature_action<app::SpaceCanvas>("space.add_fleet_at_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		log_space.info("mouse_local_coord: {}, world_coord: {}", mouse_local_coord, world_coord);

		if (!ctx.universe.fleets.empty()) {
			ctx.playout.queue<app::CommandFleetMove>(
					ctx.universe.fleets.back().id,
					world_coord
			);
			// <<< should controls use nexus or playout directly? Think about console and lua scripts too.
//			nexus.broadcast<app::CommandFleetMove>(static_cast<app::FleetID>(ctx.universe.fleets.size()), world_coord);
//			nexus.broadcast<mc::RequestCommandFleetMove>(world_coord);
		}

		ctx.playout.queue<app::CommandFleetSpawn>(world_coord);
//		nexus.broadcast<mc::RequestCommandFleetSpawn>(world_coord);
	});

	controls.feature_action<app::SpaceCanvas>("space.select_fleet", [](const auto&, app::SpaceCanvas& ctx) {
		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		log_space.info("mouse_local_coord: {}, world_coord: {}", mouse_local_coord, world_coord);

//		if (!ctx.universe.fleets.empty()) {
//			ctx.playout.queue<app::CommandFleetSelect>(
//					static_cast<app::FleetID>(ctx.universe.fleets.size()),
//					world_coord
//					);
//		}
		if(!ctx.universe.fleets.empty())
			ctx.playout.queue<app::CommandFleetSelect>(
//					ctx.universe.fleets.back().id,
//					world_coord
					);

	});

	controls.feature_action<app::SpaceCanvas>("space.move_fleet_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		log_space.info("mouse_local_coord: {}, world_coord: {}", mouse_local_coord, world_coord);

		if (!ctx.universe.fleets.empty()) {
			ctx.playout.queue<app::CommandFleetMove>(
					ctx.universe.selectedFleetID,
					world_coord
			);
		}
	});

	controls.feature_action<app::SpaceCanvas>("space.queue_move_fleet_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		log_space.info("mouse_local_coord: {}, world_coord: {}", mouse_local_coord, world_coord);

		if (!ctx.universe.fleets.empty()) {
			ctx.playout.queue<app::CommandFleetQueueMove>(
					ctx.universe.selectedFleetID,
					world_coord
			);
		}
	});

	controls.feature_action<app::SpaceCanvas>("space.warp_camera_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		log_space.info("mouse_local_coord: {}, world_coord: {}", mouse_local_coord, world_coord);

		const auto playerID = app::PlayerID{0};
		ctx.playout.queue<app::CommandCameraWarpTo>(playerID, world_coord);

		ctx.camera.warp_to(world_coord); // <<< Move this line to CommandCameraWarpTo apply
	});
}

void CanvasBehaviour::bind_default_controls(libv::ctrl::Controls& controls) {
	// TODO P1: libv.ctrl: analog feature (on time update) bypasses the accidental collusion resolution system (specialization) with an action feature

	controls.bind("space.add_fleet_at_mouse", "Ctrl + LMB [press]");
	controls.bind("space.select_fleet", "L");
	controls.bind("space.move_fleet_to_mouse", "Ctrl + RMB [press]");
	controls.bind("space.queue_move_fleet_to_mouse", "Shift + RMB [press]");
	controls.bind("space.warp_camera_to_mouse", "Z");
}

// -------------------------------------------------------------------------------------------------

} // namespace app
