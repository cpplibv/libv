// Project: libv.ctrl, File: src/libv/ctrl/controls.cpp

// hpp
#include <libv/ctrl/controls.hpp>
// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/algo/erase_if_unstable.hpp>
#include <libv/input/event.hpp>
#include <libv/utility/float_equal.hpp>
// pro
#include <libv/ctrl/controls_impl.hxx>
#include <libv/ctrl/parse.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

namespace {

struct Operation {
	bool enable_action = false; /// Enable action feature executions
	bool enable_binary = false; /// Enable binary feature executions
	bool enable_analog = false; /// Enable analog feature executions

	bool binary_operand = false;
};

struct MatchState {
	bool keep_alive = false; // Keep the state alive, as it is a stateful match
	bool state = false;
	bool terminate_sequence = false;
};

inline void calculate_operation(Operation& op, MatchState& st, AnalogDimension aia, AnalogDimension action_x_, AnalogDimension action_y_) noexcept {
	const auto action_x = [&](auto... args) {
		return ((action_x_ == args) || ...);
	};
	const auto action_y = [&](auto... args) {
		return ((action_y_ == args) || ...);
	};

	switch (aia) {
	case AnalogDimension::x:
		op.enable_action = action_x(AnalogDimension::x_plus, AnalogDimension::x_minus);
		op.enable_binary = action_x(AnalogDimension::x_plus, AnalogDimension::x_minus);
		op.enable_analog = action_x(AnalogDimension::x_plus, AnalogDimension::x_minus);
		op.binary_operand = action_x(AnalogDimension::x_plus);
		st.state = action_x(AnalogDimension::x_plus, AnalogDimension::x_minus);
	break;
	case AnalogDimension::x_plus:
		op.enable_action = action_x(AnalogDimension::x_plus);
		op.enable_binary = action_x(AnalogDimension::x_plus, AnalogDimension::x_minus);
		op.enable_analog = action_x(AnalogDimension::x_plus);
		op.binary_operand = action_x(AnalogDimension::x_plus);
		st.state = action_x(AnalogDimension::x_plus, AnalogDimension::x_minus);
	break;
	case AnalogDimension::x_minus:
		op.enable_action = action_x(AnalogDimension::x_minus);
		op.enable_binary = action_x(AnalogDimension::x_plus, AnalogDimension::x_minus);
		op.enable_analog = action_x(AnalogDimension::x_minus);
		op.binary_operand = action_x(AnalogDimension::x_minus);
		st.state = action_x(AnalogDimension::x_plus, AnalogDimension::x_minus);
	break;
	case AnalogDimension::y:
		op.enable_action = action_y(AnalogDimension::y_plus, AnalogDimension::y_minus);
		op.enable_binary = action_y(AnalogDimension::y_plus, AnalogDimension::y_minus);
		op.enable_analog = action_y(AnalogDimension::y_plus, AnalogDimension::y_minus);
		op.binary_operand = action_y(AnalogDimension::y_plus);
		st.state = action_y(AnalogDimension::y_plus, AnalogDimension::y_minus);
	break;
	case AnalogDimension::y_plus:
		op.enable_action = action_y(AnalogDimension::y_plus);
		op.enable_binary = action_y(AnalogDimension::y_plus, AnalogDimension::y_minus);
		op.enable_analog = action_y(AnalogDimension::y_plus);
		op.binary_operand = action_y(AnalogDimension::y_plus);
		st.state = action_y(AnalogDimension::y_plus, AnalogDimension::y_minus);
	break;
	case AnalogDimension::y_minus:
		op.enable_action = action_y(AnalogDimension::y_minus);
		op.enable_binary = action_y(AnalogDimension::y_plus, AnalogDimension::y_minus);
		op.enable_analog = action_y(AnalogDimension::y_minus);
		op.binary_operand = action_y(AnalogDimension::y_minus);
		st.state = action_y(AnalogDimension::y_plus, AnalogDimension::y_minus);
	break;
	}

	st.keep_alive = false;
	st.terminate_sequence = false;
}

inline void calculate_operation(Operation& op, MatchState& st, DigitalInputAction dia, ButtonAction action_) noexcept {
	const auto action = [&](auto... args) {
		return ((action_ == args) || ...);
	};

	switch (dia) {
	case DigitalInputAction::auto_:
		op.enable_action = action(ButtonAction::press, ButtonAction::repeat);
		op.enable_binary = action(ButtonAction::press, ButtonAction::release);
		op.enable_analog = false;
		op.binary_operand = action(ButtonAction::press);
		st.keep_alive = true;
		st.state = action(ButtonAction::press, ButtonAction::repeat);
		st.terminate_sequence = false;
	break;
	case DigitalInputAction::press:
		op.enable_action = action(ButtonAction::press);
		op.enable_binary = action(ButtonAction::press);
		op.enable_analog = action(ButtonAction::press);
		op.binary_operand = true;
		st.keep_alive = false;
		st.state = action(ButtonAction::press);
		st.terminate_sequence = true;
	break;
	case DigitalInputAction::repeat:
		op.enable_action = action(ButtonAction::press, ButtonAction::repeat);
		op.enable_binary = action(ButtonAction::press, ButtonAction::repeat);
		op.enable_analog = action(ButtonAction::press, ButtonAction::repeat);
		op.binary_operand = true;
		st.keep_alive = false;
		st.state = action(ButtonAction::press, ButtonAction::repeat);
		st.terminate_sequence = false;
	break;
	case DigitalInputAction::release:
		op.enable_action = action(ButtonAction::release);
		op.enable_binary = action(ButtonAction::release);
		op.enable_analog = action(ButtonAction::release);
		op.binary_operand = false;
		st.keep_alive = false;
		st.state = action(ButtonAction::release);
		st.terminate_sequence = true;
	break;
	case DigitalInputAction::any:
		op.enable_action = action(ButtonAction::press, ButtonAction::release);
		op.enable_binary = action(ButtonAction::press, ButtonAction::release);
		op.enable_analog = action(ButtonAction::press, ButtonAction::release);
		op.binary_operand = action(ButtonAction::press);
		st.keep_alive = false;
		st.state = action(ButtonAction::press, ButtonAction::release);
		st.terminate_sequence = false;
	break;
	case DigitalInputAction::hold:
		op.enable_action = action(ButtonAction::press, ButtonAction::repeat);
		op.enable_binary = action(ButtonAction::press, ButtonAction::release);
		op.enable_analog = false;
		op.binary_operand = action(ButtonAction::press);
		st.keep_alive = true;
		st.state = action(ButtonAction::press, ButtonAction::repeat);
		st.terminate_sequence = false;
	break;
	case DigitalInputAction::free:
		op.enable_action = action(ButtonAction::release);
		op.enable_binary = action(ButtonAction::press, ButtonAction::release);
		op.enable_analog = false;
		op.binary_operand = action(ButtonAction::release);
		st.keep_alive = true;
		st.state = action(ButtonAction::release);
		st.terminate_sequence = false;
	break;
	}
}

// -------------------------------------------------------------------------------------------------

struct TargetFeature {
	Feature* feature;
	void* context;

