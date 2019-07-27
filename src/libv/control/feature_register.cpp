// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

// hpp
#include <libv/control/feature_register.hpp>
// pro
#include <libv/control/controls.hpp>


namespace libv {
namespace control {

// -------------------------------------------------------------------------------------------------

void FeatureRegister::_feature_action(std::type_index context, std::string&& name, ft_action function) {
	target->_feature_action(context, std::move(name), std::move(function));
}

void FeatureRegister::_feature_binary(std::type_index context, std::string&& name, ft_binary function) {
	target->_feature_binary(context, std::move(name), std::move(function));
}

void FeatureRegister::_feature_analog(std::type_index context, std::string&& name, ft_analog function, scale_type scale_impulse, scale_type scale_time, scale_type scale_analog) {
	target->_feature_analog(context, std::move(name), std::move(function), scale_impulse, scale_time, scale_analog);
}

// -------------------------------------------------------------------------------------------------

} // namespace control
} // namespace libv
