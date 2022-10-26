// Project: libv, File: app/star/game/scene/controls_layout.hpp

#pragma once

// libv
#include <libv/ctrl/input_id.hpp>
#include <libv/math/vec.hpp>
// std
#include <vector>


//#include <libv/ctrl/controls.hpp>
//#include <libv/ctrl/binding.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

//float sign(fPoint p1, fPoint p2, fPoint p3) {
//	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
//}
//
//bool PointInTriangle(fPoint pt, fPoint v1, fPoint v2, fPoint v3) {
//	float d1, d2, d3;
//	bool has_neg, has_pos;
//
//	d1 = sign(pt, v1, v2);
//	d2 = sign(pt, v2, v3);
//	d3 = sign(pt, v3, v1);
//
//	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
//	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
//
//	return !(has_neg && has_pos);
//}

//std::vector<libv::vec2f> buildPolygon(libv::math::BezierCurve outline) {

//std::vector<libv::vec2f> triangulate_outline(std::vector<libv::vec2f> outline) {
//	using vec = libv::vec2f;
//
//	std::vector<vec> vertices;
//
//	std::size_t last = 0;
//	for (std::size_t i = 0; i < outline.size(); ++i) {
//		if (is_curvature_negative)
//			continue;
//
//		if (other_vertex_inside_the_triangle)
//			continue;
//
//		remove_middle();
//		last++;
//		pack_based_on_removed();
//
////		outline
//
//		vertices.emplace_back(outline[i + 0]);
//		vertices.emplace_back(outline[i + 1]);
//		vertices.emplace_back(outline[i + 2]);
//	}
//
//	return vertices;
//}
//
//void buildPolygon() {
//	using vec = libv::vec2f;
//	std::vector<vec> vertices;
//
//	const auto linear = [&](vec p0, vec p1) {
//		vertices.emplace_back(p0);
//		vertices.emplace_back(p1);
//	};
//
//	const auto bezier = [&](vec p0, vec p1, vec p2, vec p3, std::size_t n) {
//		const auto nf = 1.0f / static_cast<float>(n);
//
//		vertices.emplace_back(p0);
//		for (std::size_t i = 1; i < n - 1; ++i) {
//			const auto if_ = static_cast<float>(i);
//			const auto t = nf * if_;
//			vertices.emplace_back(libv::math::bezierCurvePoint(p0, p1, p2, p3, t));
//		}
//		vertices.emplace_back(p3);
//	};
//
//
//	linear({0, 0}, {1, 0});
//	linear({1, 1}, {0, 1});
//	bezier({0, 0}, {0, 1}, {1, 1}, {1, 0});
//}


struct Analog {
	// ...
};

struct Button {
	libv::ctrl::InputID input;
	libv::vec2f position;
	libv::vec2f size;
};

// ---

//void foo() {
//	libv::ctrl::Controls controls;
//	std::vector<Button> keyboard_buttons;
//
//	for (const Button& button : keyboard_buttons) {
//		controls.foreach_bindings_search([&](const libv::ctrl::Binding& binding) {
//			static constexpr auto c = libv::ctrl::Modifier::control;
//			static constexpr auto s = libv::ctrl::Modifier::shift;
//			static constexpr auto a = libv::ctrl::Modifier::alt;
////			static constexpr auto u = libv::ctrl::Modifier::super;
//
//			if (binding.sequence().contains(c + s + a))
//				0;
//			else if (binding.sequence().contains(c + s))
//				0;
//			else if (binding.sequence().contains(c + a))
//				0;
//			else if (binding.sequence().contains(s + a))
//				0;
//			else if (binding.sequence().contains(c))
//				0;
//			else if (binding.sequence().contains(s))
//				0;
//			else if (binding.sequence().contains(a))
//				0;
//			else
//				0;
//
//			// OR just
//			if (binding.sequence().contains(viewed_current_modifiers))
//				1;
//
//			binding.feature_name();
//
//		}, button.input, libv::ctrl::search_mode::contains);
//	}
//}

// ---

struct Mouse {

};

struct Keyboard {
	std::vector<Button> buttons;
	libv::vec2f size;
//	libv::glr::Mesh& mesh;

//	// TODO P5: implement the 4 physical layout of the keyboard
//	enum class physical_layout {
//		layout_a, /// Long backspace, Single lined enter
//		layout_b, /// Long backspace, Top bitten two lined enter
//		layout_c, /// Short backspace, Bottom bitten two lined enter
//		merged, /// TODO P5: Whatever is currently implemented
//	};

	explicit Keyboard(float unit = 1.0f, float gap = 0.15f) :
		Keyboard(unit, gap, gap * 2.f) {
	}

	explicit Keyboard(float unit, float gap, float big_gap_value) {
		using vec2 = libv::vec2f;

		const auto big_gap = big_gap_value - gap; // minus gap to simplify calculations

		size.x = 22 * (unit + gap) + 2.f * big_gap - gap;
		size.y = 6 * (unit + gap) + big_gap - gap;

		auto pen = vec2{};

		const auto add = [&](libv::ctrl::Keycode key, vec2 position, vec2 size) {
			buttons.emplace_back(libv::ctrl::InputID(key), position, size);
		};

		const auto place_in_row = [&](auto keycode, auto size_x) {
			add(keycode, pen, vec2(size_x, unit));
			pen.x += size_x + gap;
		};

		const auto place_in_2row = [&](auto keycode, auto size_x) {
			add(keycode, pen, vec2(size_x, unit * 2 + gap));
			pen.x += size_x + gap;
		};

		const auto new_row = [&]() {
			pen.x = 0;
			pen.y += unit + gap;
		};

		// Bottom row modifier size, based on space alignment with num4
		// Ctrl + Win + Alt matches Backtick + 1 + 2 + 3
		const auto brm_size = (4 * unit + 3 * gap - 1 * unit - 2 * gap) / 2.0f;

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
		pen.x += (5 * unit + 6 * gap - 4 * unit - 3 * gap) / 2.0f - gap; // F4 aligned with Num5, F9 aligned with minus
		place_in_row(libv::ctrl::Keycode::F5, unit);
		place_in_row(libv::ctrl::Keycode::F6, unit);
		place_in_row(libv::ctrl::Keycode::F7, unit);
		place_in_row(libv::ctrl::Keycode::F8, unit);
		pen.x += (5 * unit + 6 * gap - 4 * unit - 3 * gap) / 2.0f - gap; // F4 aligned with Num5, F9 aligned with minus
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

} // namespace star
