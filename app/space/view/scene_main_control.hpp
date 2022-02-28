// Project: libv, File: app/space/view/scene_main_control.hpp

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct SceneMainControl {
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::BindingRegister controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