	bool call_action = false;

	bool call_binary = false;
	OperationBinary operation_binary;
	bool binary_operand;

	bool call_analog = false;
	scale_type scale_analog;
	Origin analog_origin;

public:
	TargetFeature(Feature* feature, void* context) :
		feature(feature), context(context), call_action(true) { }

	TargetFeature(Feature* feature, void* context, OperationBinary operation_binary, bool binary_operand) :
		feature(feature), context(context), call_binary(true), operation_binary(operation_binary), binary_operand(binary_operand) { }

	TargetFeature(Feature* feature, void* context, scale_type scale_analog, Origin origin) :
		feature(feature), context(context), call_analog(true), scale_analog(scale_analog), analog_origin(origin) { }

public:
	void operator()() const {
		if (call_action)
			feature->fire_action(context);

		if (call_binary)
			feature->fire_binary(context, operation_binary, binary_operand);

		if (call_analog)
			feature->fire_analog(context, scale_analog, analog_origin);
	}
};

struct CandidateBinding {
	Binding* binding;
	Operation operation;
	scale_type value;
};

using TargetFeatures = boost::container::small_vector<TargetFeature, 32>;
using CandidateBindings = boost::container::small_vector<CandidateBinding, 32>;

// -------------------------------------------------------------------------------------------------

inline auto reset_from_sow(const ImplControls& sow, StatedCombination& combination) {
	combination.active = true;

	for (StatedInput& input : combination.inputs) {
		bool sow_pressed = [&sow](const InputID inputID) {
			switch (inputID.type()) {
				case SourceType::invalid:
					assert(false && "Invalid input");
					return false;

				case SourceType::codepoint:
					return sow.pressed_codepoints.contains(inputID.codepoint());
				case SourceType::keycode:
					return sow.pressed_keycodes.contains(inputID.keycode());
				case SourceType::scancode:
					return sow.pressed_scancodes.contains(inputID.scancode());
				case SourceType::modifier: {
					switch (inputID.modifier()) {
					case Modifier::alt:
						return sow.pressed_keycodes.contains(Keycode::AltLeft) ||
								sow.pressed_keycodes.contains(Keycode::AltRight);
					case Modifier::control:
						return sow.pressed_keycodes.contains(Keycode::ControlLeft) ||
								sow.pressed_keycodes.contains(Keycode::ControlRight);
					case Modifier::shift:
						return sow.pressed_keycodes.contains(Keycode::ShiftLeft) ||
								sow.pressed_keycodes.contains(Keycode::ShiftRight);
					case Modifier::super:
						return sow.pressed_keycodes.contains(Keycode::SuperLeft) ||
								sow.pressed_keycodes.contains(Keycode::SuperRight);
					}

					assert(false && "Invalid modifier");
					return false;
				}
				case SourceType::mouseButton:
					return sow.pressed_mouses.contains(inputID.mouseButton());
				case SourceType::mouseMovement:
					return false;
				case SourceType::mouseScroll:
					return false;

				case SourceType::gamepadButton: {
					if (inputID.gamepadID() != GamepadID::any) {
						const auto it = sow.gamepads.find(inputID.gamepadID());
						if (it == sow.gamepads.end())
							return false;
						return it->second.pressed_buttons.contains(inputID.gamepadButton());
					} else {
						for (const auto& sow_gamepad : sow.gamepads)
							if (sow_gamepad.second.pressed_buttons.contains(inputID.gamepadButton()))
								return true;
						return false;
					}
				}
				case SourceType::gamepadAnalog:
					return false;

				case SourceType::joystickButton: {
					if (inputID.joystickID() != JoystickID::any) {
						const auto it = sow.joysticks.find(inputID.joystickID());
						if (it == sow.joysticks.end())
							return false;
						return it->second.pressed_buttons.contains(inputID.joystickButton());
					} else {
						for (const auto& sow_joystick : sow.joysticks)
							if (sow_joystick.second.pressed_buttons.contains(inputID.joystickButton()))
								return true;
						return false;
					}
				}
				case SourceType::joystickAnalog:
					return false;
			}

			assert(false && "Invalid source type");
			return false;
		}(input.id);

		if (input.id.is_button()) {
			input.active = [&sow_pressed](const DigitalInputAction dia) {
				switch (dia) {
				case DigitalInputAction::auto_: return sow_pressed;
				case DigitalInputAction::press: return false;
				case DigitalInputAction::repeat: return false;
				case DigitalInputAction::release: return false;
				case DigitalInputAction::any: return false;
				case DigitalInputAction::hold: return sow_pressed;
				case DigitalInputAction::free: return !sow_pressed;
				}
				assert(false && "Invalid digital input action");
				return false;
			}(input.dia);

		} else if (input.id.is_analog()) {
			input.active = false;

		} else {
			assert(false && "Invalid input");
			input.active = false;
		}

		combination.active &= input.active;
	}
}

// -------------------------------------------------------------------------------------------------

inline auto state_enage(ImplControls& sow, StatedSequence& sequence, StatedCombination& combination, StatedInput& input, bool keep_alive, bool state, bool terminate_sequence) {
	struct Result {
		bool sequence_active = false;
		bool sequence_activated = false;
		bool sequence_deactivated = false;
	} result;

//	NOTE: No-op micro optimization shortcut, worth to check on its firing frequency in real use cases
//	if (input.satisfied == state) {
//		result.sequence_active = sequence.is_satisfied();
//		return result;
//	}

	const auto was_active = sequence.is_active();
	const auto was_last_combination = sequence.is_last_combination();
	bool sequence_considered_active = false;

	if (keep_alive)
		input.active = state;

	if (!state)
		sequence.active = false;

	combination.active = state;
	for (const StatedInput& cinput : combination.inputs)
		if (&cinput != &input)
			combination.active &= cinput.active;

	if (combination.active) {
		if (was_last_combination) {
			sequence_considered_active = true;
			sequence.active = keep_alive;

			if (terminate_sequence) {
				sequence.checkpoint = 0;
				reset_from_sow(sow, sequence.current_combination());
			}

		} else {
			sequence.checkpoint++;
			reset_from_sow(sow, sequence.current_combination());
		}
	}

	const auto now_active = sequence.is_active();

	result.sequence_activated = !was_active && now_active;
	result.sequence_deactivated = was_active && !now_active;
	result.sequence_active = sequence_considered_active;

	return result;
}

inline auto state_disengage(StatedInput& input) {
	const auto was_active = input.active;
	input.active = false;
	return was_active;
}

template <typename Matcher>
inline StatedInput* find_relevant_input(const Matcher& matcher, StatedCombination& combination) {
	for (StatedInput& input : combination.inputs)
		if (matcher(input))
			return &input;

	return nullptr;
}

template <typename Matcher>
inline StatedInput* find_relevant_input(const Matcher& matcher, StatedCombination& combination, Operation& operation, MatchState& state, scale_type& value) {
	for (StatedInput& input : combination.inputs) {
		const auto match_result = matcher(input);
		if (match_result.match) {
			operation = match_result.operation;
			state = match_result.state;
			value = match_result.value;
			return &input;
		}
	}

	return nullptr;
}

} // namespace =====================================================================================

