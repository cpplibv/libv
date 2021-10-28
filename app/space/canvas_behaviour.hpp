// Project: libv, File: app/space/canvas_behaviour.hpp

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct CanvasBehaviour {
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
