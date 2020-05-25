// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]
// --- Client side singleton header file - global_controls.hpp

#pragma once

#include <libv/ctrl/fwd.hpp>
#include <libv/ctrl/feature_register.hpp>

using namespace libv::ctrl;

Controls& global_controls();
FeatureRegister global_feature_register();

using FeatureRegistration = AutomaticFeatureRegister<global_feature_register>;
