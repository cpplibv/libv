// Project: libv.ctrl, File: sandbox/libv_ctrl_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/input/input.hpp>
#include <libv/input/event.hpp>
// std
#include <iostream>
// pro
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>


// ---

void foo() {
	std::cout << "dll" << std::endl;

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
//	global_controls().bind("test.missing.2", "Space");
//	global_controls().import_bindings(controls.export_bindings());
//	global_controls().import_settings(controls.export_settings());
//	std::cout << global_controls().export_bindings() << std::endl;
//	std::cout << global_controls().export_settings() << std::endl;
//	std::cout << "---" << std::endl;

	controls.event(libv::input::EventKey{libv::input::Keycode::A, libv::input::Scancode{30}, libv::input::Action::press});
	controls.event(libv::input::EventKey{libv::input::Keycode::A, libv::input::Scancode{30}, libv::input::Action::release});
	controls.event(libv::input::EventKey{libv::input::Keycode::S, libv::input::Scancode{31}, libv::input::Action::press});
	controls.event(libv::input::EventKey{libv::input::Keycode::S, libv::input::Scancode{31}, libv::input::Action::release});
	controls.event(libv::input::EventKey{libv::input::Keycode::D, libv::input::Scancode{32}, libv::input::Action::press});
	controls.event(libv::input::EventKey{libv::input::Keycode::D, libv::input::Scancode{32}, libv::input::Action::release});
	controls.event(libv::input::EventKey{libv::input::Keycode::D, libv::input::Scancode{32}, libv::input::Action::press});
	controls.event(libv::input::EventKey{libv::input::Keycode::D, libv::input::Scancode{32}, libv::input::Action::release});
	controls.event(libv::input::EventKey{libv::input::Keycode::F, libv::input::Scancode{33}, libv::input::Action::press});
	controls.update(std::chrono::microseconds{16667});
	controls.event(libv::input::EventKey{libv::input::Keycode::F, libv::input::Scancode{33}, libv::input::Action::release});

}
