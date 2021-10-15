// Project: libv.ctrl, File: sandbox/libv_ctrl_main.cpp

// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/frame/frame.hpp>
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
#include <libv/log/log.hpp>
#include <libv/utility/to_underlying.hpp>
// std
#include <iostream>
// pro
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

// -------------------------------------------------------------------------------------------------

struct Sandbox {
	Sandbox() {
//		const auto keyboard_spin_speed = iris::to_rad(60.f); // 60°/s
//		const auto mouse_spin_speed = iris::to_rad(60.f) * 0.0025f; // 60°/400px
//		const auto keyboard_move_speed = zoom * 0.5f + 1.0f;
//		const auto keyboard_zoom_scale = zoom * 0.2f + 0.1f;
//		const auto mouse_move_speed = zoom * 0.0006f + 0.05f;
//		const auto mouse_scroll_scale = zoom * 0.08f + 0.1f;
//
//
//		hotkey("camera.pitch_down", [] {
//			camera->rotateX(keyboard_spin_speed * elapsed_time);
//		});
//		hotkey("camera.pitch_up", [] {
//			camera->rotateX(-keyboard_spin_speed * elapsed_time);
//		});
//		hotkey("camera.yaw_left", [] {
//			camera->rotateZ(keyboard_spin_speed * elapsed_time);
//		});
//		hotkey("camera.yaw_right", [] {
//			camera->rotateZ(-keyboard_spin_speed * elapsed_time);
//		});
//		hotkey("camera.roll_cw", [] {
//			camera->rotateY(keyboard_spin_speed * elapsed_time);
//		});
//		hotkey("camera.roll_ccw", [] {
//			camera->rotateY(-keyboard_spin_speed * elapsed_time);
//		});
//
//		using namespace libv::frame;
//
//		bind("camera.pitch_down", Keycode::W);
//		bind("camera.pitch_up", Keycode::S);
//		bind("camera.yaw_left", Keycode::A);
//		bind("camera.yaw_right", Keycode::D);
//		bind("camera.roll_cw", Keycode::E);
//		bind("camera.roll_ccw", Keycode::Q);
//		bind("camera.move_forward", Keycode::Up);
//		bind("camera.move_backward", Keycode::Down);
//		bind("camera.move_left", Keycode::Left);
//		bind("camera.move_right", Keycode::Right);
//		bind("camera.move_up", Keycode::R);
//		bind("camera.move_down", Keycode::F);
//		bind("camera.zoom_move_in", Keycode::T);
//		bind("camera.zoom_move_out", Keycode::G);
//
//		bind("camera.move_forward", Keycode::ControlLeft & Mouse::Y);
//		bind("camera.move_backward", Keycode::ControlLeft & -Mouse::Y);
//		bind("camera.move_left", Keycode::ControlLeft & Mouse::X);
//		bind("camera.move_right", Keycode::ControlLeft & -Mouse::X);
//
////		bind("game.building.redo", Keycode::ControlLeft + Keycode::Y);
////		bind("game.building.undo", Keycode::ControlLeft + Keycode::Z);
////		bind("game.fleet.redo", Keycode::ControlLeft + Keycode::Y);
////		bind("game.fleet.undo", Keycode::ControlLeft + Keycode::Z);
//
//		bind("text.redo", Keycode::ControlLeft + Keycode::Y);
//		bind("text.undo", Keycode::ControlLeft + Keycode::Z);
//		bind("text.edit.copy", Keycode::ControlLeft + Keycode::C);
//		bind("text.edit.cut", Keycode::ControlLeft + Keycode::X);
//		bind("text.edit.paste", Keycode::ControlLeft + Keycode::V);
//		bind("text.edit.to_lower", Keycode::ControlLeft + Keycode::U, Keycode::L);
//		bind("text.edit.to_upper", Keycode::ControlLeft + Keycode::U, Keycode::U);
//		bind("text.search", Keycode::ControlLeft + Keycode::F);
//		bind("text.selection.expand_word_left", Keycode::ControlLeft + Keycode::ShiftLeft + Keycode::Left);
//		bind("text.selection.expand_word_right", Keycode::ControlLeft + Keycode::ShiftLeft + Keycode::Right);
//
//		if (frame->isKeyPressed(libv::input::Keycode::Up))
//			camera->translateY(-keyboard_move_speed * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Keycode::Down))
//			camera->translateY(keyboard_move_speed * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Keycode::Right))
//			camera->translateX(keyboard_move_speed * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Keycode::Left))
//			camera->translateX(-keyboard_move_speed * elapsed_time);
//
//		if (frame->isKeyPressed(libv::input::Keycode::R))
//			camera->translateZ(keyboard_move_speed * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Keycode::F))
//			camera->translateZ(-keyboard_move_speed * elapsed_time);
//
//		if (frame->isKeyPressed(libv::input::Keycode::T))
//			camera->translateZoom(-keyboard_zoom_scale * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Keycode::G))
//			camera->translateZoom(+keyboard_zoom_scale * elapsed_time);
	}
};

