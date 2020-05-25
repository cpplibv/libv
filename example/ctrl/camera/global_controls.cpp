// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]
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
