// Project: libv.ctrl, File: src/libv/ctrl/controls_serialization.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ctrl/controls.hpp>
// libv
#include <libv/utility/float_equal.hpp>
#include <libv/utility/trim.hpp>
// std
#include <sstream>
// pro
#include <libv/ctrl/binding.hpp>
#include <libv/ctrl/binding_level.hpp>
#include <libv/ctrl/controls_impl.lpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

std::string Controls::export_bindings() {
	// Format example:
	//		test.feature #1: A [press]
	//		test.feature #2 *2.0: Ctrl + A

	std::ostringstream os;

	for (const auto& binding : self->bindings) {

		os << binding->feature_name();
		os << " #" << libv::to_value(binding->level);
//		os << " %" << binding.operation_binary;

		if (!libv::float_equal(binding->scale(), scale_type{1}))
			os << " *" << binding->scale();

		os << ": ";
		binding->sequence().to_stream_name(os);
		os << "\n";
	}

	return std::move(os).str();
}

std::string Controls::export_settings() {
	// Format example:
	//		scale_analog: 2.0
	//		gamepad_scale_analog #2: 2.0

	std::ostringstream os;

	const auto out_1D = [&os](auto name, auto value) {
		if (!libv::float_equal(value, scale_type{1}))
			os << name << ": " << value << '\n';
	};

	const auto out_2D = [&os](auto name, auto value) {
		if (!libv::float_equal(value.x, scale_type{1}))
			os << name << "_x: " << value.x << '\n';
		if (!libv::float_equal(value.y, scale_type{1}))
			os << name << "_y: " << value.y << '\n';
	};

	const auto out_div_1D = [&os](auto name, auto id, auto value) {
		if (!libv::float_equal(value, scale_type{1}))
			os << name << " #" << static_cast<int32_t>(libv::to_value(id)) << ": " << value << '\n';
	};

	const auto out_div_2D = [&os](auto name, auto id, auto value) {
		if (!libv::float_equal(value.x, scale_type{1}))
			os << name << "_x #" << static_cast<int32_t>(libv::to_value(id)) << ": " << value.x << '\n';
		if (!libv::float_equal(value.y, scale_type{1}))
			os << name << "_y #" << static_cast<int32_t>(libv::to_value(id)) << ": " << value.y << '\n';
	};

	if (!libv::float_equal(self->timeout_sequence.count(), scale_type{5}))
		os << "timeout_sequence" << ": " << self->timeout_sequence.count() << '\n';

	out_2D("scale_analog", self->scale_analog);
	out_1D("impulse_button", self->impulse_button);
	out_1D("scale_time", self->scale_time);

	out_1D("impulse_mouse_button", self->impulse_mouse_button);
	out_2D("scale_mouse_move", self->scale_mouse_move);
	out_2D("scale_scroll", self->scale_scroll);
	out_1D("impulse_keyboard", self->impulse_keyboard);

	out_1D("impulse_gamepad_button", self->impulse_gamepad_button);
	out_2D("scale_gamepad_analog", self->scale_gamepad_analog);
	for (const auto& gamepad : self->gamepads) {
		out_div_1D("gamepad_impulse_button", gamepad.first, gamepad.second.impulse_button);
		out_div_2D("gamepad_scale_analog", gamepad.first, gamepad.second.scale_analog);
	}

	out_1D("impulse_joystick_button", self->impulse_joystick_button);
	out_2D("scale_joystick_analog", self->scale_joystick_analog);
	for (const auto& joystick : self->joysticks) {
		out_div_1D("joystick_impulse_button", joystick.first, joystick.second.impulse_button);
		out_div_2D("joystick_scale_analog", joystick.first, joystick.second.scale_analog);
	}

	return std::move(os).str();
}

