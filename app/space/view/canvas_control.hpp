// Project: libv, File: app/space/view/canvas_control.hpp

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/utility/chrono.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class SpaceCanvas;

struct CanvasControl {
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void update(SpaceCanvas& ctx, libv::time_duration delta_time);
	static void bind_default_controls(libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