template <typename Matcher, typename OffMatcher, typename Op>
inline auto ImplControls::update_binding(const Matcher& matcher, const OffMatcher& off_matcher, Binding& binding, Op& operation) {
	struct Result {
		bool statisfied = false;
		bool forward_progress = false;
		bool disengage_progress = false;
		bool is_sequence = false;
		bool in_progress = false;
		scale_type value;
	} result;

	StatedSequence& sequence = binding.sequence_;
	StatedCombination& current_combination = sequence.current_combination();
	result.is_sequence = sequence.is_sequence();

	MatchState state;
	StatedInput* input = find_relevant_input(matcher, current_combination, operation, state, result.value);

	// Previous step disengage case
	// NOTE: Optimization opportunity: Only observe if there is no forward progress
	if (sequence.in_progress()) {
		StatedCombination& previous_combination = sequence.previous_combination();
		StatedInput* previous_input = find_relevant_input(off_matcher, previous_combination);
		if (previous_input != nullptr)
			result.disengage_progress = state_disengage(*previous_input);
	}

	// ---

	if (input == nullptr) { // No input matched in the current combination
		// Unrelated event case
		if (sequence.is_sequence() && !result.disengage_progress) {
			sequence.checkpoint = 0;
			reset_from_sow(*this, sequence.current_combination());
		}

		result.in_progress = sequence.in_progress();
		return result;
	}

	result.forward_progress = state.state;
	const auto engage_result = state_enage(*this, sequence, current_combination, *input, state.keep_alive, state.state, state.terminate_sequence);

	if (engage_result.sequence_activated)
		active_bindings.emplace(&binding);

	if (engage_result.sequence_deactivated)
		active_bindings.erase(&binding);

	// NOTE: Deactivated is needed for binary feature's down edge
	result.statisfied = engage_result.sequence_active || engage_result.sequence_deactivated;
	result.in_progress = sequence.in_progress();
	return result;
}

