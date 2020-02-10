// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

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

	controls.input(EventKey{Keycode::A, Scancode{30}, Action::press});
	// Output: "Action: Hello!"
	controls.input(EventKey{Keycode::A, Scancode{30}, Action::release});
	controls.input(EventKey{Keycode::S, Scancode{31}, Action::press});
	// Output: "Action: Bye!"
	controls.input(EventKey{Keycode::S, Scancode{31}, Action::release});

	return EXIT_SUCCESS;
}
