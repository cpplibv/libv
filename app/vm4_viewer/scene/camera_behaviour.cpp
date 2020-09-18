// Project: libv, File: app/vm4_viewer/scene/camera_behaviour.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <vm4_viewer/scene/camera_behaviour.hpp>
// libv
#include <libv/ctrl/controls.hpp> // TODO P0: temporary for binds
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/angle.hpp>
// pro
#include <vm4_viewer/scene/camera_2.hpp>
//#include <vm4_viewer/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void CameraBehaviour::register_controls(libv::ctrl::FeatureRegister& controls) {

	// °/impulse °/600px °/s
	libv::ctrl::scale_group sg_rotate{libv::to_rad(5.0), libv::to_rad(140.0), libv::to_rad(80.0)};
	// unit/impulse, unit/600px, unit/s
	libv::ctrl::scale_group sg_translate{0.1, 1.0, 1.0};
	// unit/impulse, unit/600px, unit/s
	libv::ctrl::scale_group sg_orbit{0.5, 9.0, 8.0};

	static constexpr auto c0_orbit = 1.15f;
	static constexpr auto c0_translate = 1.1f; (void) c0_translate; // (Suppressing a false warning)


//	controls.feature_analog<Camera2>("camera.roll", sg_rotate, [](const auto& arg, Camera2& ctx) {
//		ctx.roll(arg.value_f());
//	});
//
//	controls.feature_analog<Camera2>("camera.pitch", sg_rotate, [](const auto& arg, Camera2& ctx) {
//		ctx.pitch(arg.value_f());
//	});
//
//	controls.feature_analog<Camera2>("camera.yaw", sg_rotate, [](const auto& arg, Camera2& ctx) {
//		ctx.yaw(arg.value_f());
//	});


	controls.feature_analog<Camera2>("camera.rotate_x", sg_rotate, [](const auto& arg, Camera2& ctx) {
		const auto value = arg.value_f();
		ctx.rotate_x(value);
	});

	controls.feature_analog<Camera2>("camera.rotate_y", sg_rotate, [](const auto& arg, Camera2& ctx) {
		const auto value = arg.value_f();
		ctx.rotate_y(value);
	});

	controls.feature_analog<Camera2>("camera.rotate_z", sg_rotate, [](const auto& arg, Camera2& ctx) {
		const auto value = arg.value_f();
		ctx.rotate_z(value);
	});


	controls.feature_analog<Camera2>("camera.translate_x", sg_translate, [](const auto& arg, Camera2& ctx) {
		const auto value = arg.value_f() * std::pow(ctx.orbit(), c0_translate);
		ctx.translate_x(value);
	});

	controls.feature_analog<Camera2>("camera.translate_y", sg_translate, [](const auto& arg, Camera2& ctx) {
		const auto value = arg.value_f() * std::pow(ctx.orbit(), c0_translate);
		ctx.translate_y(value);
	});

	controls.feature_analog<Camera2>("camera.translate_z", sg_translate, [](const auto& arg, Camera2& ctx) {
		const auto value = arg.value_f() * std::pow(ctx.orbit(), c0_translate);
		ctx.translate_z(value);
	});


	controls.feature_analog<Camera2>("camera.pivot_orbit", sg_orbit, [](const auto& arg, Camera2& ctx) {
		const auto linear_orbit = std::pow(ctx.orbit(), 1.0f / c0_orbit);
		const auto value = std::pow(linear_orbit + arg.value_f(), c0_orbit);

		ctx.orbit_to(std::max(value, ctx.near()));
	});
}

void CameraBehaviour::bind_controls(libv::ctrl::Controls& controls) {
	controls.bind("camera.pivot_orbit", "T");
	controls.bind("camera.pivot_orbit", "G", -1);
	controls.bind("camera.pivot_orbit", "Scroll", -0.1);
	controls.bind("camera.pivot_orbit", "LMB + RMB + Mouse Y", -1);
	controls.bind("camera.pivot_orbit", "LMB + RMB + Mouse X", 0);
	// TODO P5: libv.ctrl: ^^^ Bug, if the selected dim is 0, the change in other dims are not subject to binding specialziation selection | solution will be to dont discard 0 in selection just only after gather complete

	controls.bind("camera.rotate_x", "W");
	controls.bind("camera.rotate_x", "S", -1);
	controls.bind("camera.rotate_x", "LMB + Mouse Y", -1);

	controls.bind("camera.rotate_y", "Q", -1);
	controls.bind("camera.rotate_y", "E");

	controls.bind("camera.rotate_z", "A");
	controls.bind("camera.rotate_z", "D", -1);
	controls.bind("camera.rotate_z", "LMB + Mouse X");

	controls.bind("camera.translate_x", "Up", -1);
	controls.bind("camera.translate_x", "Down");
	controls.bind("camera.translate_x", "RMB + Mouse Y");

	controls.bind("camera.translate_y", "Left", -1);
	controls.bind("camera.translate_y", "Right");
	controls.bind("camera.translate_y", "RMB + Mouse X", -1);

	controls.bind("camera.translate_z", "Y");
	controls.bind("camera.translate_z", "H", -1);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