template <typename Matcher, typename OffMatcher>
inline auto ImplControls::update_and_gather_bindings(const Matcher& matcher, const OffMatcher& off_matcher) {
	CandidateBindings candidate_bindings;
	Operation operation;

	const auto visit = [&](Binding& binding) {
		const auto update_result = update_binding(matcher, off_matcher, binding, operation);

		if (update_result.is_sequence && update_result.in_progress && (update_result.disengage_progress || update_result.forward_progress)) {
			deadkey_set.add_to_background(&binding);
		}

		if (update_result.statisfied)
			candidate_bindings.emplace_back(CandidateBinding{&binding, operation, update_result.value});
	};

	const bool in_deadkey_mode = !deadkey_set.foreground_empty();
	if (in_deadkey_mode)
		for (Binding* binding : deadkey_set.foreground())
			visit(*binding);
	else
		// NOTE: Optimization opportunity: Instead of iterating every binding, create an index with normalized InputIDs to Binding that contains them
		for (auto& binding : bindings)
			visit(*binding);

	deadkey_set.rotate();
	sequence_timeout_timer = timeout_sequence;
	return candidate_bindings;
}

template <typename T>
inline auto ImplControls::select_bindings(T& candidate_bindings) {
	int max_complexity = 0;

	for (const auto& candidate : candidate_bindings)
		max_complexity = std::max(max_complexity, candidate.binding->complexity());

	libv::erase_if_unstable(candidate_bindings, [&](const auto& candidate) {
		return candidate.binding->complexity() < max_complexity;
	});
}

template <typename C, typename O, typename T>
inline auto ImplControls::gather_feature(const C& target_binding, const O& operation, T& target_features, Origin origin, scale_type value_) {
	const auto& feature_name = target_binding.feature_name();
	const auto& binary_operation = target_binding.operation_binary;
	const auto value = target_binding.scale() * value_;

	const auto range = features.equal_range(feature_name);
	if (range.first == range.second)
		return;

	for (auto fit = range.first; fit != range.second; ++fit) {
		auto& feature = fit->second;

		const auto it = active_contexts.find(feature.context);
		if (it == active_contexts.end())
			continue;

		if (operation.enable_action && feature.is_action())
			target_features.emplace_back(&feature, it->second);

		if (operation.enable_binary && feature.is_binary())
			target_features.emplace_back(&feature, it->second, binary_operation, operation.binary_operand);

		if (operation.enable_analog && feature.is_analog())
			target_features.emplace_back(&feature, it->second, value, origin);
	}
}

template <typename C, typename T>
inline auto ImplControls::gather_features(const C& target_bindings, T& target_features, Origin origin) {
	for (const auto& target_binding : target_bindings)
		gather_feature(*target_binding.binding, target_binding.operation, target_features, origin, target_binding.value);
}

template <typename T>
inline auto ImplControls::notify_features(const T& target_features) {
	for (const TargetFeature& target_feature : target_features)
		target_feature();
}

// =================================================================================================

void ImplControls::process_analog(const InputID event_X, const InputID event_Y, analog_type_2D value, scale_type_2D scale, Origin origin) {
	assert(event_X.is_analog());
	assert(event_Y.is_analog());

	const bool noop_x = libv::float_equal(value.x, 0.0);
	const bool noop_y = libv::float_equal(value.y, 0.0);

	// Fake inputIDs are used to select the whole dimension with alias matching (opposites of the dimension might be used by binary features)
	auto fake_x = event_X;
	fake_x.analogDimension(AnalogDimension::x);
	auto fake_y = event_Y;
	fake_y.analogDimension(AnalogDimension::y);

	const auto off_matcher = [&](const auto& input) {
		const bool match_x = !noop_x && alias_match(fake_x, input.id);
		const bool match_y = !noop_y && alias_match(fake_y, input.id);
		return match_x || match_y;
	};

	const auto matcher = [&](const auto& input) {
		struct Result {
			bool match = false;
			scale_type value = 1;
			Operation operation;
			MatchState state;
		} result;

		const bool match_x = !noop_x && alias_match(fake_x, input.id);
		const bool match_y = !noop_y && alias_match(fake_y, input.id);
		result.match = match_x || match_y;

		if (!result.match)
			return result;

		if (match_x)
			result.value = value.x * scale.x;
		else
			result.value = value.y * scale.y;

		calculate_operation(result.operation, result.state, input.id.analogDimension(), event_X.analogDimension(), event_Y.analogDimension());

		return result;
	};

	auto candidate_bindings = update_and_gather_bindings(matcher, off_matcher);
	select_bindings(candidate_bindings);
	TargetFeatures target_features;
	gather_features(candidate_bindings, target_features, origin);
	notify_features(target_features);
}

