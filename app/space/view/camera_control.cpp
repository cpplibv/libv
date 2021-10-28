// Project: libv, File: app/space/view/camera_behaviour.cpp

// hpp
#include <space/view/camera_control.hpp>
// libv
#include <libv/ctrl/controls.hpp> // TODO P0: temporary for default binds
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/angle.hpp>
// std
#include <cmath>
// pro
#include <space/view/camera.hpp>
#include <space/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void CameraControl::register_controls(libv::ctrl::FeatureRegister controls) {
	// TODO P2: libv.ctrl: These groups are kind of universal, make them libv.ctrl level constant with a multiplier support over them
	//                  So one could just write: auto my_sg_rotate = ctrl::sg_rotate * 2.0;
	//                  Include groups for:
	//                      camera_rotate, camera_move, camera_zoom, number_slider, scroll_bar

	libv::ctrl::scale_group sg_rotate{
			.impulse = libv::to_rad(5.0),
			.time = libv::to_rad(80.0),
			.mouse = libv::to_rad(140.0 / 600.0),
			.scroll = libv::to_rad(5.0),
			.gp_analog = libv::to_rad(80.0),
			.js_analog = libv::to_rad(80.0)
	};

	libv::ctrl::scale_group sg_translate{
			.impulse = 0.1,
			.time = 1.0,
			.mouse = 1.0 / 600.0,
			.scroll = 0.1,
			.gp_analog = 1.0,
			.js_analog = 1.0
	};

	libv::ctrl::scale_group sg_orbit{
			.impulse = 0.5,
			.time = 4.0,
			.mouse = 4.0 / 600.0,
			.scroll = 0.20,
			.gp_analog = 4.0,
			.js_analog = 4.0
	};

	// TODO P3: Refine c0 constants in camera control
	static constexpr auto c0_orbit = 1.15f;
	static constexpr auto c0_translate = 1.1f; (void) c0_translate; // Suppressing a false warning

	controls.feature_analog<BaseCameraOrbit>("camera.orbit_roll", sg_rotate, [](const auto& arg, BaseCameraOrbit& ctx) {
		const auto value = arg.value_f();
		ctx.orbit_roll(value);
	});

	controls.feature_analog<BaseCameraOrbit>("camera.orbit_pitch", sg_rotate, [](const auto& arg, BaseCameraOrbit& ctx) {
		const auto value = arg.value_f();
		ctx.orbit_pitch(value);
	});

	controls.feature_analog<BaseCameraOrbit>("camera.orbit_yaw", sg_rotate, [](const auto& arg, BaseCameraOrbit& ctx) {
		const auto value = arg.value_f();
		ctx.orbit_yaw(value);
	});


	controls.feature_analog<BaseCameraOrbit>("camera.move_forward", sg_translate, [](const auto& arg, BaseCameraOrbit& ctx) {
		const auto value = arg.value_f() * std::pow(ctx.orbit_distance(), c0_translate);
		ctx.move_forward(value);
	});

	controls.feature_analog<BaseCameraOrbit>("camera.move_right", sg_translate, [](const auto& arg, BaseCameraOrbit& ctx) {
		const auto value = arg.value_f() * std::pow(ctx.orbit_distance(), c0_translate);
		ctx.move_right(value);
	});

	controls.feature_analog<BaseCameraOrbit>("camera.move_up", sg_translate, [](const auto& arg, BaseCameraOrbit& ctx) {
		const auto value = arg.value_f() * std::pow(ctx.orbit_distance(), c0_translate);
		ctx.move_up(value);
	});


	controls.feature_analog<BaseCameraOrbit>("camera.orbit_distance", sg_orbit, [](const auto& arg, BaseCameraOrbit& ctx) {
		const auto linear_orbit = std::pow(ctx.orbit_distance(), 1.0f / c0_orbit);
		const auto is_neg = linear_orbit + arg.value_f() < 0;
		const auto value = is_neg ? 0.0f : std::pow(linear_orbit + arg.value_f(), c0_orbit);

		ctx.orbit_distance(std::clamp(value, ctx.near() * 2.0f, ctx.far() * 0.5f));
	});


	controls.feature_action<BaseCameraOrbit>("camera.snap_angle", [](const auto&, BaseCameraOrbit& camera) {
		const auto snap_angle = 8.f / libv::tau; // 8 snapping point per circles = 45°

		camera.roll(std::round(camera.roll() * snap_angle) / snap_angle);
		camera.pitch(std::round(camera.pitch() * snap_angle) / snap_angle);
		camera.yaw(std::round(camera.yaw() * snap_angle) / snap_angle);
	});

	controls.feature_action<BaseCameraOrbit>("camera.snap_position", [](const auto&, BaseCameraOrbit& camera) {
		const auto snap_position = 1.f;

		camera.orbit_point(libv::vec::round(camera.orbit_point() * snap_position) / snap_position);
		camera.orbit_distance(std::round(camera.orbit_distance() * snap_position) / snap_position);
	});

	controls.feature_action<BaseCameraOrbit>("camera.debug", [](const auto&, BaseCameraOrbit& camera) {
		log_space.info("Camera:"
					   "\n\tEye     {}"
					   "\n\tForward {}"
					   "\n\tRight   {}"
					   "\n\tUp      {}", camera.eye(), camera.forward(), camera.right(), camera.up());
	});
}

void CameraControl::bind_default_controls(libv::ctrl::Controls& controls) {
	controls.bind("camera.orbit_distance", "T", -1);
	controls.bind("camera.orbit_distance", "G");
	controls.bind("camera.orbit_distance", "Scroll", -1);
	controls.bind("camera.orbit_distance", "LMB + RMB + Mouse Y", -1);
	controls.bind("camera.orbit_distance", "LMB + RMB + Mouse X", 0);
	// TODO P5: libv.ctrl: ^^^ Bug, if the selected dim is 0, the change in other dims are not subject to binding specialziation selection | solution will be to dont discard 0 in selection just only after gather complete
	// TODO P4: libv.ctrl: Implicit "noop" feature that is not visible in introspection but can be used as a dummy target

	controls.bind("camera.orbit_pitch", "W", -1);
	controls.bind("camera.orbit_pitch", "S");
	controls.bind("camera.orbit_pitch", "LMB + Mouse Y");

	controls.bind("camera.orbit_roll", "Q", -1);
	controls.bind("camera.orbit_roll", "E");

	controls.bind("camera.orbit_yaw", "A");
	controls.bind("camera.orbit_yaw", "D", -1);
	controls.bind("camera.orbit_yaw", "LMB + Mouse X");

	controls.bind("camera.move_forward", "Up");
	controls.bind("camera.move_forward", "Down", -1);
	controls.bind("camera.move_forward", "RMB + Mouse Y", -1);

	controls.bind("camera.move_right", "Right");
	controls.bind("camera.move_right", "Left", -1);
	controls.bind("camera.move_right", "RMB + Mouse X", -1);

	controls.bind("camera.move_up", "Y");
	controls.bind("camera.move_up", "H", -1);
	controls.bind("camera.move_up", "MMB + Mouse Y");

	// Debug / dev tools:
	controls.bind("camera.debug", "Shift + Ctrl + P");
	controls.bind("camera.snap_position", "Shift + Ctrl + O");
	controls.bind("camera.snap_angle", "Shift + Ctrl + I");

	controls.bind("camera.snap_angle", "Shift + Ctrl + U");
	controls.bind("camera.snap_position", "Shift + Ctrl + U");
}

// -------------------------------------------------------------------------------------------------

} // namespace app
