// Project: libv.ctrl, File: example/ctrl/hello/main.cpp

#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
#include <iostream>
#include <libv/ctrl/controls.hpp>


using namespace std;
using namespace libv;
using namespace libv::input;
using namespace libv::ctrl;


int main() {
	Controls controls;

	controls.feature_action<void>("sandbox.hello", [](arg_action) {
		cout << "Action: Hello!" << endl;
	});

	controls.feature_action<void>("sandbox.bye", [](arg_action) {
		cout << "Action: Bye!" << endl;
	});

	controls.bind("sandbox.hello", "A");
	controls.bind("sandbox.bye", "S");

	controls.event(EventKey{Keycode::A, Scancode{30}, Action::press});
	// Output: "Action: Hello!"
	controls.event(EventKey{Keycode::A, Scancode{30}, Action::release});
	controls.event(EventKey{Keycode::S, Scancode{31}, Action::press});
	// Output: "Action: Bye!"
	controls.event(EventKey{Keycode::S, Scancode{31}, Action::release});

	return EXIT_SUCCESS;
}