template <typename EventMatcher>
void ImplControls::process_button(const EventMatcher& event_matcher, ButtonAction action, scale_type impulse) {
	const auto off_matcher = [&](const auto& input) {
		return event_matcher(input.id);
	};

	const auto matcher = [&](const auto& input) {
		struct Result {
			bool match = false;
			scale_type value = 1;
			Operation operation;
			MatchState state;
		} result;

		result.match = event_matcher(input.id);
		if (!result.match)
			return result;

		result.value = impulse;
		calculate_operation(result.operation, result.state, input.dia, action);

		return result;
	};

	auto candidate_bindings = update_and_gather_bindings(matcher, off_matcher);
	select_bindings(candidate_bindings);
	TargetFeatures target_features;
	gather_features(candidate_bindings, target_features, Origin::impulse);
	notify_features(target_features);
}

void ImplControls::process_time(const duration delta_time, scale_type scale) {
	Operation operation;
	operation.enable_analog = true;

	if (!deadkey_set.foreground_empty()) {
		sequence_timeout_timer -= delta_time;
		if (sequence_timeout_timer <= duration::zero()) {
			sequence_timeout_timer = duration::zero();
			for (Binding* binding : deadkey_set.foreground()) {
				binding->sequence_.checkpoint = 0;
				reset_from_sow(*this, binding->sequence_.current_combination());
			}
			deadkey_set.rotate();
		}
	}

	const auto value = scale * delta_time.count();

	TargetFeatures target_features;
	for (const Binding* binding : active_bindings)
		gather_feature(*binding, operation, target_features, Origin::time, value);

	notify_features(target_features);
}

// =================================================================================================

Controls::Controls() : self(std::make_unique<ImplControls>()) {
	// NOTE: void context is always present and it is never passed to any feature callback
	_context_enter(libv::type_key<void>(), nullptr);
}

Controls::~Controls() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

void Controls::_context_enter(libv::type_uid type, void* ctx) {
	self->active_contexts.insert_or_assign(type, ctx);
}

void Controls::_context_leave(libv::type_uid type) {
	self->active_contexts.erase(type);
}

void Controls::_context_leave_if_matches(libv::type_uid type, void* ctx) {
	auto it = self->active_contexts.find(type);
	if (it == self->active_contexts.end())
		return;

	if (it->second != ctx)
		return;

	self->active_contexts.erase(it);
}
// -------------------------------------------------------------------------------------------------

void Controls::_feature_action(libv::type_uid context, std::string&& name, ft_action function) {
	// TODO P4: Better container would remove key copy
	auto key = name;
	self->features.emplace(std::move(key), Feature(context, std::move(name), std::move(function)));
}

void Controls::_feature_analog(libv::type_uid context, std::string&& name, ft_analog function, scale_group multipliers) {
	// TODO P4: Better container would remove key copy
	auto key = name;
	self->features.emplace(std::move(key), Feature(context, std::move(name), std::move(function), multipliers));
}

void Controls::_feature_binary(libv::type_uid context, std::string&& name, ft_binary function) {
	// TODO P4: Better container would remove key copy
	auto key = name;
	self->features.emplace(std::move(key), Feature(context, std::move(name), std::move(function)));
}

void Controls::_remove_feature(libv::type_uid context, std::string_view name) {
	// TODO P5: Unordered multi map and equal range based erase algorithm
	const auto range = self->features.equal_range(name);
	auto i = range.first;
	const auto last = range.second;

	while (i != last) {
		if (i->second.name() == name && i->second.context == context)
			i = self->features.erase(i);
		else
			++i;
	}
}

void Controls::aux_update(duration delta_time) {
	// Update SOW
	self->input_time += delta_time;

	// Process event
	const auto scale = self->scale_time;
	self->process_time(delta_time, scale);
}

// -------------------------------------------------------------------------------------------------

binding_outcome Controls::bind(std::string feature_name, Sequence seq, binding_level level) {
	return bind(std::move(feature_name), std::move(seq), scale_type{1}, level);
}

binding_outcome Controls::bind(std::string feature_name, Sequence seq, scale_type scale, binding_level level) {
	if (level == auto_binding_level) {
		level = binding_level::primary;

		for (const auto& binding : self->bindings) {
			if (binding->feature_name() == feature_name && binding->level == binding_level::primary) {
				level = binding_level::secondary;
				break;
			}
		}
	}

	seq.normalize();

	if (seq.validate() != validation_result::success)
		return binding_outcome::failure_invalid_sequence;

	self->bindings.emplace_back(std::make_unique<Binding>(std::move(feature_name), std::move(seq), scale, level));

	return binding_outcome::success;
}

binding_outcome Controls::bind(std::string feature_name, Combination comb, binding_level level) {
	return bind(std::move(feature_name), std::move(comb), scale_type{1}, level);
}

binding_outcome Controls::bind(std::string feature_name, Combination comb, scale_type scale, binding_level level) {
	return bind(std::move(feature_name), Sequence{std::move(comb)}, scale, level);
}

binding_outcome Controls::bind(std::string feature_name, Input input, binding_level level) {
	return bind(std::move(feature_name), std::move(input), scale_type{1}, level);
}

