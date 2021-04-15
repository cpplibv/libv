// Project: libv.ctrl, File: example/ctrl/camera/main.cpp, Author: Császár Mátyás [Vader]
// --- Client business logic entry point - main.cpp

#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature.hpp>
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
#include <iostream>
#include "camera.hpp"
#include "global_controls.hpp"


using namespace std;
using namespace libv;
using namespace libv::input;
using namespace libv::ctrl;


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
	controls.event(EventMousePosition{120, 60});
	// Output: Camera moved to X: 0.2
	// Output: Camera moved to Y: 0.1

	controls.event(EventKey{Keycode::ControlLeft, Scancode{29}, Action::press});
	controls.event(EventMousePosition{300, 60});
	// Output: Camera moved to Z: 0.3
	controls.event(EventKey{Keycode::ControlLeft, Scancode{29}, Action::release});

	controls.event(EventKey{Keycode::A, Scancode{30}, Action::press});
	// Output: Camera moved to origin
	controls.event(EventKey{Keycode::A, Scancode{30}, Action::release});

	controls.context_leave<Camera>(); // Leave camera context

	return EXIT_SUCCESS;
}
