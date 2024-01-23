// Project: libv, File: app/star/game/control/starmap_controller.cpp

#include <star/game/control/starmap_controller.hpp>

#include <libv/ctrl/controls.hpp>
#include <libv/math/distance/intersect.hpp>
#include <libv/math/plane.hpp>

#include <star/game/scene/scene_starmap.hpp>
#include <star/game/sim/universe.hpp>

#include <libv/re/node/node.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

StarmapController::StarmapController(libv::Nexus& nexus) :
	nexus(nexus) {
}

void StarmapController::init(Starmap* starmap, CanvasStarmap* canvas) {
	this->starmap = starmap;
	this->canvas = canvas;
}

StarmapController::~StarmapController() {
	nexus.disconnect_all(this);
}

// -------------------------------------------------------------------------------------------------

void StarmapController::request(RequestStarmapPlaceBallUnderCursor) {
	const auto plane = libv::planef::from_normal_point({0, 0, 1}, {0, 0, 0});

	const auto intersection = libv::intersect_ray_plane(canvas->mouseRay, plane);
	if (!intersection)
		return; // Player visibile error notifiction would go here (or a nexus event)

	starmap->balls.emplace_back(*intersection, 0.5f);
}

void StarmapController::request(RequestStarmapToggleGrid) {
	canvas->editorGrid->visible(!canvas->editorGrid->visible());
}

// -------------------------------------------------------------------------------------------------

void StarmapController::register_controls(libv::ctrl::Controls& controls, StarmapControllerFeatures features) {
	using ControlState = StarmapController;

	const auto act = [&](bool enable, std::string&& name, auto&& reqProto) {
		if (enable) {
			controls.feature_action<ControlState>(std::move(name), [req = std::move(reqProto)](libv::ctrl::arg_action, ControlState& state) {
				state.request(req);
			});
		}
	};

	act(features.placeBall, "starmap.place-ball", RequestStarmapPlaceBallUnderCursor{});
	act(features.placeBall, "starmap.debug.toggle-grid", RequestStarmapToggleGrid{});
}

void StarmapController::bind_default_controls(libv::ctrl::Controls& controls) {
	controls.bind("starmap.place-ball", "M");
	controls.bind("starmap.debug.toggle-grid", "F5");
}

// -------------------------------------------------------------------------------------------------

} // namespace star