binding_outcome Controls::bind(std::string feature_name, Input input, scale_type scale, binding_level level) {
	return bind(std::move(feature_name), Sequence{std::move(input)}, scale, level);
}

binding_outcome Controls::bind(std::string feature_name, std::string_view seq_str, binding_level level) {
	return bind(std::move(feature_name), seq_str, scale_type{1}, level);
}

binding_outcome Controls::bind(std::string feature_name, std::string_view seq_str, scale_type scale, binding_level level) {
	auto seq = parse_sequence_optional(seq_str);

	if (!seq)
		return binding_outcome::failure_invalid_sequence_str;

	return bind(std::move(feature_name), std::move(*seq), scale, level);
}

unbinding_outcome Controls::unbind(std::string_view feature_name, Sequence seq) {
	seq.normalize();

	libv::erase_if_unstable(self->bindings, [&](const auto& binding) {
		const auto match = feature_name == binding->feature_name() && seq == binding->sequence();
		if (match) {
			self->active_bindings.erase(binding.get());
			self->deadkey_set.foreground().erase(binding.get());
		}
		return match;
	});

	return unbinding_outcome::success;
}

unbinding_outcome Controls::unbind(std::string_view feature_name, std::string_view seq_str) {
	auto seq = parse_sequence_optional(seq_str);

	if (!seq)
		return unbinding_outcome::failure_invalid_sequence_str;

	return unbind(feature_name, std::move(*seq));
}

unbinding_outcome Controls::unbind_all(std::string_view feature_name) {
	libv::erase_if_unstable(self->bindings, [&](const auto& binding) {
		const auto match = feature_name == binding->feature_name();
		if (match) {
			self->active_bindings.erase(binding.get());
			self->deadkey_set.foreground().erase(binding.get());
		}
		return match;
	});

	return unbinding_outcome::success;
}

// -------------------------------------------------------------------------------------------------

void Controls::ignore_events(bool value) {
	self->ignore_events = value;
}

// -------------------------------------------------------------------------------------------------

void Controls::event(const libv::input::EventKey& event) {
	if (self->ignore_events)
		return;

	// Update SOW
	if (event.action == ButtonAction::release) {
		self->pressed_keycodes.erase(event.keycode);
		self->pressed_scancodes.erase(event.scancode);
	} else {
		self->pressed_keycodes.emplace(event.keycode);
		self->pressed_scancodes.emplace(event.scancode);
	}

	const auto it = self->codepoint_mappings.find(Keycode{event.keycode});
	const auto impulse = self->impulse_button * self->impulse_keyboard;

	if (it == self->codepoint_mappings.end()) {
		// Process event
		self->process_button([&](const auto inputID) {
			return
					alias_match(InputID{Keycode{event.keycode}}, inputID) ||
					alias_match(InputID{Scancode{event.scancode}}, inputID);
		}, event.action, impulse);

	} else {
		// Update SOW
		if (event.action == ButtonAction::release)
			self->pressed_codepoints.erase(it->second);
		else
			self->pressed_codepoints.emplace(it->second);

		// Process event
		self->process_button([&](const auto inputID) {
			return
					alias_match(InputID{Keycode{event.keycode}}, inputID) ||
					alias_match(InputID{Scancode{event.scancode}}, inputID) ||
					alias_match(InputID{Codepoint{it->second}}, inputID);
		}, event.action, impulse);
	}
}

void Controls::event(const libv::input::EventMouseButton& event) {
	if (self->ignore_events)
		return;

	// Update SOW
	if (event.action == ButtonAction::release) {
		self->pressed_mouses.erase(event.button);
	} else {
		self->pressed_mouses.emplace(event.button);
	}

	// Process event
	const auto impulse = self->impulse_button * self->impulse_mouse_button;

	const auto event_matcher = [&](const auto inputID) { return alias_match(InputID{MouseButton{event.button}}, inputID); };
	self->process_button(event_matcher, event.action, impulse);
}

void Controls::event(const libv::input::EventMousePosition& event) {
	if (self->ignore_events)
		return;

	// Update SOW
	const auto mouse_diff = event.position - self->mouse_position;
	self->mouse_position = event.position;

	// Process event
	const auto scale = self->scale_analog * self->scale_mouse_move;
	self->process_analog(
			InputID{event.position.x >= 0 ? MouseMovement::x_plus : MouseMovement::x_minus},
			InputID{event.position.y >= 0 ? MouseMovement::y_plus : MouseMovement::y_minus},
			mouse_diff,
			scale,
			Origin::mouse);
}

void Controls::event(const libv::input::EventMouseScroll& event) {
	if (self->ignore_events)
		return;

	// Update SOW
	self->scroll_position = event.offset;

	// Process event
	const auto scale = self->scale_analog * self->scale_scroll;
	self->process_analog(
			InputID{event.offset.x >= 0 ? MouseScroll::x_plus : MouseScroll::x_minus},
			InputID{event.offset.y >= 0 ? MouseScroll::y_plus : MouseScroll::y_minus},
			event.offset,
			scale,
			Origin::scroll);
}

