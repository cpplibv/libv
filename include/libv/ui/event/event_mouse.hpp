// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// libv
#include <libv/input/inputs.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/optional_ref.hpp>
// std
#include <variant>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct EventMouse {
	struct Button {
		libv::input::Mouse button;
		libv::input::Action action;
	};
	struct Movement {
		libv::vec2f mouse_position;
		libv::vec2f mouse_movement;

		bool enter = false;
		bool leave = false;
	};
	struct Scroll {
		libv::vec2f scroll_position;
		libv::vec2f scroll_movement;
		//TYPE scroll_unit;
	};

//	libv::vec2f mouse_position;
//	libv::vec2f scroll_position;
//	boost::container::flat_map<libv::input::Mouse, libv::input::Action> mouse_buttons;

	std::variant<Button, Movement, Scroll> value;
	// key states
	// joystick states
	// mouse states
	// time states
	// hotkey states (?)

public:
	constexpr inline bool isButton() const noexcept {
		return std::holds_alternative<Button>(value);
	}
	constexpr inline bool isMovement() const noexcept {
		return std::holds_alternative<Movement>(value);
	}
	constexpr inline bool isScroll() const noexcept {
		return std::holds_alternative<Scroll>(value);
	}

public:
	template <typename F>
	constexpr inline void ifButton(F&& func) const {
		if (isButton())
			std::forward<F>(func)(std::get<Button>(value));
	}
	template <typename F>
	constexpr inline void ifMovement(F&& func) const {
		if (isMovement())
			std::forward<F>(func)(std::get<Movement>(value));
	}
	template <typename F>
	constexpr inline void ifScroll(F&& func) const {
		if (isScroll())
			std::forward<F>(func)(std::get<Scroll>(value));
	}

public:
	constexpr inline const Button& button() const {
		return std::get<Button>(value);
	}
	constexpr inline const Movement& movement() const {
		return std::get<Movement>(value);
	}
	constexpr inline const Scroll& scroll() const {
		return std::get<Scroll>(value);
	}

public:
	constexpr inline libv::optional_ref<const Button> buttonOptional() const {
		if (isButton())
			return libv::make_optional_ref(std::get<Button>(value));
		else
			return nullptr;
	}
	constexpr inline libv::optional_ref<const Movement> movementOptional() const {
		if (isMovement())
			return libv::make_optional_ref(std::get<Movement>(value));
		else
			return nullptr;
	}
	constexpr inline libv::optional_ref<const Scroll> scrollOptional() const {
		if (isScroll())
			return libv::make_optional_ref(std::get<Scroll>(value));
		else
			return nullptr;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
