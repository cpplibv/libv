// Project: libv, File: app/space/view/canvas_control.hpp

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct CanvasControl {
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
