// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

// libv
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
// std
#include <iostream>
// pro
#include <libv/control/controls.hpp>
#include <libv/control/feature_register.hpp>


libv::control::Controls& global_controls() {
	static libv::control::Controls controls;
	return controls;
}

libv::control::FeatureRegister global_feature_register() {
	return global_controls();
}

using FeatureRegistrationSlow = libv::control::AutomaticFeatureRegister<global_controls>;
using FeatureRegistrationFast = libv::control::AutomaticFeatureRegister<global_feature_register>;

struct Camera {
	double x = 0, y = 0, z = 0;
};

FeatureRegistrationFast _reg_camera_controls([](libv::control::FeatureRegister& registry) {
	registry.feature_action<Camera>("camera.focus", [](libv::control::arg_action, Camera& camera) {
		(void) camera;
		std::cout << "Camera: focus" << std::endl;
	});

	registry.feature_analog<Camera>("camera.move_x", [](libv::control::arg_analog arg, Camera& camera) {
		camera.x += arg.value;
		std::cout << "Camera: X=" << camera.x << std::endl;
	});

	registry.feature_analog<Camera>("camera.move_y", [](libv::control::arg_analog arg, Camera& camera) {
		camera.x += arg.value;
		std::cout << "Camera: Y=" << camera.y << std::endl;
	});

	registry.feature_analog<Camera>("camera.move_z", [](libv::control::arg_analog arg, Camera& camera) {
		camera.x += arg.value;
		std::cout << "Camera: Z=" << camera.z << std::endl;
	});
});

// ---

int main() {
	std::cout << libv::logger_stream;

//	open_debug_frame();

	libv::control::Controls controls;

	controls.feature_action<void>("sandbox.hello", [](libv::control::arg_action) {
		std::cout << "Action: Hello" << std::endl;
	});

	controls.feature_action<void>("sandbox.bye", [](libv::control::arg_action) {
		std::cout << "Action: Bye" << std::endl;
	});

	controls.feature_binary<void>("sandbox.binary", [](libv::control::arg_binary arg) {
		std::cout << "Binary: " << arg.value << std::endl;
	});

	controls.feature_analog<void>("sandbox.analog", [](libv::control::arg_analog arg) {
		std::cout << "Analog: " << arg.value << std::endl;
	});

	controls.scale_gamepad_analog(1.5);
	controls.scale_gamepad_analog(libv::control::GamepadID{10}, 2.0, 2.5);
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

	controls.input(libv::input::EventKey{libv::input::Key::A, libv::input::Scancode{30}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.input(libv::input::EventKey{libv::input::Key::A, libv::input::Scancode{30}, libv::input::Action::release, libv::input::KeyModifier::none});
	controls.input(libv::input::EventKey{libv::input::Key::S, libv::input::Scancode{31}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.input(libv::input::EventKey{libv::input::Key::S, libv::input::Scancode{31}, libv::input::Action::release, libv::input::KeyModifier::none});
	controls.input(libv::input::EventKey{libv::input::Key::D, libv::input::Scancode{32}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.input(libv::input::EventKey{libv::input::Key::D, libv::input::Scancode{32}, libv::input::Action::release, libv::input::KeyModifier::none});
	controls.input(libv::input::EventKey{libv::input::Key::D, libv::input::Scancode{32}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.input(libv::input::EventKey{libv::input::Key::D, libv::input::Scancode{32}, libv::input::Action::release, libv::input::KeyModifier::none});
	controls.input(libv::input::EventKey{libv::input::Key::F, libv::input::Scancode{33}, libv::input::Action::press, libv::input::KeyModifier::none});
	controls.update(std::chrono::microseconds{16667});
	controls.input(libv::input::EventKey{libv::input::Key::F, libv::input::Scancode{33}, libv::input::Action::release, libv::input::KeyModifier::none});

	return EXIT_SUCCESS;
}
