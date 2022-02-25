// Project: libv.ctrl, File: src/libv/ctrl/controls.hpp

// hpp
#include <libv/ctrl/binding_register.hpp>
// pro
#include <libv/ctrl/combination.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/input.hpp>
#include <libv/ctrl/sequence.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

binding_outcome BindingRegister::bind(std::string feature_name, Sequence seq, binding_level level) {
	return target->bind(std::move(feature_name), std::move(seq), level);
}
binding_outcome BindingRegister::bind(std::string feature_name, Sequence seq, scale_type scale, binding_level level) {
	return target->bind(std::move(feature_name), std::move(seq), scale, level);
}
binding_outcome BindingRegister::bind(std::string feature_name, Combination comb, binding_level level) {
	return target->bind(std::move(feature_name), std::move(comb), level);
}
binding_outcome BindingRegister::bind(std::string feature_name, Combination comb, scale_type scale, binding_level level) {
	return target->bind(std::move(feature_name), std::move(comb), scale, level);
}
binding_outcome BindingRegister::bind(std::string feature_name, Input input, binding_level level) {
	return target->bind(std::move(feature_name), std::move(input), level);
}
binding_outcome BindingRegister::bind(std::string feature_name, Input input, scale_type scale, binding_level level) {
	return target->bind(std::move(feature_name), std::move(input), scale, level);
}
binding_outcome BindingRegister::bind(std::string feature_name, std::string_view seq_str, binding_level level) {
	return target->bind(std::move(feature_name), seq_str, level);
}
binding_outcome BindingRegister::bind(std::string feature_name, std::string_view seq_str, scale_type scale, binding_level level) {
	return target->bind(std::move(feature_name), seq_str, scale, level);
}

unbinding_outcome BindingRegister::unbind(std::string_view feature_name, Sequence seq) {
	return target->unbind(feature_name, std::move(seq));
}
unbinding_outcome BindingRegister::unbind(std::string_view feature_name, std::string_view seq_str) {
	return target->unbind(feature_name, seq_str);
}
unbinding_outcome BindingRegister::unbind_all(std::string_view feature_name) {
	return target->unbind_all(feature_name);
}

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
