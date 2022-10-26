// Project: libv, File: app/surface/view/camera_control.hpp

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct CameraControl {
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::BindingRegister controls);
};

struct CameraControl2D {
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::BindingRegister controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
