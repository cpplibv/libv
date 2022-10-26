// Project: libv.sun, File: src/libv/sun/camera_control.hpp

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>


namespace libv {
namespace sun {

// -------------------------------------------------------------------------------------------------

struct CameraControl {
	static constexpr const char* fn_camera_orbit_distance = "camera.orbit_distance";
	static constexpr const char* fn_camera_orbit_pitch = "camera.orbit_pitch";
	static constexpr const char* fn_camera_orbit_roll = "camera.orbit_roll";
	static constexpr const char* fn_camera_orbit_yaw = "camera.orbit_yaw";
	static constexpr const char* fn_camera_move_forward = "camera.move_forward";
	static constexpr const char* fn_camera_move_right = "camera.move_right";
	static constexpr const char* fn_camera_move_up = "camera.move_up";
	static constexpr const char* fn_camera_debug = "camera.debug";
	static constexpr const char* fn_camera_snap_position = "camera.snap_position";
	static constexpr const char* fn_camera_snap_angle = "camera.snap_angle";

	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::BindingRegister controls);
};

struct CameraControl2D {
	static constexpr const char* fn_camera_zoom = "camera2D.zoom";
	static constexpr const char* fn_camera_move_forward = "camera2D.move_forward";
	static constexpr const char* fn_camera_move_right = "camera2D.move_right";

	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::BindingRegister controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace sun
} // namespace star
