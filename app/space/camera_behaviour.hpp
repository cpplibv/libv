// Project: libv, File: app/space/camera_behaviour.hpp

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct CameraBehaviour {
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
