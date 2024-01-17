//

#include <star/game/control/surface_controller.hpp>

#include <libv/ctrl/controls.hpp>
#include <libv/math/distance/intersect.hpp>
#include <libv/math/plane.hpp>

#include <star/game/scene/scene_surface.hpp>
#include <star/log.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

SurfaceController::SurfaceController(libv::Nexus& nexus) :
	nexus(nexus) {
}

void SurfaceController::init(Surface* surface, CanvasSurface* canvas) {
	this->surface = surface;
	this->canvas = canvas;
}

SurfaceController::~SurfaceController() {
	nexus.disconnect_all(this);
}

// -------------------------------------------------------------------------------------------------

void SurfaceController::request(RequestSurfacePlaceBallUnderCursor) {
	const auto plane = libv::planef::from_normal_point({0, 0, 1}, {0, 0, 0});

	const auto intersection = libv::intersect_ray_plane(canvas->mouseRay, plane);
	if (!intersection)
		return;

	surface->balls.emplace_back(*intersection, 0.5f);
}

// -------------------------------------------------------------------------------------------------

void SurfaceController::register_controls(libv::ctrl::Controls& controls, SurfaceControllerFeatures features) {
	using ControlState = SurfaceController;

	const auto act = [&](bool enable, std::string&& name, auto&& reqProto) {
		if (enable) {
			controls.feature_action<ControlState>(std::move(name), [req = std::move(reqProto)](libv::ctrl::arg_action, ControlState& state) {
				state.request(req);
			});
		}
	};

	act(features.placeBall, "surface.place-ball", RequestSurfacePlaceBallUnderCursor{});
}

void SurfaceController::bind_default_controls(libv::ctrl::Controls& controls) {
	controls.bind("surface.place-ball", "M");
}

// -------------------------------------------------------------------------------------------------

} // namespace star
