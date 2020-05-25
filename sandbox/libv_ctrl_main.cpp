// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

/*

// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/frame/frame.hpp>
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
#include <libv/log/log.hpp>
#include <libv/utility/enum.hpp>
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
//		bind("camera.pitch_down", Key::W);
//		bind("camera.pitch_up", Key::S);
//		bind("camera.yaw_left", Key::A);
//		bind("camera.yaw_right", Key::D);
//		bind("camera.roll_cw", Key::E);
//		bind("camera.roll_ccw", Key::Q);
//		bind("camera.move_forward", Key::Up);
//		bind("camera.move_backward", Key::Down);
//		bind("camera.move_left", Key::Left);
//		bind("camera.move_right", Key::Right);
//		bind("camera.move_up", Key::R);
//		bind("camera.move_down", Key::F);
//		bind("camera.zoom_move_in", Key::T);
//		bind("camera.zoom_move_out", Key::G);
//
//		bind("camera.move_forward", Key::ControlLeft & Mouse::Y);
//		bind("camera.move_backward", Key::ControlLeft & -Mouse::Y);
//		bind("camera.move_left", Key::ControlLeft & Mouse::X);
//		bind("camera.move_right", Key::ControlLeft & -Mouse::X);
//
////		bind("game.building.redo", Key::ControlLeft + Key::Y);
////		bind("game.building.undo", Key::ControlLeft + Key::Z);
////		bind("game.fleet.redo", Key::ControlLeft + Key::Y);
////		bind("game.fleet.undo", Key::ControlLeft + Key::Z);
//
//		bind("text.redo", Key::ControlLeft + Key::Y);
//		bind("text.undo", Key::ControlLeft + Key::Z);
//		bind("text.edit.copy", Key::ControlLeft + Key::C);
//		bind("text.edit.cut", Key::ControlLeft + Key::X);
//		bind("text.edit.paste", Key::ControlLeft + Key::V);
//		bind("text.edit.to_lower", Key::ControlLeft + Key::U, Key::L);
//		bind("text.edit.to_upper", Key::ControlLeft + Key::U, Key::U);
//		bind("text.search", Key::ControlLeft + Key::F);
//		bind("text.selection.expand_word_left", Key::ControlLeft + Key::ShiftLeft + Key::Left);
//		bind("text.selection.expand_word_right", Key::ControlLeft + Key::ShiftLeft + Key::Right);
//
//		if (frame->isKeyPressed(libv::input::Key::Up))
//			camera->translateY(-keyboard_move_speed * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Key::Down))
//			camera->translateY(keyboard_move_speed * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Key::Right))
//			camera->translateX(keyboard_move_speed * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Key::Left))
//			camera->translateX(-keyboard_move_speed * elapsed_time);
//
//		if (frame->isKeyPressed(libv::input::Key::R))
//			camera->translateZ(keyboard_move_speed * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Key::F))
//			camera->translateZ(-keyboard_move_speed * elapsed_time);
//
//		if (frame->isKeyPressed(libv::input::Key::T))
//			camera->translateZoom(-keyboard_zoom_scale * elapsed_time);
//		if (frame->isKeyPressed(libv::input::Key::G))
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
		if (e.key == libv::input::Key::Escape)
			frame.closeDefault();
	});

//	frame.onKey.output([&frame](const libv::input::EventKey& e) {
//		if (e.action != libv::input::Action::release)
//			;
//	});

	frame.onKey.output([](const libv::input::EventKey& e) {
		const int scancode = e.key != libv::input::Key::Unknown ? glfwGetKeyScancode(libv::to_value(e.key)) : -1;
		const char* name = glfwGetKeyName(libv::to_value(e.key), libv::to_value(e.scancode));
		log_sandbox.trace("{}, rescan = {}, name = {}, rescan-match = {}",
				e.toPrettyString(), scancode, name ? name : "unkown", scancode == libv::to_value(e.scancode));
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

		log_sandbox.trace("{}, key: {} scan: {}, name: {}", i, libv::input::to_string(libv::input::Key{i}), scancode, name ? name : "unkown");
	}

	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");
	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");
	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");
	log_sandbox.trace("---------------- ---------------- ---------------- ----------------");

	for (int i = 0; i <= 65536; ++i) {
		const char* name = glfwGetKeyName(i, -1);

		if (name != nullptr)
			log_sandbox.trace("{}, key: {} name: {}", i, libv::input::to_string(libv::input::Key{i}), name);
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

*/

