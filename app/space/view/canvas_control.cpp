// Project: libv, File: app/space/view/canvas_control.cpp

// hpp
#include <space/view/canvas_control.hpp>
// libv
#include <libv/ctrl/binding_register.hpp> // Temporary for the default binds
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/distance/distance.hpp>
#include <libv/math/distance/intersect.hpp>
#include <space/view/frustum.hpp>
// std
#include <optional>
// pro
#include <space/log.hpp>
#include <space/message/cto.hpp>
#include <space/sim/faction.hpp>
#include <space/sim/galaxy.hpp>
#include <space/sim/playout/playout.hpp>
#include <space/view/canvas.hpp>
//#include <space/sim/universe.hpp>


namespace space {
namespace {

// -------------------------------------------------------------------------------------------------

libv::entity_ptr<Fleet> calculateHitFleet(SpaceCanvas& canvas, Galaxy& galaxy) {
	bool directHit = false;
	libv::entity_ptr<Fleet> fleetHit = nullptr;

	const auto mouse_local_coord = canvas.calculate_local_mouse_coord();
	const auto mouse_ray_dir = canvas.screen_picker.to_world(mouse_local_coord);
	const auto mouse_ray_pos = canvas.camera.eye();

	auto hover_distance = std::numeric_limits<float>::max();

	for (auto& fleet : galaxy.fleets) {
		//TODO P4?: Readjusting is needed for multiple ship fleets (either multiple spheres, or an oblong/cuboid shape)
		auto[hit, distance] = libv::distanceTestLineToSphere(mouse_ray_pos, mouse_ray_dir, fleet->position, 0.2f);

		if (hit && (!directHit || distance < hover_distance)) {
			directHit = true;
			fleetHit = fleet;
			hover_distance = distance;

		} else if (!directHit) {
			const auto objectSPosition = canvas.screen_picker.to_screen(fleet->position);
			distance = (mouse_local_coord - objectSPosition).length();

			if (distance < hover_distance && distance < Fleet::pickingType.radius_screen) {
				fleetHit = fleet;
				hover_distance = distance;
			}
		}
	}

	return fleetHit;
}

auto calculateHit(SpaceCanvas& canvas, Galaxy& galaxy) {
	struct Result {
		libv::entity_ptr<Planet> planetHit = nullptr;
		libv::entity_ptr<Fleet> fleetHit = nullptr;
		libv::vec3f worldCoord;
	} result;

	bool directHit = false;

	const auto mouse_local_coord = canvas.calculate_local_mouse_coord();
	const auto mouse_ray_dir = canvas.screen_picker.to_world(mouse_local_coord);
	const auto mouse_ray_pos = canvas.camera.eye();

	auto hover_distance = std::numeric_limits<float>::max();

	for (auto& planet : galaxy.planets) {
		auto[hit, distance] = libv::distanceTestLineToSphere(mouse_ray_pos, mouse_ray_dir, planet->position, 0.2f);

		if (hit && (!directHit || distance < hover_distance)) {
			directHit = true;
			result.planetHit = planet;
			hover_distance = distance;

		} else if (!directHit) {
			const auto objectSPosition = canvas.screen_picker.to_screen(planet->position);
			distance = (mouse_local_coord - objectSPosition).length();

			if (distance < hover_distance && distance < Planet::pickingType.radius_screen) {
				result.planetHit = planet;
				hover_distance = distance;
			}
		}
	}

	for (auto& fleet : galaxy.fleets) {
		auto[hit, distance] = libv::distanceTestLineToSphere(mouse_ray_pos, mouse_ray_dir, fleet->position, 0.2f);

		if (hit && (!directHit || distance < hover_distance)) {
			directHit = true;
			result.fleetHit = fleet;
			hover_distance = distance;

		} else if (!directHit) {
			const auto objectSPosition = canvas.screen_picker.to_screen(fleet->position);
			distance = (mouse_local_coord - objectSPosition).length();

			if (distance < hover_distance && distance < Fleet::pickingType.radius_screen) {
				result.fleetHit = fleet;
				hover_distance = distance;
			}
		}
	}

	if (result.fleetHit)
		result.planetHit = nullptr;

	if (!result.fleetHit && !result.planetHit)
		result.worldCoord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

	return result;
}

libv::vec3f calculate_world_coord(SpaceCanvas& canvas) {
	const auto mouse_local_coord = canvas.calculate_local_mouse_coord();
	const auto mouse_ray_dir = canvas.screen_picker.to_world(mouse_local_coord);
	const auto mouse_ray_pos = canvas.camera.eye();
	const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

	return world_coord;
}

// -------------------------------------------------------------------------------------------------

void selectionSingle(CanvasControl& ctx, bool commitSelection) {
	auto fleet = calculateHitFleet(ctx.canvas, ctx.galaxy);

	const auto isSelected = [](const auto& fl) {
		return fl.selectionStatus == Fleet::Selection::selected ||
				fl.selectionStatus == Fleet::Selection::selectedHoverSingle ||
				fl.selectionStatus == Fleet::Selection::selectedHoverBox;
	};

	if (commitSelection)
		for (auto& fl : ctx.galaxy.fleets)
			fl->selectionStatus = Fleet::Selection::notSelected;
	else
		for (auto& fl : ctx.galaxy.fleets)
			fl->selectionStatus = isSelected(*fl) ? Fleet::Selection::selected : Fleet::Selection::notSelected;

	if (fleet) {
		if (commitSelection) {
			fleet->selectionStatus = Fleet::Selection::selectedHoverSingle;
			ctx.playout.process<CTO_FleetSelect>(fleet->id);
		} else
			fleet->selectionStatus = isSelected(*fleet) ? Fleet::Selection::selectedHoverSingle : Fleet::Selection::hoverSingle;
	} else if (commitSelection)
		ctx.playout.process<CTO_FleetClearSelection>();
}

// VERSION 1: Box selection in real 2.5D (on grid)
void selection25D(CanvasControl& ctx, bool commitSelection) {
	const auto world_coord = calculate_world_coord(ctx.canvas);

	// Project the up and right vectors to the z=0 plane
	const auto plane_up = libv::vec3f(xy(ctx.canvas.camera.up()), 0.f);
	const auto plane_right = libv::vec3f(xy(ctx.canvas.camera.right()), 0.f);

	libv::vec3f A = ctx.selectionStartGridPoint;
	libv::vec3f C = world_coord;

	const float x2 = ((C.y - A.y) * plane_up.x - (C.x - A.x) * plane_up.y) / (plane_right.y * plane_up.x - plane_right.x * plane_up.y);
	// const float x1 = (B.x - A.x - x2 * plane_right.x) / plane_up.x; // No need for x1 as x2 could be reused from a different corner

	libv::vec3f B = C - x2 * plane_right;
	libv::vec3f D = A + x2 * plane_right; // Alternative formula: A + x1 * plane_up

	// A---------D
	// |         |
	// |         |
	// B---------C

	const auto AD = D - A;
	const auto AB = B - A;
	const auto dot_AD = dot(AD, AD);
	const auto dot_AB = dot(AB, AB);

	std::vector<FleetID> selectedFleetIDs;

	for (auto& fleet : ctx.galaxy.fleets) {
		const auto M = libv::vec3f(xy(fleet->position), 0.f);
		// (0 < AM ⋅ AB < AB ⋅ AB) ∧ (0 < AM ⋅ AD < AD ⋅ AD)
		const auto AM = M - A;
		if (0 < dot(AM, AD) && dot(AM, AD) < dot_AD && 0 < dot(AM, AB) && dot(AM, AB) < dot_AB) {
			if (commitSelection) {
				selectedFleetIDs.emplace_back(fleet->id);
				fleet->selectionStatus = Fleet::Selection::selected;
			} else
				fleet->selectionStatus = Fleet::Selection::hoverBox;
		} else
			fleet->selectionStatus = Fleet::Selection::notSelected;
	}

	if (commitSelection)
		ctx.playout.process<CTO_FleetSelectBox>(std::move(selectedFleetIDs));

	// Debug visualization
	if (true) {
		ctx.canvas.clear_debug_shapes();
		ctx.canvas.add_debug_quad(A, B, C, D, {0, 0.8f, 0.8f, 0.6f});
	}
}

// VERSION 2: Box selection in real 3D
void selection3D(CanvasControl& ctx, bool commitSelection) {
	const auto mouse_local_coord = ctx.canvas.calculate_local_mouse_coord();
	const auto mouse_ray_dir = ctx.canvas.screen_picker.to_world(mouse_local_coord);
	const auto eye = ctx.canvas.camera.eye();
//		const auto world_coord = libv::intersect_ray_plane(eye, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

	// A---------D
	// |         |
	// |         |
	// B---------C

	// <<< const an
	auto an = ctx.selectionStartDir;
	auto cn = mouse_ray_dir;

	const auto up = ctx.canvas.camera.up();
	const auto right = ctx.canvas.camera.right();
	const auto forward = ctx.canvas.camera.forward();

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
//		float x1;
//		if (std::abs(up.x) > 0.5f)
//			x1 = (cn.x - an.x - x2 * right.x) / up.x;
//		else if (std::abs(up.y) > 0.5f)
//			x1 = (cn.y - an.y - x2 * right.y) / up.y;
//		else
//			x1 = (cn.z - an.z - x2 * right.z) / up.z;

	// <<< const bn
	// <<< const dn
	auto bn = (cn - x2 * right);
	auto dn = (an + x2 * right);

	//const auto near = ctx.canvas.camera.near();
	const auto far = ctx.canvas.camera.far() * 0.8f;
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

	std::vector<FleetID> selectedFleetIDs;
	for (auto& fleet : ctx.galaxy.fleets) {
		const auto result = frustum.sphereInFrustum(fleet->position, Fleet::pickingType.radius_universe);

		switch (result) {
		case Frustum::Position::OUTSIDE:
			fleet->selectionStatus = Fleet::Selection::notSelected;
			break;
		case Frustum::Position::INTERSECT:
			[[fallthrough]];
		case Frustum::Position::INSIDE:
			if (commitSelection) {
				selectedFleetIDs.emplace_back(fleet->id);
				fleet->selectionStatus = Fleet::Selection::selected;
			} else
				fleet->selectionStatus = Fleet::Selection::hoverBox;
			break;
		}
	}

	if (commitSelection)
		ctx.playout.process<CTO_FleetSelectBox>(std::move(selectedFleetIDs));

	if (true) {
		ctx.canvas.clear_debug_shapes();
//		libv::vec4f line_color = {0.3f, 0.5f, 0.9f, 0.9f};
//		line_color = {0.9f, 0.2f, 0, 0.9f};
//
//		auto a_near = std::acos(dot(normalize(nbl - nbr), normalize(ntr - nbr)));
//		auto b_near = std::acos(dot(normalize(nbr - ntr), normalize(ntl - ntr)));
//		auto c_near = std::acos(dot(normalize(ntr - ntl), normalize(nbl - ntl)));
//		auto d_near = std::acos(dot(normalize(ntl - nbl), normalize(nbr - nbl)));
//		auto a_far = std::acos(dot(normalize(fbl - fbr), normalize(ftr - fbr)));
//		auto b_far = std::acos(dot(normalize(fbr - ftr), normalize(ftl - ftr)));
//		auto c_far = std::acos(dot(normalize(ftr - ftl), normalize(fbl - ftl)));
//		auto d_far = std::acos(dot(normalize(ftl - fbl), normalize(fbr - fbl)));
//		log_space.info("\na_near: {:6.2f}, b_near: {:6.2f}, c_near: {:6.2f}, d_near: {:6.2f}\na_far : {:6.2f}, b_far : {:6.2f}, c_far : {:6.2f}, d_far : {:6.2f}",
//				libv::rad_to_deg(a_near), libv::rad_to_deg(b_near), libv::rad_to_deg(c_near), libv::rad_to_deg(d_near),
//				libv::rad_to_deg(a_far), libv::rad_to_deg(b_far), libv::rad_to_deg(c_far), libv::rad_to_deg(d_far));
		ctx.canvas.add_debug_frustum(
				nbl, nbr, ntr, ntl, fbl, fbr, ftr, ftl,
				{0.8f, 0, 1, 1}, {0.2f, 0.2f, 0.6f, 0.2f});

		//			{
//				for (int i = -20; i < 20; ++i) {
//					for (int j = -20; j < 20; ++j) {
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
}

} // namespace -------------------------------------------------------------------------------------

void CanvasControl::register_controls(libv::ctrl::FeatureRegister controls) {
	controls.feature_action<CanvasControl>("space.spawn_fleet_at_mouse", [](const auto&, CanvasControl& ctx) {
		const auto world_coord = calculate_world_coord(ctx.canvas);

		// <<< should controls use nexus or playout directly? Think about the console and the lua scripts too.
//		nexus.broadcast<mc::RequestCommandFleetSpawn>(world_coord);

		ctx.playout.process<CTO_FleetSpawn>(world_coord);
	});

	controls.feature_action<CanvasControl>("space.spawn_planet_at_mouse", [](const auto&, CanvasControl& ctx) {
		const auto world_coord = calculate_world_coord(ctx.canvas);

		// <<< should controls use nexus or playout directly? Think about the console and the lua scripts too.
//		nexus.broadcast<mc::RequestCommandFleetSpawn>(world_coord);

		ctx.playout.process<CTO_PlanetSpawn>(world_coord);
	});

	controls.feature_action<CanvasControl>("space.select_fleet", [](const auto&, CanvasControl& ctx) {
		selectionSingle(ctx, true);
	});

	controls.feature_action<CanvasControl>("space.select_fleet_add", [](const auto&, CanvasControl& ctx) {
		auto fleet = calculateHitFleet(ctx.canvas, ctx.galaxy);
		if (fleet) {
			fleet->selectionStatus = Fleet::Selection::selected;
			ctx.playout.process<CTO_FleetSelectAdd>(fleet->id);
		}
	});

	controls.feature_action<CanvasControl>("space.move_fleet_to_mouse", [](const auto&, CanvasControl& ctx) {
		const auto world_coord = calculate_world_coord(ctx.canvas);
		ctx.playout.process<CTO_FleetMove>(world_coord);
	});

	controls.feature_action<CanvasControl>("space.queue_move_fleet_to_mouse", [](const auto&, CanvasControl& ctx) {
		const auto world_coord = calculate_world_coord(ctx.canvas);
		ctx.playout.process<CTO_FleetMoveQueue>(world_coord);
	});

	controls.feature_action<CanvasControl>("space.order_fleet_to_mouse", [](const auto&, CanvasControl& ctx) {
		const auto mouse = calculateHit(ctx.canvas, ctx.galaxy);
		if (mouse.fleetHit) {
			if (ctx.player.faction->canAttack(*mouse.fleetHit->faction))
//			if (ctx.playout.controlledFaction->canAttack(*mouse.fleetHit->faction))
				ctx.playout.process<CTO_FleetAttackFleet>(mouse.fleetHit->id);
			else
				// <<< Not complete, Merge? Land? Go there? go near, or something
				ctx.playout.process<CTO_FleetMove>(mouse.fleetHit->position);

		} else if (mouse.planetHit) {
			if (ctx.player.faction->canAttack(*mouse.planetHit->faction))
				ctx.playout.process<CTO_FleetAttackPlanet>(mouse.planetHit->id);
			else
				// <<< Not complete, Merge? Land? Go there? go near, or something
				ctx.playout.process<CTO_FleetMove>(mouse.planetHit->position);

		} else {
			ctx.playout.process<CTO_FleetMove>(mouse.worldCoord);
		}
	});

	controls.feature_action<CanvasControl>("space.queue_order_fleet_to_mouse", [](const auto&, CanvasControl& ctx) {
		const auto mouse = calculateHit(ctx.canvas, ctx.galaxy);
		if (mouse.fleetHit) {
			if (ctx.player.faction->canAttack(*mouse.fleetHit->faction))
				ctx.playout.process<CTO_FleetAttackFleetQueue>(mouse.fleetHit->id);
			else
				// <<< Not complete, Merge? Land? Go there? go near, or something
				ctx.playout.process<CTO_FleetMoveQueue>(mouse.fleetHit->position);

		} else if (mouse.planetHit) {
			if (ctx.player.faction->canAttack(*mouse.planetHit->faction))
				ctx.playout.process<CTO_FleetAttackPlanetQueue>(mouse.planetHit->id);
			else
				// <<< Not complete, Merge? Land? Go there? go near, or something
				ctx.playout.process<CTO_FleetMoveQueue>(mouse.planetHit->position);

		} else {
			ctx.playout.process<CTO_FleetMoveQueue>(mouse.worldCoord);
		}
	});

	controls.feature_binary<CanvasControl>("space.selection_box_25D", [](libv::ctrl::arg_binary arg, CanvasControl& ctx) {
		if (arg.value) {
			ctx.selectionMode = SelectionMode::d25;

			const auto mouse_local_coord = ctx.canvas.calculate_local_mouse_coord();
			const auto mouse_ray_dir = ctx.canvas.screen_picker.to_world(mouse_local_coord);
			const auto eye = ctx.canvas.camera.eye();
			const auto world_coord = libv::intersect_ray_plane(eye, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

			ctx.selectionStartGridPoint = world_coord;
			ctx.selectionStartDir = mouse_ray_dir;

		} else {
			selection25D(ctx, true);
			ctx.selectionMode = SelectionMode::none;
		}
	});

	controls.feature_binary<CanvasControl>("space.selection_box_3D", [](libv::ctrl::arg_binary arg, CanvasControl& ctx) {
		if (arg.value) {
			ctx.selectionMode = SelectionMode::d3;

			const auto mouse_local_coord = ctx.canvas.calculate_local_mouse_coord();
			const auto mouse_ray_dir = ctx.canvas.screen_picker.to_world(mouse_local_coord);
			const auto eye = ctx.canvas.camera.eye();
			const auto world_coord = libv::intersect_ray_plane(eye, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

			ctx.selectionStartGridPoint = world_coord;
			ctx.selectionStartDir = mouse_ray_dir;

		} else {
			selection3D(ctx, true);
			ctx.selectionMode = SelectionMode::none;
		}
	});

	controls.feature_action<CanvasControl>("space.warp_camera_to_mouse", [](const auto&, CanvasControl& ctx) {
		const auto world_coord = calculate_world_coord(ctx.canvas);
//		const auto userID = UserID{0};
//		ctx.playout.process<CTO_CameraWarpTo>(userID, world_coord);

		ctx.canvas.camera.warp_to(world_coord); // <<< Move this line to CommandCameraWarpTo apply for tracking
	});
}

void CanvasControl::update(CanvasControl& ctx, libv::time_duration delta_time) {
	(void) delta_time;

	switch (ctx.selectionMode) {
	case SelectionMode::none:
		selectionSingle(ctx, false);
		break;
	case SelectionMode::d25:
		selection25D(ctx, false);
		break;
	case SelectionMode::d3:
		selection3D(ctx, false);
		break;
	}
}

void CanvasControl::bind_default_controls(libv::ctrl::BindingRegister controls) {
	// TODO P1: libv.ctrl: analog feature (on time update) bypasses the accidental collusion resolution system (specialization) with an action feature

//	controls.bind("space.move_fleet_to_mouse", "Ctrl + RMB [press]");
//	controls.bind("space.queue_move_fleet_to_mouse", "Ctrl + Shift + RMB [press]");

//	controls.bind("space.order_fleet_to_mouse", "L");
	controls.bind("space.order_fleet_to_mouse", "RMB [press]");
//	controls.bind("space.queue_order_fleet_to_mouse", "Shift + L");
	controls.bind("space.queue_order_fleet_to_mouse", "Shift + RMB [press]");

	controls.bind("space.select_fleet", "LMB [press]");
	controls.bind("space.select_fleet_add", "Shift + LMB [press]");
	controls.bind("space.selection_box_25D", "I");
	controls.bind("space.selection_box_3D", "O");
	controls.bind("space.spawn_fleet_at_mouse", "Ctrl + LMB [press]");
	controls.bind("space.spawn_planet_at_mouse", "Ctrl + B [press]");
	controls.bind("space.spawn_planet_at_mouse", "J [hold]");
	controls.bind("space.warp_camera_to_mouse", "Z");
}

// -------------------------------------------------------------------------------------------------

} // namespace space