void Controls::event(const libv::input::EventGamepadAnalog& event) {
	if (self->ignore_events)
		return;

	// Update SOW
	auto& sow = self->gamepads[event.gamepadID];
	sow.analogs[event.analogID] = event.position;

	// Process event
	const auto scale = self->scale_analog * self->scale_gamepad_analog * sow.scale_analog;
	self->process_analog(
			InputID{event.gamepadID, event.analogID, event.position.x >= 0 ? AnalogDimension::x_plus : AnalogDimension::x_minus},
			InputID{event.gamepadID, event.analogID, event.position.y >= 0 ? AnalogDimension::y_plus : AnalogDimension::y_minus},
			event.position,
			scale,
			Origin::gp_analog);
}

void Controls::event(const libv::input::EventGamepadButton& event) {
	if (self->ignore_events)
		return;

	// Update SOW
	auto& sow = self->gamepads[event.gamepadID];

	if (event.action == ButtonAction::release) {
		sow.pressed_buttons.erase(event.button);
	} else {
		sow.pressed_buttons.insert(event.button);
	}

	// Process event
	const auto impulse = self->impulse_button * self->impulse_gamepad_button * sow.impulse_button;

	const auto event_matcher = [&](const auto inputID) { return alias_match(InputID{event.gamepadID, event.button}, inputID); };
	self->process_button(event_matcher, event.action, impulse);
}

void Controls::event(const libv::input::EventJoystickAnalog& event) {
	if (self->ignore_events)
		return;

	// Update SOW
	auto& sow = self->joysticks[event.joystickID];
	sow.analogs[event.analogID] = event.position;

	// Process event
	const auto scale = self->scale_analog * self->scale_joystick_analog * sow.scale_analog;
	self->process_analog(
			InputID{event.joystickID, event.analogID, event.position.x >= 0 ? AnalogDimension::x_plus : AnalogDimension::x_minus},
			InputID{event.joystickID, event.analogID, event.position.y >= 0 ? AnalogDimension::y_plus : AnalogDimension::y_minus},
			event.position,
			scale,
			Origin::js_analog);
}

void Controls::event(const libv::input::EventJoystickButton& event) {
	if (self->ignore_events)
		return;

	// Update SOW
	auto& sow = self->joysticks[event.joystickID];

	if (event.action == ButtonAction::release) {
		sow.pressed_buttons.erase(event.button);
	} else {
		sow.pressed_buttons.insert(event.button);
	}

	// Process event
	const auto impulse = self->impulse_button * self->impulse_joystick_button * sow.impulse_button;

	const auto event_matcher = [&](const auto inputID) { return alias_match(InputID{event.joystickID, event.button}, inputID); };
	self->process_button(event_matcher, event.action, impulse);
}

// -------------------------------------------------------------------------------------------------

void Controls::update(const duration delta_time) {
	self->last_update_timer.reset();

	if (self->ignore_events)
		return;

	aux_update(delta_time);
}

void Controls::update_since_last_update() {
	if (self->ignore_events) {
		self->last_update_timer.reset();
		return;
	}

	aux_update(self->last_update_timer.time());
}


// -------------------------------------------------------------------------------------------------

void Controls::timeout_sequence(duration timeout) noexcept {
	// NOTE: This could abort some currently active sequences, but the next update will takes care of it
	self->timeout_sequence = timeout;
}

void Controls::impulse_button(scale_type impulse) noexcept {
	self->impulse_button = impulse;
}
void Controls::scale_analog(scale_type_2D scale) noexcept {
	self->scale_analog = scale;
}
void Controls::scale_analog(scale_type scale_xy) noexcept {
	scale_analog({scale_xy, scale_xy});
}
void Controls::scale_analog(scale_type scale_x, scale_type scale_y) noexcept {
	scale_analog({scale_x, scale_y});
}
void Controls::scale_time(scale_type scale) noexcept {
	self->scale_time = scale;
}

void Controls::impulse_gamepad_button(GamepadID gamepadID, scale_type impulse) noexcept {
	self->gamepads[gamepadID].impulse_button = impulse;
}
void Controls::impulse_gamepad_button(scale_type impulse) noexcept {
	self->impulse_gamepad_button = impulse;
}
void Controls::impulse_joystick_button(JoystickID joystickID, scale_type impulse) noexcept {
	self->joysticks[joystickID].impulse_button = impulse;
}
void Controls::impulse_joystick_button(scale_type impulse) noexcept {
	self->impulse_joystick_button = impulse;
}
void Controls::impulse_keyboard(scale_type impulse) noexcept {
	self->impulse_keyboard = impulse;
}
void Controls::impulse_mouse_button(scale_type impulse) noexcept {
	self->impulse_mouse_button = impulse;
}

