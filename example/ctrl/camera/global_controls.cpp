// Project: libv.ctrl, File: example/ctrl/camera/global_controls.cpp
// --- Client side singleton source file - global_controls.cpp

#include "global_controls.hpp"
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>

using namespace libv::ctrl;

Controls& global_controls() {
	static Controls controls;
	return controls;
}

FeatureRegister global_feature_register() {
	return global_controls();
}