// -------------------------------------------------------------------------------------------------

void init(libv::Frame& frame) {
	const auto pretty_print_to_log = [](const auto& event) {
		log_sandbox.trace("{}", event.toPrettyString());
	};

	frame.onChar.output(pretty_print_to_log);
//	frame.onKey.output(pretty_print_to_log);
	frame.onMouseButton.output(pretty_print_to_log);
	frame.onMouseScroll.output(pretty_print_to_log);
	frame.onMousePosition.output(pretty_print_to_log);

	frame.onKey.output([&frame](const libv::input::EventKey& e) {
		if (e.keycode == libv::input::Keycode::Escape)
			frame.closeDefault();
	});

//	frame.onKey.output([&frame](const libv::input::EventKey& e) {
//		if (e.action != libv::input::Action::release)
//			;
//	});

	frame.onKey.output([](const libv::input::EventKey& e) {
		const int scancode = e.keycode != libv::input::Keycode::Unknown ? glfwGetKeyScancode(libv::to_value(e.keycode)) : -1;
		const char* name = glfwGetKeyName(libv::to_value(e.keycode), libv::to_value(e.scancode));
		log_sandbox.trace("{}, rescan = {}, name = {}, rescan-match = {}",
				e.toPrettyString(), scancode, name ? name : "unknown", scancode == libv::to_value(e.scancode));
	});

	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");
	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");
	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");
	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");

	for (int i = 0; i <= 350; ++i) {
		auto scancode = glfwGetKeyScancode(i);
		if (scancode == -1)
			continue;

		const char* name = glfwGetKeyName(-1, scancode);
		if (name == nullptr)
			continue;

		log_sandbox.trace("{}, key: {} scan: {}, name: {}", i, libv::input::to_string(libv::input::Keycode{i}), scancode, name ? name : "unknown");
	}

	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");
	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");
	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");
	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");

	for (int i = 0; i <= 65536; ++i) {
		const char* name = glfwGetKeyName(i, -1);

		if (name != nullptr)
			log_sandbox.trace("{}, key: {} name: {}", i, libv::input::to_string(libv::input::Keycode{i}), name);
	}

//	frame.onMouseButton.output(pretty_print_to_log);
//	frame.onMousePosition.output(pretty_print_to_log);
//	frame.onMouseScroll.output(pretty_print_to_log);
}

// -------------------------------------------------------------------------------------------------

void open_debug_frame() {
	libv::Frame frame("Input event window", 900, 600);

	init(frame);

	frame.setPosition(libv::Frame::FramePosition::center_current_monitor);
	frame.show();
	frame.join();
}

// -------------------------------------------------------------------------------------------------

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

	open_debug_frame();

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

	return EXIT_SUCCESS;
}
