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

	controls.input(EventKey{Key::A, Scancode{30}, Action::press, KeyModifier::none});
	// Output: "Action: Hello!"
	controls.input(EventKey{Key::A, Scancode{30}, Action::release, KeyModifier::none});
	controls.input(EventKey{Key::S, Scancode{31}, Action::press, KeyModifier::none});
	// Output: "Action: Bye!"
	controls.input(EventKey{Key::S, Scancode{31}, Action::release, KeyModifier::none});

	return EXIT_SUCCESS;
}
