// Project: libv.ctrl, File: src/libv/ctrl/feature_register.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ctrl/feature_register.hpp>
// pro
#include <libv/ctrl/controls.hpp>


namespace libv {
namespace ctrl {

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

} // namespace ctrl
} // namespace libv
