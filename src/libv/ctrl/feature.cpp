// Project: libv.ctrl, File: src/libv/ctrl/feature.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ctrl/feature.hpp>
// std
#include <cassert>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

Feature::Feature(std::type_index context, std::string&& name, ft_action function) :
	context(context),
	name_(std::move(name)),
	function_action(std::move(function)) { }

Feature::Feature(std::type_index context, std::string&& name, ft_analog function, scale_type scale_impulse, scale_type scale_time, scale_type scale_analog) :
	context(context),
	name_(std::move(name)),
	function_analog(std::move(function)),
	scale_impulse(scale_impulse),
	scale_time(scale_time),
	scale_analog(scale_analog) { }

Feature::Feature(std::type_index context, std::string&& name, ft_binary function) :
	context(context),
	name_(std::move(name)),
	function_binary(std::move(function)) { }

void Feature::fire_action(void* ctx_ptr) {
	if (!function_action)
		return;

	function_action(arg_action_t{}, ctx_ptr);
}

void Feature::fire_binary(void* ctx_ptr, OperationBinary operation, bool operand_) {
	if (!function_binary)
		return;

	binary_state = [&](bool operand) {
		switch (operation) {
		case OperationBinary::enable:
			return true;
		case OperationBinary::disable:
			return false;
		case OperationBinary::toggle:
			return !binary_state;
		case OperationBinary::state:
			return operand;
		case OperationBinary::inverted:
			return !operand;
		}

		assert(false && "Invalid Binary Operation");
		return false;
	}(operand_);

	function_binary(arg_binary_t{binary_state}, ctx_ptr);
}

void Feature::fire_analog(void* ctx_ptr, scale_type scale, Origin origin) {
	if (!function_analog)
		return;

	switch (origin) {
	case Origin::analog:
		return function_analog(arg_analog_t{scale * scale_analog}, ctx_ptr);
	case Origin::impulse:
		return function_analog(arg_analog_t{scale * scale_impulse}, ctx_ptr);
	case Origin::time:
		return function_analog(arg_analog_t{scale * scale_time}, ctx_ptr);
	}

	assert(false && "Invalid Analog Origin");
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv