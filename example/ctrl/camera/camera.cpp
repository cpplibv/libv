// Project: libv.ctrl, File: example/ctrl/camera/camera.cpp
// --- Client business logic source file - camera.cpp

#include "camera.hpp"
#include <iostream>
#include "global_controls.hpp"

using namespace std;
using namespace libv::ctrl;

static FeatureRegistration __camera_register([](FeatureRegister& registry) {
	// Instance and input independent feature registration, with minimal exposure to the header of control

	registry.feature_action<Camera>("camera.reset", [](arg_action, Camera& camera) {
		camera.x = 0;
		camera.y = 0;
		camera.z = 0;
		cout << "Camera moved to origin" << endl;
	});

	registry.feature_action<Camera>("camera.drift", [](arg_action, Camera& camera) {
		camera.x *= 2;
		camera.y *= 2;
		camera.z *= 2;
		cout << "Camera drifted to: " << camera.x << ", " << camera.y << ", "  << camera.z << endl;
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
