// Project: libv, File: app/vm4_viewer/scene/camera_behaviour.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/camera_behaviour.hpp>
// libv
#include <libv/ctrl/controls.hpp> // TODO P0: temporary for binds
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/angle.hpp>
// pro
#include <space/camera.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void CameraBehaviour::register_controls(libv::ctrl::FeatureRegister controls) {

	// degree/impulse degree/600px degree/s
	libv::ctrl::scale_group sg_rotate{libv::to_rad(5.0), libv::to_rad(140.0), libv::to_rad(80.0)};
	// unit/impulse, unit/600px, unit/s
	libv::ctrl::scale_group sg_translate{0.1, 1.0, 1.0};
	// unit/impulse, unit/600px, unit/s
	libv::ctrl::scale_group sg_orbit{0.5, 8.0, 4.0};

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
}

void CameraBehaviour::bind_default_controls(libv::ctrl::Controls& controls) {
	controls.bind("camera.orbit_distance", "T", -1);
	controls.bind("camera.orbit_distance", "G");
	// TODO P1: Solve the scroll impulse value on analog feature
	//			| I want mouse scroll
	//	            - to affect camera movement less
	//			    - but still affect the sliders and number inputs the same (unaltered) way
	//          | solution will be additional entries for features in scale grounds
	controls.bind("camera.orbit_distance", "Scroll", -0.05);
	// TODO P4: libv.ctrl: Scale groups would replace ^^^   and   vvvv constants with -1.0
	controls.bind("camera.orbit_distance", "LMB + RMB + Mouse Y", -0.6);
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
}

// -------------------------------------------------------------------------------------------------

} // namespace app