void Controls::import_bindings(std::string_view data) {
	// TODO P4: C++20 replace temporary strings that are created for stoi and stod
	while (!data.empty()) {
		const auto it_lf = data.find('\n');
		const auto line = libv::trim(data.substr(0, it_lf));
		data.remove_prefix(it_lf + 1);

		if (line.empty() || line.starts_with("//"))
			continue;

		const auto it_seq = line.find(":");

		const auto range_info = line.substr(0, it_seq);
		const auto range_sequence = libv::trim(line.substr(it_seq + 1));

		const auto it_number = range_info.find("#");
		const auto it_scale = range_info.find("*");
		const bool has_scale = it_scale == std::string_view::npos;

		const auto range_feature = libv::trim(range_info.substr(0, it_number));
		const auto range_level = libv::trim(range_info.substr(it_number + 1, has_scale ? (it_scale - it_number - 1) : std::string_view::npos));
		const auto range_scale = libv::trim(range_info.substr(it_scale + 1));

		bind(
				std::string(range_feature),
				range_sequence,
				has_scale ? scale_type{1} : std::stod(std::string(range_scale)),
				binding_level{std::stoi(std::string(range_level))}
		);
	}
}

void Controls::import_settings(std::string_view data) {
	// TODO P4: C++20 replace temporary strings that are created for stoi and stod
	while (!data.empty()) {
		const auto it_lf = data.find('\n');
		const auto line = libv::trim(data.substr(0, it_lf));
		data.remove_prefix(it_lf + 1);

		if (line.empty() || line.starts_with("//"))
			continue;

		const auto it_seq = line.find(":");
		const auto it_div = line.find("#");
		const auto has_div = it_div != std::string_view::npos;

		const auto range_info = libv::trim(line.substr(0, it_seq));
		const auto range_key = libv::trim(range_info.substr(0, it_div));
		const auto range_id = libv::trim(range_info.substr(it_div + 1));
		const auto range_value = libv::trim(line.substr(it_seq + 1));

		const auto in_1D = [&](auto name, auto& var) {
			if (range_key == name)
				var = std::stod(std::string(range_value));
		};

		const auto in_2D = [&](auto name, auto& var) {
			if (range_key.starts_with(name) && range_key.ends_with("_x"))
				var.x = std::stod(std::string(range_value));
			if (range_key.starts_with(name) && range_key.ends_with("_y"))
				var.y = std::stod(std::string(range_value));
		};

		const auto in_div_1D = [&](auto name, auto& map, auto mptr) {
			using id = std::remove_reference_t<decltype(map)>::key_type;

			if (has_div && range_key == name)
				map[static_cast<id>(std::stoi(std::string(range_id)))].*mptr = std::stod(std::string(range_value));
		};

		const auto in_div_2D = [&](auto name, auto& map, auto mptr) {
			using id = std::remove_reference_t<decltype(map)>::key_type;

			if (has_div && range_key.starts_with(name) && range_key.ends_with("_x"))
				(map[static_cast<id>(std::stoi(std::string(range_id)))].*mptr).x = std::stod(std::string(range_value));
			if (has_div && range_key.starts_with(name) && range_key.ends_with("_y"))
				(map[static_cast<id>(std::stoi(std::string(range_id)))].*mptr).y = std::stod(std::string(range_value));
		};

		if (range_key == "timeout_sequence")
				self->timeout_sequence = duration{std::stod(std::string(libv::trim(range_value)))};

		in_2D("scale_analog", self->scale_analog);
		in_1D("impulse_button", self->impulse_button);
		in_1D("scale_time", self->scale_time);

		in_1D("impulse_mouse_button", self->impulse_mouse_button);
		in_2D("scale_mouse_move", self->scale_mouse_move);
		in_2D("scale_scroll", self->scale_scroll);
		in_1D("impulse_keyboard", self->impulse_keyboard);

		in_1D("impulse_gamepad_button", self->impulse_gamepad_button);
		in_2D("scale_gamepad_analog", self->scale_gamepad_analog);

		in_div_1D("gamepad_impulse_button", self->gamepads, &ImplControls::GamepadState::impulse_button);
		in_div_2D("gamepad_scale_analog", self->gamepads, &ImplControls::GamepadState::scale_analog);

		in_1D("impulse_joystick_button", self->impulse_joystick_button);
		in_2D("scale_joystick_analog", self->scale_joystick_analog);

		in_div_1D("joystick_impulse_button", self->joysticks, &ImplControls::JoystickState::impulse_button);
		in_div_2D("joystick_scale_analog", self->joysticks, &ImplControls::JoystickState::scale_analog);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
