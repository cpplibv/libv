// Project: libv.ctrl, File: example/ctrl/utility/main.cpp

// libv
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
// std
#include <iostream>
// pro
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>


libv::ctrl::Controls& global_controls() {
	static libv::ctrl::Controls controls;
	return controls;
}

libv::ctrl::FeatureRegister global_feature_register() {
	return global_controls();
}

using FeatureRegistrationSlow = libv::ctrl::AutomaticFeatureRegister<global_controls>;
using FeatureRegistrationFast = libv::ctrl::AutomaticFeatureRegister<global_feature_register>;

struct Camera {
	double x = 0, y = 0, z = 0;
};

FeatureRegistrationFast _reg_camera_controls([](libv::ctrl::FeatureRegister& registry) {
	registry.feature_action<Camera>("camera.focus", [](libv::ctrl::arg_action, Camera& camera) {
		(void) camera;
		std::cout << "Camera: focus" << std::endl;
	});

	registry.feature_analog<Camera>("camera.move_x", [](libv::ctrl::arg_analog arg, Camera& camera) {
		camera.x += arg.value;
		std::cout << "Camera: X=" << camera.x << std::endl;
	});

	registry.feature_analog<Camera>("camera.move_y", [](libv::ctrl::arg_analog arg, Camera& camera) {
		camera.x += arg.value;
		std::cout << "Camera: Y=" << camera.y << std::endl;
	});

	registry.feature_analog<Camera>("camera.move_z", [](libv::ctrl::arg_analog arg, Camera& camera) {
		camera.x += arg.value;
		std::cout << "Camera: Z=" << camera.z << std::endl;
	});
});

// ---

int main() {
	std::cout << libv::logger_stream;

//	open_debug_frame();

	libv::ctrl::Controls controls;

	controls.feature_action<void>("sandbox.hello", [](libv::ctrl::arg_action) {
		std::cout << "Action: Hello" << std::endl;
	});

	controls.feature_action<void>("sandbox.bye", [](libv::ctrl::arg_action) {
		std::cout << "Action: Bye" << std::endl;
	});

	controls.feature_binary<void>("sandbox.binary", [](libv::ctrl::arg_binary arg) {
		std::cout << "Binary: " << arg.value << std::endl;
	});

	controls.feature_analog<void>("sandbox.analog", [](libv::ctrl::arg_analog arg) {
		std::cout << "Analog: " << arg.value << std::endl;
	});

	controls.scale_gamepad_analog(1.5);
	controls.scale_gamepad_analog(libv::ctrl::GamepadID{10}, 2.0, 2.5);
	controls.scale_mouse_move(3.0, 1.0);

	controls.bind("sandbox.hello", "A");
	controls.bind("sandbox.bye", "S");
	controls.bind("sandbox.binary", "D");
	controls.bind("sandbox.analog", "F");
	controls.bind("sandbox.analog", "Ctrl + G", 2.0);
	controls.bind("test.missing", "Ctrl + B[press]");

	std::cout << "---" << std::endl;
	std::cout << controls.export_bindings() << std::endl;
	std::cout << controls.export_settings() << std::endl;
	std::cout << "---" << std::endl;
	global_controls().bind("test.missing.2", "Space");
	global_controls().import_bindings(controls.export_bindings());
	global_controls().import_settings(controls.export_settings());
	std::cout << global_controls().export_bindings() << std::endl;
	std::cout << global_controls().export_settings() << std::endl;
	std::cout << "---" << std::endl;

	controls.event(libv::input::EventKey{libv::input::Key::A, libv::input::Scancode{30}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.event(libv::input::EventKey{libv::input::Key::A, libv::input::Scancode{30}, libv::input::Action::release, libv::input::KeyModifier::none});
	controls.event(libv::input::EventKey{libv::input::Key::S, libv::input::Scancode{31}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.event(libv::input::EventKey{libv::input::Key::S, libv::input::Scancode{31}, libv::input::Action::release, libv::input::KeyModifier::none});
	controls.event(libv::input::EventKey{libv::input::Key::D, libv::input::Scancode{32}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.event(libv::input::EventKey{libv::input::Key::D, libv::input::Scancode{32}, libv::input::Action::release, libv::input::KeyModifier::none});
	controls.event(libv::input::EventKey{libv::input::Key::D, libv::input::Scancode{32}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.event(libv::input::EventKey{libv::input::Key::D, libv::input::Scancode{32}, libv::input::Action::release, libv::input::KeyModifier::none});
	controls.event(libv::input::EventKey{libv::input::Key::F, libv::input::Scancode{33}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.update(std::chrono::microseconds{16667});
	controls.event(libv::input::EventKey{libv::input::Key::F, libv::input::Scancode{33}, libv::input::Action::release, libv::input::KeyModifier::none});

	return EXIT_SUCCESS;
}
