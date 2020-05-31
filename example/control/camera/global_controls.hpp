// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]
// --- Client side singleton header file - global_controls.hpp

#pragma once

#include <libv/control/fwd.hpp>
#include <libv/control/feature_register.hpp>

using namespace libv::control;

Controls& global_controls();
FeatureRegister global_feature_register();

using FeatureRegistration = AutomaticFeatureRegister<global_feature_register>;
