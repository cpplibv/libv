// Project: libv, File: app/space/canvas_behaviour.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/canvas_behaviour.hpp>
// libv
#include <libv/ctrl/controls.hpp> // TODO P0: temporary for binds
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/distance/intersect.hpp>
// pro
#include <space/command.hpp>
#include <space/log.hpp>
#include <space/playout.hpp>
#include <space/render.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void CanvasBehaviour::register_controls(libv::ctrl::FeatureRegister controls) {
//		libv::ctrl::scale_group sg_translate{
//				.impulse = 0.1,
//				.time = 1.0,
//				.mouse = 1.0 / 600.0,
//				.scroll = 0.1,
//				.gp_analog = 1.0,
//				.js_analog = 1.0
//		};

	controls.feature_action<app::SpaceCanvas>("space.add_fleet_by_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		log_space.info("mouse_local_coord: {}, world_coord: {}", mouse_local_coord, world_coord);

		if (!ctx.universe.fleets.empty()) {
			ctx.playout.buffer.queue<app::CommandFleetMove>(
					static_cast<app::FleetID>(ctx.universe.fleets.size() - 1),
					world_coord
			);

//			nexus.broadcast<app::CommandFleetMove>(static_cast<app::FleetID>(ctx.universe.fleets.size() - 1), world_coord);
//			nexus.broadcast<mc::RequestCommandFleetMove>(world_coord);
		}

		ctx.playout.buffer.queue<app::CommandFleetSpawn>(world_coord);
//		nexus.broadcast<mc::RequestCommandFleetSpawn>(world_coord);
	});

	controls.feature_action<app::SpaceCanvas>("space.warp_camera_to_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		log_space.info("mouse_local_coord: {}, world_coord: {}", mouse_local_coord, world_coord);

		const auto playerID = app::PlayerID{0};
		ctx.playout.buffer.queue<app::CommandCameraWarpTo>(playerID, world_coord);

		ctx.camera.warp_to(world_coord); // <<< Move this line to CommandCameraWarpTo apply
	});
}

void CanvasBehaviour::bind_default_controls(libv::ctrl::Controls& controls) {
	controls.bind("space.add_fleet_by_mouse", "Ctrl + LMB [press]");
	controls.bind("space.warp_camera_to_mouse", "Z");
}

// -------------------------------------------------------------------------------------------------

} // namespace app
