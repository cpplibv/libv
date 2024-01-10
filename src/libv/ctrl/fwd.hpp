// Project: libv.ctrl, File: src/libv/ctrl/fwd.hpp

#pragma once


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

class ImplControls;
void increase_ref_count(ImplControls* ptr);
void decrease_ref_count(ImplControls* ptr);

class Controls;
class BindingRegister;
class FeatureRegister;

struct Binding;
struct Combination;
struct Feature;
struct Input;
struct InputID;
struct Sequence;

struct arg_action_t;
struct arg_analog_t;
struct arg_binary_t;

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
