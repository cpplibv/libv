// Project: libv, File: app/space/view/scene_main_control.hpp

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct SceneMainControl {
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace app