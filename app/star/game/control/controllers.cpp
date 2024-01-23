// Project: libv, File: app/star/game/control/controllers.cpp

#include <star/game/control/controllers.hpp>

#include <libv/ctrl/fwd.hpp>

#include <star/game/control/starmap_controller.hpp>
#include <star/game/control/surface_controller.hpp>
#include <star/game/control/time_controller.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

void registerSPSessionControls(libv::ctrl::Controls& controls) {
	TimeController::register_controls(controls);
	TimeController::bind_default_controls(controls);
	StarmapController::register_controls(controls);
	StarmapController::bind_default_controls(controls);
	SurfaceController::register_controls(controls);
	SurfaceController::bind_default_controls(controls);
}


// -------------------------------------------------------------------------------------------------

} // namespace star
