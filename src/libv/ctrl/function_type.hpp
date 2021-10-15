// Project: libv.ctrl, File: src/libv/ctrl/function_type.hpp

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// std
#include <functional>
// pro
#include <libv/ctrl/arg.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

using arg_action = const arg_action_t&;
using arg_analog = const arg_analog_t&;
using arg_binary = const arg_binary_t&;

// TODO P5: Consider using unique function
using ft_action = std::function<void(arg_action, void*)>;
using ft_analog = std::function<void(arg_analog, void*)>;
using ft_binary = std::function<void(arg_binary, void*)>;

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
