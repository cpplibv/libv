// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/ctrl/input_id.hpp>
// std
#include <vector>
#include <string>


// libv
#include <libv/glr/procedural/cube.hpp>
#include <libv/glr/procedural/grid.hpp>
#include <libv/glr/procedural/ignore.hpp>
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/queue.hpp>
#include <libv/math/angle.hpp>
// pro
#include <vm4_viewer/attribute.hpp>
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/ui/keyboard.hpp>


namespace app {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct Button {
	libv::ctrl::InputID input;
	libv::vec2f position;
	libv::vec2f size;
};

// ---

struct Mouse {

};

struct Keyboard {
//	std::vector<Button> buttons;
	libv::glr::Mesh& mesh;



	void foo() {
		auto pos = mesh.attribute(attribute_position);
		auto index = mesh.index();

		int i = 0;
		const auto add = [&](libv::ctrl::Keycode, libv::vec2f position, libv::vec2f size) {
			pos(position.x, position.y, 0);
			pos(position.x + size.x, position.y, 0);
			pos(position.x + size.x, position.y, 0);
			pos(position.x + size.x, position.y + size.y, 0);
			pos(position.x + size.x, position.y + size.y, 0);
			pos(position.x, position.y + size.y, 0);
			pos(position.x, position.y + size.y, 0);
			pos(position.x, position.y, 0);

			index.line(i, i + 1); i += 2;
			index.line(i, i + 1); i += 2;
			index.line(i, i + 1); i += 2;
			index.line(i, i + 1); i += 2;

	//		buttons.emplace_back(std::forward<Args>(args)...);
		};

		using vec = libv::vec2f;

		const auto unit = 1.0f;
		const auto gap = 0.15f;
		const auto big_gap = gap * 3 - gap;

		auto pen = vec{};

		const auto place_in_row = [&](auto keycode, auto size_x) {
			add(keycode, pen, vec(size_x, unit));
			pen.x += size_x + gap;
		};

		const auto place_in_2row = [&](auto keycode, auto size_x) {
			add(keycode, pen, vec(size_x, unit * 2 + gap));
			pen.x += size_x + gap;
		};

		const auto new_row = [&]() {
			pen.x = 0;
			pen.y += unit + gap;
		};

		// Bottom row modifier size, based on space alignment with num4
		const auto brm_size = (4 * unit + 3 * gap - 1 * unit - 2 * gap) / 2.0f; // Ctrl+Win+Alt matches Backtick,num1,num2,num3

		//
		// Layout
		//

		place_in_row(libv::ctrl::Keycode::ControlLeft, brm_size);
		place_in_row(libv::ctrl::Keycode::SuperLeft, unit);
		place_in_row(libv::ctrl::Keycode::AltLeft, brm_size);
		place_in_row(libv::ctrl::Keycode::Space, 15 * unit + 14 * gap - 4 * brm_size - 3 * unit - 7 * gap); // Uses remaining space
		place_in_row(libv::ctrl::Keycode::AltRight, brm_size);
		place_in_row(libv::ctrl::Keycode::SuperRight, unit);
		place_in_row(libv::ctrl::Keycode::Menu, unit);
		place_in_row(libv::ctrl::Keycode::ControlRight, brm_size);

		pen.x += big_gap;
		place_in_row(libv::ctrl::Keycode::Left, unit);
		place_in_row(libv::ctrl::Keycode::Down, unit);
		place_in_row(libv::ctrl::Keycode::Right, unit);

		pen.x += big_gap;
		place_in_row(libv::ctrl::Keycode::KPNum0, 2 * unit + gap); // Matches KPNum1 and KPNum2
		place_in_row(libv::ctrl::Keycode::KPDot, unit);
		place_in_2row(libv::ctrl::Keycode::KPEnter, unit);

		new_row(); // -----

		const auto size_lshift = (3 * unit + 2 * gap - gap) / 2.0f + gap + 2.0f / 3.0f * unit - unit - gap;
		place_in_row(libv::ctrl::Keycode::ShiftLeft, size_lshift); // Align the Z to 2/3 of Q
		place_in_row(libv::ctrl::Keycode::World1, unit);
		place_in_row(libv::ctrl::Keycode::Z, unit);
		place_in_row(libv::ctrl::Keycode::X, unit);
		place_in_row(libv::ctrl::Keycode::C, unit);
		place_in_row(libv::ctrl::Keycode::V, unit);
		place_in_row(libv::ctrl::Keycode::B, unit);
		place_in_row(libv::ctrl::Keycode::N, unit);
		place_in_row(libv::ctrl::Keycode::M, unit);
		place_in_row(libv::ctrl::Keycode::Comma, unit);
		place_in_row(libv::ctrl::Keycode::Period, unit);
		place_in_row(libv::ctrl::Keycode::Slash, unit);
		place_in_row(libv::ctrl::Keycode::World2, unit);
		place_in_row(libv::ctrl::Keycode::ShiftRight, 15 * unit + 14 * gap - size_lshift - 12 * unit - 13 * gap);

		pen.x += big_gap + unit + gap;
		place_in_row(libv::ctrl::Keycode::Up, unit);

		pen.x += big_gap + unit + gap;
		place_in_row(libv::ctrl::Keycode::KPNum1, unit);
		place_in_row(libv::ctrl::Keycode::KPNum2, unit);
		place_in_row(libv::ctrl::Keycode::KPNum3, unit);

		new_row(); // -----

		const auto size_caps = (3 * unit + 2 * gap - gap) / 2.0f + gap + 1.0f / 3.0f * unit;
		place_in_row(libv::ctrl::Keycode::CapsLock, size_caps); // Align the A to 1/3 of Q
		place_in_row(libv::ctrl::Keycode::A, unit);
		place_in_row(libv::ctrl::Keycode::S, unit);
		place_in_row(libv::ctrl::Keycode::D, unit);
		place_in_row(libv::ctrl::Keycode::F, unit);
		place_in_row(libv::ctrl::Keycode::G, unit);
		place_in_row(libv::ctrl::Keycode::H, unit);
		place_in_row(libv::ctrl::Keycode::J, unit);
		place_in_row(libv::ctrl::Keycode::K, unit);
		place_in_row(libv::ctrl::Keycode::L, unit);
		place_in_row(libv::ctrl::Keycode::Semicolon, unit);
		place_in_row(libv::ctrl::Keycode::Apostrophe, unit);
		place_in_row(libv::ctrl::Keycode::Enter, 15 * unit + 14 * gap - size_caps - 11 * unit - 12 * gap);

		pen.x += big_gap * 2 + unit * 3 + gap * 3;
		place_in_row(libv::ctrl::Keycode::KPNum4, unit);
		place_in_row(libv::ctrl::Keycode::KPNum5, unit);
		place_in_row(libv::ctrl::Keycode::KPNum6, unit);
		place_in_2row(libv::ctrl::Keycode::KPPlus, unit);

		new_row(); // -----

		place_in_row(libv::ctrl::Keycode::Tab, (3 * unit + 2 * gap - gap) / 2.0f); // Align Tab-Q gap to middle of num1
		place_in_row(libv::ctrl::Keycode::Q, unit);
		place_in_row(libv::ctrl::Keycode::W, unit);
		place_in_row(libv::ctrl::Keycode::E, unit);
		place_in_row(libv::ctrl::Keycode::R, unit);
		place_in_row(libv::ctrl::Keycode::T, unit);
		place_in_row(libv::ctrl::Keycode::Y, unit);
		place_in_row(libv::ctrl::Keycode::U, unit);
		place_in_row(libv::ctrl::Keycode::I, unit);
		place_in_row(libv::ctrl::Keycode::O, unit);
		place_in_row(libv::ctrl::Keycode::P, unit);
		place_in_row(libv::ctrl::Keycode::BracketOpen, unit);
		place_in_row(libv::ctrl::Keycode::BracketClose, unit);
		place_in_row(libv::ctrl::Keycode::Backslash, (3 * unit + 2 * gap - gap) / 2.0f); // Matches Tab

		pen.x += big_gap;
		place_in_row(libv::ctrl::Keycode::Delete, unit);
		place_in_row(libv::ctrl::Keycode::End, unit);
		place_in_row(libv::ctrl::Keycode::PageDown, unit);

		pen.x += big_gap;
		place_in_row(libv::ctrl::Keycode::KPNum7, unit);
		place_in_row(libv::ctrl::Keycode::KPNum8, unit);
		place_in_row(libv::ctrl::Keycode::KPNum9, unit);

		new_row(); // -----

		place_in_row(libv::ctrl::Keycode::Backtick, unit);
		place_in_row(libv::ctrl::Keycode::Num1, unit);
		place_in_row(libv::ctrl::Keycode::Num2, unit);
		place_in_row(libv::ctrl::Keycode::Num3, unit);
		place_in_row(libv::ctrl::Keycode::Num4, unit);
		place_in_row(libv::ctrl::Keycode::Num5, unit);
		place_in_row(libv::ctrl::Keycode::Num6, unit);
		place_in_row(libv::ctrl::Keycode::Num7, unit);
		place_in_row(libv::ctrl::Keycode::Num8, unit);
		place_in_row(libv::ctrl::Keycode::Num9, unit);
		place_in_row(libv::ctrl::Keycode::Num0, unit);
		place_in_row(libv::ctrl::Keycode::Minus, unit);
		place_in_row(libv::ctrl::Keycode::Equals, unit);
		place_in_row(libv::ctrl::Keycode::Backspace, 2 * unit + gap); // Matches F11 and F12

		pen.x += big_gap;
		place_in_row(libv::ctrl::Keycode::Insert, unit);
		place_in_row(libv::ctrl::Keycode::Home, unit);
		place_in_row(libv::ctrl::Keycode::PageUp, unit);

		pen.x += big_gap;
		place_in_row(libv::ctrl::Keycode::NumLock, unit);
		place_in_row(libv::ctrl::Keycode::KPSlash, unit);
		place_in_row(libv::ctrl::Keycode::KPAsterisk, unit);
		place_in_row(libv::ctrl::Keycode::KPMinus, unit);

		new_row(); // -----
		pen.y += big_gap;

		place_in_row(libv::ctrl::Keycode::Escape, unit);
		pen.x += unit + gap;
		place_in_row(libv::ctrl::Keycode::F1, unit);
		place_in_row(libv::ctrl::Keycode::F2, unit);
		place_in_row(libv::ctrl::Keycode::F3, unit);
		place_in_row(libv::ctrl::Keycode::F4, unit);
		pen.x += (5 * unit + 6 * gap - 4 * unit - 3 * gap) / 2.0f - gap; // F4 aligned with num5, F9 aligned with minus
		place_in_row(libv::ctrl::Keycode::F5, unit);
		place_in_row(libv::ctrl::Keycode::F6, unit);
		place_in_row(libv::ctrl::Keycode::F7, unit);
		place_in_row(libv::ctrl::Keycode::F8, unit);
		pen.x += (5 * unit + 6 * gap - 4 * unit - 3 * gap) / 2.0f - gap; // F4 aligned with num5, F9 aligned with minus
		place_in_row(libv::ctrl::Keycode::F9, unit);
		place_in_row(libv::ctrl::Keycode::F10, unit);
		place_in_row(libv::ctrl::Keycode::F11, unit);
		place_in_row(libv::ctrl::Keycode::F12, unit);

		pen.x += big_gap;
		place_in_row(libv::ctrl::Keycode::PrintScreen, unit);
		place_in_row(libv::ctrl::Keycode::ScrollLock, unit);
		place_in_row(libv::ctrl::Keycode::Pause, unit);
	}
};

struct Gamepad {

};

struct Joystick {

};

// ---

struct InputSet {
	Mouse mouse;
	Keyboard keyboard;
	std::vector<Gamepad> gamepads;
	std::vector<Joystick> joysticks;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace app
