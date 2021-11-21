// Project: libv, File: app/space/view/canvas_control.cpp

// hpp
#include <space/view/canvas_control.hpp>
// libv
#include <libv/ctrl/controls.hpp> // TODO P0: temporary for default binds
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/distance/distance.hpp>
#include <libv/math/distance/intersect.hpp>
#include <space/view/frustum.hpp>
// std
#include <optional>
// pro
#include <space/view/canvas.hpp>
#include <space/cto.hpp>
#include <space/log.hpp>
#include <space/playout.hpp>
#include <space/universe/universe.hpp>


namespace app {
namespace {

// -------------------------------------------------------------------------------------------------

std::optional<FleetID> calculate_hit_fleet(SpaceCanvas& ctx) {
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

			if (distance < hover_distance && distance < Fleet::pickingType.radius_screen) {
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

void CanvasControl::register_controls(libv::ctrl::FeatureRegister controls) {
	controls.feature_action<app::SpaceCanvas>("space.spawn_fleet_at_mouse", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);

		// <<< should controls use nexus or playout directly? Think about the console and the lua scripts too.
//		nexus.broadcast<mc::RequestCommandFleetSpawn>(world_coord);

		ctx.playout.process<CTO_FleetSpawn>(world_coord);
	});

	controls.feature_action<app::SpaceCanvas>("space.select_fleet", [](const auto&, app::SpaceCanvas& ctx) {
		std::optional<FleetID> fleet_id = calculate_hit_fleet(ctx);
		if (fleet_id) {
			ctx.playout.process<CTO_FleetSelect>(*fleet_id);
		}
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
		const auto eye = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(eye, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		ctx.start_selected_grid_point = world_coord;
		ctx.start_dir = mouse_ray_dir;
//		ctx.start_eye = eye;
	});

	///VERSION 1: Box selection in real 3D
	controls.feature_analog<app::SpaceCanvas>("space.selection_box_end_3D", [](const auto&, app::SpaceCanvas& ctx) {
		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto eye = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(eye, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		// A---------D
		// |         |
		// |         |
		// B---------C

		// <<< const an
		auto an = ctx.start_dir;
		auto cn = mouse_ray_dir;

		const auto up = ctx.camera.up();
		const auto right = ctx.camera.right();
		const auto forward = ctx.camera.forward();

		//\     |   /
		//  \   |f /
		//alfa_\|/gamma____
		float alfa = libv::pi / 2 - std::acos(dot(an, forward));
		float gamma = libv::pi / 2 - std::acos(dot(cn, forward));
		float sin_alfa = std::sin(alfa);
		float sin_gamma = std::sin(gamma);
		float height_on_camera_plane = sin_alfa / sin_gamma;
		cn = cn * height_on_camera_plane;

		// There is at least one component that is greater than sqrt(1/3) = 0.57735, 0.5 is a good enough check
		float x2;
		if (const auto xy = (right.y * up.x - right.x * up.y); std::abs(xy) > 0.5f)
			x2 = ((cn.y - an.y) * up.x - (cn.x - an.x) * up.y) / xy;
		else if (const auto xz = (right.z * up.x - right.x * up.z); std::abs(xz) > 0.5f)
			x2 = ((cn.z - an.z) * up.x - (cn.x - an.x) * up.z) / xz;
		else
			x2 = ((cn.y - an.y) * up.z - (cn.z - an.z) * up.y) / (right.y * up.z - right.z * up.y);

		// There is at least one component that is greater than sqrt(1/3) = 0.57735, 0.5 is a good enough check
		float x1;
		if (std::abs(up.x) > 0.5f)
			x1 = (cn.x - an.x - x2 * right.x) / up.x;
		else if (std::abs(up.y) > 0.5f)
			x1 = (cn.y - an.y - x2 * right.y) / up.y;
		else
			x1 = (cn.z - an.z - x2 * right.z) / up.z;

		// <<< const bn
		// <<< const dn
		auto bn = (cn - x2 * right);
		auto dn = (an + x2 * right);


		//			const auto near = ctx.camera.near();
		//			const auto far = ctx.camera.far();
		const auto far = 15.f;
		const auto near = 5.f;

		const auto ann = normalize(an);
		const auto bnn = normalize(bn);
		const auto cnn = normalize(cn);
		const auto dnn = normalize(dn);
		const auto bnn_cos = (dot(bnn, forward));
		const auto cnn_cos = (dot(cnn, forward));
		const auto dnn_cos = (dot(dnn, forward));
		const auto ann_cos = (dot(ann, forward));

		libv::vec3f nbl = eye + bnn * near / bnn_cos;
		libv::vec3f nbr = eye + cnn * near / cnn_cos;
		libv::vec3f ntr = eye + dnn * near / dnn_cos;
		libv::vec3f ntl = eye + ann * near / ann_cos;

		libv::vec3f fbl = eye + bnn * far / bnn_cos;
		libv::vec3f fbr = eye + cnn * far / cnn_cos;
		libv::vec3f ftr = eye + dnn * far / dnn_cos;
		libv::vec3f ftl = eye + ann * far / ann_cos;

		const auto frustum = Frustum(nbl, nbr, ntr, ntl, fbl, fbr, ftr, ftl);

		std::vector<FleetID> selected_fleet_ids;
		for (auto& fleet : ctx.universe.fleets) {
			const auto result = frustum.sphereInFrustum(fleet.position, Fleet::pickingType.radius_universe);

			switch (result) {
			case Frustum::Position::OUTSIDE:
				break;
			case Frustum::Position::INTERSECT:
				selected_fleet_ids.emplace_back(fleet.id);
				break;
			case Frustum::Position::INSIDE:
				selected_fleet_ids.emplace_back(fleet.id);
				break;
			}
		}
		ctx.playout.process<CTO_FleetBoxSelect>(selected_fleet_ids);

		if (true) {
			ctx.clear_debug_shapes();
			libv::vec4f line_color = {0.3f, 0.5f, 0.9f, 0.9f};
			line_color = {0.9f, 0.2f, 0, 0.9f};

			auto a_near = std::acos(dot(normalize(nbl - nbr), normalize(ntr - nbr)));
			auto b_near = std::acos(dot(normalize(nbr - ntr), normalize(ntl - ntr)));
			auto c_near = std::acos(dot(normalize(ntr - ntl), normalize(nbl - ntl)));
			auto d_near = std::acos(dot(normalize(ntl - nbl), normalize(nbr - nbl)));
			auto a_far = std::acos(dot(normalize(fbl - fbr), normalize(ftr - fbr)));
			auto b_far = std::acos(dot(normalize(fbr - ftr), normalize(ftl - ftr)));
			auto c_far = std::acos(dot(normalize(ftr - ftl), normalize(fbl - ftl)));
			auto d_far = std::acos(dot(normalize(ftl - fbl), normalize(fbr - fbl)));
			log_space.info("\na_near: {:6.2f}, b_near: {:6.2f}, c_near: {:6.2f}, d_near: {:6.2f}\na_far : {:6.2f}, b_far : {:6.2f}, c_far : {:6.2f}, d_far : {:6.2f}",
					libv::rad_to_deg(a_near), libv::rad_to_deg(b_near), libv::rad_to_deg(c_near), libv::rad_to_deg(d_near),
					libv::rad_to_deg(a_far), libv::rad_to_deg(b_far), libv::rad_to_deg(c_far), libv::rad_to_deg(d_far));
			ctx.add_debug_frustum(
					nbl, nbr, ntr, ntl, fbl, fbr, ftr, ftl,
					{0.8f, 0, 1, 1}, {0.2f, 0.2f, 0.6, 0.2f});

//			{
//				for (int i = -20 ; i < 20 ; ++i) {
//					for (int j = -20 ; j < 20 ; ++j) {
//						const auto result = frustum.sphereInFrustum({i, j, 0}, 0.25f);
//						libv::vec4f color;
//						switch (result) {
//						case Frustum::Position::OUTSIDE:
//							color = {0.5f, 0.5f, 0.5f, 1};
//							break;
//						case Frustum::Position::INTERSECT:
//							color = {1, 0.5f, 0, 1};
//							break;
//						case Frustum::Position::INSIDE:
//							color = {1, 1, 0.8f, 1};
//							break;
//						}
//						ctx.add_debug_sphere({i, j, 0}, 0.25f, color, 10, 10);
//					}
//				}
//			}
		}
	});

	///VERSION 2: Box selection in real 2.5D (on grid)
	controls.feature_analog<app::SpaceCanvas>("space.selection_box_end_25D", [](const auto&, app::SpaceCanvas& ctx) {
		const auto world_coord = calculate_world_coord(ctx);
		const auto eye = ctx.camera.eye();
		libv::vec3f A = ctx.start_selected_grid_point;
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

			ctx.clear_debug_shapes();
//			const auto count = 100;
//			for (int i = 0 ; i < count ; ++i) {
//				float fi = i;
//				const auto rad1 = libv::pi / 23.12312f;
//				const auto rad2 = libv::pi / 16.12312f;
//				ctx.add_debug_circle({fi * 2, 0, 0}, 1, {std::sin(rad1 * fi), std::cos(rad1 * fi) * std::sin(rad2 * fi), std::cos(rad2 * fi)}, {fi / count * 0.8f, fi / count * 0.5f, 0.2f, 0.9f});
////				ctx.add_debug_circle({fi, 2, 0}, 1, {std::sin(rad1 * fi), 0, std::cos(rad1 * fi)}, {0.8f, 0.5f, 0.2f, 0.9f});
//			}


//			const auto line_color = libv::vec4f{0.9f, 0.2f, 0, 0.9f};
//			ctx.add_debug_line(line_a.a, line_a.b, line_color);
//			ctx.add_debug_line(line_b.a, line_b.b, line_color);
//			ctx.add_debug_line(line_c.a, line_c.b, line_color);
//			ctx.add_debug_line(line_d.a, line_d.b, line_color);
//			ctx.add_debug_frustum(line_a.b, line_b.b, line_c.b, line_d.b, line_a.a, {1, 0, 0, 1}, {0.2f, 0, 0.8f, 0.6f});
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

void CanvasControl::bind_default_controls(libv::ctrl::Controls& controls) {
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