// =================================================================================================
// =================================================================================================
// === HELLO WORLD =================================================================================
// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------
// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

// libv
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
// std
#include <iostream>
// pro
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/ctrl/fwd.hpp>


using namespace std;
using namespace std::chrono;
using namespace libv;
using namespace libv::input;
using namespace libv::ctrl;


// --- Client side singleton header file - global_controls.hpp

#include <libv/ctrl/fwd.hpp>
#include <libv/ctrl/feature_register.hpp>

Controls& global_controls();
FeatureRegister global_feature_register();

using FeatureRegistration = AutomaticFeatureRegister<global_feature_register>;

// --- Client side singleton source file - global_controls.cpp

#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>


Controls& global_controls() {
	static Controls controls;
	return controls;
}

FeatureRegister global_feature_register() {
	return global_controls();
}

// --- Client business logic header file - camera.hpp

struct Camera {
	double x = 0, y = 0, z = 0;
};

// --- Client business logic source file - camera.cpp

//#include <camera.hpp>
//#include <global_controls.hpp>

static FeatureRegistration __camera_register([](FeatureRegister& registry) {
	// Instance and input independent feature registration

	registry.feature_action<Camera>("camera.reset", [](arg_action, Camera& camera) {
		camera.x = 0;
		camera.y = 0;
		camera.z = 0;
		cout << "Camera moved to origin" << endl;
	});

	registry.feature_analog<Camera>("camera.move_x", [](arg_analog arg, Camera& camera) {
		camera.x += arg.value;
		cout << "Camera moved to X: " << camera.x << endl;
	});

	registry.feature_analog<Camera>("camera.move_y", [](arg_analog arg, Camera& camera) {
		camera.y += arg.value;
		cout << "Camera moved to Y: " << camera.y << endl;
	});

	registry.feature_analog<Camera>("camera.move_z", [](arg_analog arg, Camera& camera) {
		camera.z += arg.value;
		cout << "Camera moved to Z: " << camera.z << endl;
	});
});

// --- Client business logic entry point - main.cpp

#include <libv/ctrl/feature.hpp>
#include <libv/ctrl/binding.hpp>
#include <libv/ctrl/parse.hpp>
//#include <camera.hpp>
//#include <global_controls.hpp>

int main() {
	Controls& controls = global_controls();

	// Setup / load bindings
	cout << "Feature list:\n";
	controls.foreach_features([](const Feature& feature) {
		cout << "    " << feature.name() << endl;
	});

	controls.bind("camera.reset", "A");
	controls.bind("camera.move_x", "Mouse X");
	controls.bind("camera.move_y", "Mouse Y");
	controls.bind("camera.move_z", "Ctrl + Mouse X");

	// Setup context
	Camera camera; // Camera context variable with unrelated lifetime to Controls
	controls.context_enter(&camera); // Enter camera context

	// Process events
	controls.input(EventMousePosition{120, 60});
	// Output: Camera moved to X: 0.2
	// Output: Camera moved to Y: 0.1

	controls.input(EventKey{Key::ControlLeft, Scancode{29}, Action::press, KeyModifier::none});
	controls.input(EventMousePosition{300, 60});
	// Output: Camera moved to Z: 0.3
	controls.input(EventKey{Key::ControlLeft, Scancode{29}, Action::release, KeyModifier::none});

	controls.input(EventKey{Key::A, Scancode{30}, Action::press, KeyModifier::none});
	// Output: Camera moved to origin
	controls.input(EventKey{Key::A, Scancode{30}, Action::release, KeyModifier::none});

	controls.context_leave<Camera>(); // Leave camera context

	return EXIT_SUCCESS;
}