void Controls::scale_gamepad_analog(GamepadID gamepadID, scale_type scale_xy) noexcept {
	scale_gamepad_analog(gamepadID, {scale_xy, scale_xy});
}
void Controls::scale_gamepad_analog(GamepadID gamepadID, scale_type scale_x, scale_type scale_y) noexcept {
	scale_gamepad_analog(gamepadID, {scale_x, scale_y});
}
void Controls::scale_gamepad_analog(GamepadID gamepadID, scale_type_2D scale) noexcept {
	self->gamepads[gamepadID].scale_analog = scale;
}
void Controls::scale_gamepad_analog(scale_type scale_xy) noexcept {
	scale_gamepad_analog({scale_xy, scale_xy});
}
void Controls::scale_gamepad_analog(scale_type scale_x, scale_type scale_y) noexcept {
	scale_gamepad_analog({scale_x, scale_y});
}
void Controls::scale_gamepad_analog(scale_type_2D scale) noexcept {
	self->scale_gamepad_analog = scale;
}
void Controls::scale_joystick_analog(JoystickID joystickID, scale_type scale_xy) noexcept {
	scale_joystick_analog(joystickID, {scale_xy, scale_xy});
}
void Controls::scale_joystick_analog(JoystickID joystickID, scale_type scale_x, scale_type scale_y) noexcept {
	scale_joystick_analog(joystickID, {scale_x, scale_y});
}
void Controls::scale_joystick_analog(JoystickID joystickID, scale_type_2D scale) noexcept {
	self->joysticks[joystickID].scale_analog = scale;
}
void Controls::scale_joystick_analog(scale_type scale_xy) noexcept {
	scale_joystick_analog(scale_type_2D{scale_xy, scale_xy});
}
void Controls::scale_joystick_analog(scale_type scale_x, scale_type scale_y) noexcept {
	scale_joystick_analog({scale_x, scale_y});
}
void Controls::scale_joystick_analog(scale_type_2D scale) noexcept {
	self->scale_joystick_analog = scale;
}
void Controls::scale_mouse_move(scale_type scale_xy) noexcept {
	scale_mouse_move({scale_xy, scale_xy});
}
void Controls::scale_mouse_move(scale_type scale_x, scale_type scale_y) noexcept {
	scale_mouse_move({scale_x, scale_y});
}
void Controls::scale_mouse_move(scale_type_2D scale) noexcept {
	self->scale_mouse_move = scale;
}
void Controls::scale_scroll(scale_type scale_xy) noexcept {
	scale_scroll({scale_xy, scale_xy});
}
void Controls::scale_scroll(scale_type scale_x, scale_type scale_y) noexcept {
	scale_scroll({scale_x, scale_y});
}
void Controls::scale_scroll(scale_type_2D scale) noexcept {
	self->scale_scroll = scale;
}

// -------------------------------------------------------------------------------------------------

duration Controls::timeout_sequence() const noexcept {
	return self->timeout_sequence;
}

scale_type Controls::impulse_button() const noexcept {
	return self->impulse_button;
}
scale_type_2D Controls::scale_analog() const noexcept {
	return self->scale_analog;
}
scale_type Controls::scale_time() const noexcept {
	return self->scale_time;
}

scale_type Controls::impulse_keyboard() const noexcept {
	return self->impulse_keyboard;
}
scale_type Controls::impulse_mouse_button() const noexcept {
	return self->impulse_mouse_button;
}
scale_type Controls::impulse_gamepad_button() const noexcept {
	return self->impulse_gamepad_button;
}
scale_type Controls::impulse_gamepad_button(GamepadID gamepadID) const noexcept {
	return self->gamepads[gamepadID].impulse_button;
}
scale_type Controls::impulse_joystick_button() const noexcept {
	return self->impulse_joystick_button;
}
scale_type Controls::impulse_joystick_button(JoystickID joystickID) const noexcept {
	return self->joysticks[joystickID].impulse_button;
}

scale_type_2D Controls::scale_gamepad_analog(GamepadID gamepadID) const noexcept {
	return self->gamepads[gamepadID].scale_analog;
}
scale_type_2D Controls::scale_gamepad_analog() const noexcept {
	return self->scale_gamepad_analog;
}
scale_type_2D Controls::scale_joystick_analog(JoystickID joystickID) const noexcept {
	return self->joysticks[joystickID].scale_analog;
}
scale_type_2D Controls::scale_joystick_analog() const noexcept {
	return self->scale_joystick_analog;
}
scale_type_2D Controls::scale_mouse_move() const noexcept {
	return self->scale_mouse_move;
}
scale_type_2D Controls::scale_scroll() const noexcept {
	return self->scale_scroll;
}

// -------------------------------------------------------------------------------------------------

void Controls::register_codepoint(Keycode keycode, Codepoint codepoint) {
	self->codepoint_mappings.insert_or_assign(keycode, codepoint);

	if (self->pressed_keycodes.contains(keycode))
		self->pressed_codepoints.emplace(codepoint);
}

void Controls::unregister_codepoint(Keycode keycode) {
	const auto it = self->codepoint_mappings.find(keycode);

	if (it == self->codepoint_mappings.end())
		return;

	self->codepoint_mappings.erase(it);
	self->pressed_codepoints.erase(it->second);
}

Codepoint Controls::codepoint(Keycode keycode) {
	if (const auto it = self->codepoint_mappings.find(keycode); it != self->codepoint_mappings.end())
		return it->second;

	return Codepoint{0};
}

// -------------------------------------------------------------------------------------------------

void Controls::clear_bindings() {
	self->active_bindings.clear();
	self->deadkey_set.rotate();
	self->bindings.clear();
}

//void Controls::clear_contexts();
//void Controls::clear_features();
//void Controls::clear_sow();

void Controls::clear_codepoints() {
	self->codepoint_mappings.clear();
	self->pressed_codepoints.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
