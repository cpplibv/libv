// Project: libv.ctrl, File: src/libv/ctrl/feature_register.cpp

// hpp
#include <libv/ctrl/feature_register.hpp>
// pro
#include <libv/ctrl/controls.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

void FeatureRegister::_feature_action(libv::type_uid context, std::string&& name, ft_action function) {
	target->_feature_action(context, std::move(name), std::move(function));
}

void FeatureRegister::_feature_binary(libv::type_uid context, std::string&& name, ft_binary function) {
	target->_feature_binary(context, std::move(name), std::move(function));
}

void FeatureRegister::_feature_analog(libv::type_uid context, std::string&& name, ft_analog function, scale_group multipliers) {
	target->_feature_analog(context, std::move(name), std::move(function), multipliers);
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
