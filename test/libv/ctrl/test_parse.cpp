// Project: libv.ctrl, File: test/libv/ctrl/test_parse.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// libv
#include <libv/utility/utf8.hpp>
// pro
#include <libv/ctrl/parse.hpp>
#include <libv/ctrl/sequence.hpp>


// -------------------------------------------------------------------------------------------------

namespace lc = libv::ctrl;

const auto utf8_codepoint = [](std::string_view input) {
	uint32_t codepoint = 0;
	auto it = input.begin();
	const auto valid_utf8_codepoint = libv::next_unicode(it, input.end(), codepoint);

	if (!valid_utf8_codepoint || it != input.end())
		throw std::runtime_error("Invalid test input utf8 codepoint string: " + std::string(input));

	return lc::Codepoint{codepoint};
};

// -------------------------------------------------------------------------------------------------

TEST_CASE("Sequence to_string", "[libv.ctrl.parse]") {
	lc::Sequence s;
	lc::Combination c;
	lc::Input iW{lc::Keycode::W, lc::DigitalInputAction::press};
	lc::Input iC{lc::Keycode::ControlLeft, lc::DigitalInputAction::auto_};

	c.inputs.emplace_back(iC);
	c.inputs.emplace_back(iW);

	s.combinations.emplace_back(c);

	CHECK(s.to_string_name() == "LCtrl + W [press]");

	s.combinations.emplace_back(c);

	CHECK(s.to_string_name() == "LCtrl + W [press], LCtrl + W [press]");
}

TEST_CASE("Parse inputID general", "[libv.ctrl.parse]") {
	const auto test = [](std::string_view input, auto... args) {
		const auto result = lc::parse_input_id_or_throw(input);
		const auto expected = lc::InputID(args...);

		CHECK(result == expected);
		// Second pass consistency check, verify round trips
		CHECK(lc::parse_input_id_or_throw(result.to_string_name()) == result);
		CHECK(lc::parse_input_id_or_throw(result.to_string_symbol()) == result);
	};

	// Modifier
	test("control       ", lc::Modifier::control);
	test("ctrl          ", lc::Modifier::control);
	test("alt           ", lc::Modifier::alt);
	test("shft          ", lc::Modifier::shift);
	test("SHIFT         ", lc::Modifier::shift);
	test("Shift         ", lc::Modifier::shift);
	test("shift         ", lc::Modifier::shift);
	test("super         ", lc::Modifier::super);
	test("sper          ", lc::Modifier::super);

	// Scancode
	test("sc0           ", lc::Scancode{0});
	test("sc1           ", lc::Scancode{1});
	test("sc10          ", lc::Scancode{10});
	test("sc150         ", lc::Scancode{150});
	test("sc 150        ", lc::Scancode{150});
	test("Sc150         ", lc::Scancode{150});
	test("SC150         ", lc::Scancode{150});
	test("scan150       ", lc::Scancode{150});
	test("scancode150   ", lc::Scancode{150});
	test("scancode 150  ", lc::Scancode{150});
	test("sc-15         ", lc::Scancode{-15});

	// Keycode
	test("x             ", lc::Keycode::X);
	test("X             ", lc::Keycode::X);
	test("Y             ", lc::Keycode::Y);
	test("S             ", lc::Keycode::S);
	test("up            ", lc::Keycode::Up);
	test("Up            ", lc::Keycode::Up);
	test("UP            ", lc::Keycode::Up);
	test("shift left    ", lc::Keycode::ShiftLeft);
	test("Shift Left    ", lc::Keycode::ShiftLeft);
	test("Left Shift    ", lc::Keycode::ShiftLeft);
	test("alt gr        ", lc::Keycode::AltRight);
	test(",             ", lc::Keycode::Comma);
	test("'             ", lc::Keycode::Apostrophe);

	// Codepoint
	test(R"("ɫ"        )", utf8_codepoint("ɫ"));
	test(R"("á"        )", utf8_codepoint("á"));
	test(R"("x"        )", utf8_codepoint("x"));
	test(R"('x'        )", utf8_codepoint("x"));
	test(R"("X"        )", utf8_codepoint("X"));
	test(R"('X'        )", utf8_codepoint("X"));
	test(R"(","        )", utf8_codepoint(","));
	test(R"("'"        )", utf8_codepoint("'"));
	test(R"('"'        )", utf8_codepoint("\""));
	test(R"("\'"       )", utf8_codepoint("'"));
	test(R"('\''       )", utf8_codepoint("'"));
	test(R"("\""       )", utf8_codepoint("\""));
	test(R"('\"'       )", utf8_codepoint("\""));

	// Mouse Button
	test("0 m           ", lc::MouseButton::Left);
	test("0m            ", lc::MouseButton::Left);
	test("0mb           ", lc::MouseButton::Left);
	test("l m           ", lc::MouseButton::Left);
	test("l mouse       ", lc::MouseButton::Left);
	test("l mouse btn   ", lc::MouseButton::Left);
	test("l mouse button", lc::MouseButton::Left);
	test("left m        ", lc::MouseButton::Left);
	test("left mb       ", lc::MouseButton::Left);
	test("leftmb        ", lc::MouseButton::Left);
	test("leftmouse     ", lc::MouseButton::Left);
	test("lm            ", lc::MouseButton::Left);
	test("lmb           ", lc::MouseButton::Left);
	test("m 0           ", lc::MouseButton::Left);
	test("m left        ", lc::MouseButton::Left);
	test("m0            ", lc::MouseButton::Left);
	test("mb 0          ", lc::MouseButton::Left);
	test("mb0           ", lc::MouseButton::Left);
	test("mleft         ", lc::MouseButton::Left);
	test("mouse 0       ", lc::MouseButton::Left);
	test("mouse left    ", lc::MouseButton::Left);
	test("mouse0        ", lc::MouseButton::Left);
	test("m m           ", lc::MouseButton::Middle);
	test("m mb          ", lc::MouseButton::Middle);
	test("m mouse       ", lc::MouseButton::Middle);
	test("mb m          ", lc::MouseButton::Middle);
	test("mbm           ", lc::MouseButton::Middle);
	test("mm            ", lc::MouseButton::Middle);
	test("mmb           ", lc::MouseButton::Middle);
	test("mb16          ", lc::MouseButton::Button16);
	test("mouse16       ", lc::MouseButton::Button16);
	test("mouse 16      ", lc::MouseButton::Button16);

	// Mouse Movement
	test("m Hori        ", lc::MouseMovement::x);
	test("m Horizontal  ", lc::MouseMovement::x);
	test("m X           ", lc::MouseMovement::x);
	test("m h           ", lc::MouseMovement::x);
	test("mh            ", lc::MouseMovement::x);
	test("mouse X       ", lc::MouseMovement::x);
	test("mouse x       ", lc::MouseMovement::x);
	test("mousex        ", lc::MouseMovement::x);
	test("mx            ", lc::MouseMovement::x);
	test("m Y           ", lc::MouseMovement::y);
	test("m vertical    ", lc::MouseMovement::y);
	test("mv            ", lc::MouseMovement::y);
	test("my            ", lc::MouseMovement::y);
	test("m Hori+       ", lc::MouseMovement::x_plus);
	test("m Horizontal+ ", lc::MouseMovement::x_plus);
	test("m X+          ", lc::MouseMovement::x_plus);
	test("m h+          ", lc::MouseMovement::x_plus);
	test("mh+           ", lc::MouseMovement::x_plus);
	test("mouse X+      ", lc::MouseMovement::x_plus);
	test("mouse x+      ", lc::MouseMovement::x_plus);
	test("mousex+       ", lc::MouseMovement::x_plus);
	test("mx+           ", lc::MouseMovement::x_plus);
	test("m Y+          ", lc::MouseMovement::y_plus);
	test("m vertical+   ", lc::MouseMovement::y_plus);
	test("mv+           ", lc::MouseMovement::y_plus);
	test("my+           ", lc::MouseMovement::y_plus);
	test("m Hori-       ", lc::MouseMovement::x_minus);
	test("m Horizontal- ", lc::MouseMovement::x_minus);
	test("m X-          ", lc::MouseMovement::x_minus);
	test("m h-          ", lc::MouseMovement::x_minus);
	test("mh-           ", lc::MouseMovement::x_minus);
	test("mouse X-      ", lc::MouseMovement::x_minus);
	test("mouse x-      ", lc::MouseMovement::x_minus);
	test("mousex-       ", lc::MouseMovement::x_minus);
	test("mx-           ", lc::MouseMovement::x_minus);
	test("m Y-          ", lc::MouseMovement::y_minus);
	test("m vertical-   ", lc::MouseMovement::y_minus);
	test("mv-           ", lc::MouseMovement::y_minus);
	test("my-           ", lc::MouseMovement::y_minus);

	// Mouse scroll
	test("Wheel         ", lc::MouseScroll::y);
	test("Scroll        ", lc::MouseScroll::y);
	test("MouseWheel    ", lc::MouseScroll::y);
	test("MouseScroll   ", lc::MouseScroll::y);
	test("MS            ", lc::MouseScroll::y);
	test("MW            ", lc::MouseScroll::y);
	test("WheelY        ", lc::MouseScroll::y);
	test("ScrollY       ", lc::MouseScroll::y);
	test("MouseWheelY   ", lc::MouseScroll::y);
	test("MouseScrollY  ", lc::MouseScroll::y);
	test("MSY           ", lc::MouseScroll::y);
	test("MWY           ", lc::MouseScroll::y);
	test("WheelX        ", lc::MouseScroll::x);
	test("ScrollX       ", lc::MouseScroll::x);
	test("MSX           ", lc::MouseScroll::x);
	test("MWX           ", lc::MouseScroll::x);
	test("Wheel+        ", lc::MouseScroll::y_plus);
	test("Scroll+       ", lc::MouseScroll::y_plus);
	test("MouseWheel+   ", lc::MouseScroll::y_plus);
	test("MouseScroll+  ", lc::MouseScroll::y_plus);
	test("MS+           ", lc::MouseScroll::y_plus);
	test("MW+           ", lc::MouseScroll::y_plus);
	test("WheelY+       ", lc::MouseScroll::y_plus);
	test("ScrollY+      ", lc::MouseScroll::y_plus);
	test("MouseWheelY+  ", lc::MouseScroll::y_plus);
	test("MouseScrollY+ ", lc::MouseScroll::y_plus);
	test("MSY+          ", lc::MouseScroll::y_plus);
	test("MWY+          ", lc::MouseScroll::y_plus);
	test("WheelX+       ", lc::MouseScroll::x_plus);
	test("ScrollX+      ", lc::MouseScroll::x_plus);
	test("MSX+          ", lc::MouseScroll::x_plus);
	test("MWX+          ", lc::MouseScroll::x_plus);
	test("Wheel-        ", lc::MouseScroll::y_minus);
	test("Scroll-       ", lc::MouseScroll::y_minus);
	test("MouseWheel-   ", lc::MouseScroll::y_minus);
	test("MouseScroll-  ", lc::MouseScroll::y_minus);
	test("MS-           ", lc::MouseScroll::y_minus);
	test("MW-           ", lc::MouseScroll::y_minus);
	test("WheelY-       ", lc::MouseScroll::y_minus);
	test("ScrollY-      ", lc::MouseScroll::y_minus);
	test("MouseWheelY-  ", lc::MouseScroll::y_minus);
	test("MouseScrollY- ", lc::MouseScroll::y_minus);
	test("MSY-          ", lc::MouseScroll::y_minus);
	test("MWY-          ", lc::MouseScroll::y_minus);
	test("WheelX-       ", lc::MouseScroll::x_minus);
	test("ScrollX-      ", lc::MouseScroll::x_minus);
	test("MSX-          ", lc::MouseScroll::x_minus);
	test("MWX-          ", lc::MouseScroll::x_minus);

	// Joystick Analog
	test("JA1:x+             ", lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JS A1:X+           ", lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JS Analog1:x+      ", lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JSA 1 : x +        ", lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JSA1:x             ", lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x);
	test("JSA1:x+            ", lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JSA1:x-            ", lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus);
	test("Joystick A1:x+     ", lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JoystickAnalog1:x+ ", lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);

	test("J2A1:x+            ", lc::JoystickID{2}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JS 2 A 1 : x +     ", lc::JoystickID{2}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JS2 A1:X+          ", lc::JoystickID{2}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JS2 Analog1:x+     ", lc::JoystickID{2}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JS2A1:x            ", lc::JoystickID{2}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x);
	test("JS2A1:x+           ", lc::JoystickID{2}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("JS2A1:x-           ", lc::JoystickID{2}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus);
	test("Joystick2 A1:x+    ", lc::JoystickID{2}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);
	test("Joystick2Analog1:x+", lc::JoystickID{2}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus);

	// Joystick Button
	test("JB1                ", lc::JoystickID::any, lc::JoystickButton::Button1);
	test("JS B 1             ", lc::JoystickID::any, lc::JoystickButton::Button1);
	test("JS Button1         ", lc::JoystickID::any, lc::JoystickButton::Button1);
	test("JSB 1              ", lc::JoystickID::any, lc::JoystickButton::Button1);
	test("JSB1               ", lc::JoystickID::any, lc::JoystickButton::Button1);
	test("JSBtn1             ", lc::JoystickID::any, lc::JoystickButton::Button1);
	test("Joystick B1        ", lc::JoystickID::any, lc::JoystickButton::Button1);
	test("JoystickBtn1       ", lc::JoystickID::any, lc::JoystickButton::Button1);
	test("JoystickButton1    ", lc::JoystickID::any, lc::JoystickButton::Button1);

	test("J2B1               ", lc::JoystickID{2}, lc::JoystickButton::Button1);
	test("JS 2 B 1           ", lc::JoystickID{2}, lc::JoystickButton::Button1);
	test("JS 2 Button1       ", lc::JoystickID{2}, lc::JoystickButton::Button1);
	test("JS2B 1             ", lc::JoystickID{2}, lc::JoystickButton::Button1);
	test("JS2B1              ", lc::JoystickID{2}, lc::JoystickButton::Button1);
	test("JS2Btn1            ", lc::JoystickID{2}, lc::JoystickButton::Button1);
	test("Joystick2 B1       ", lc::JoystickID{2}, lc::JoystickButton::Button1);
	test("Joystick2Btn1      ", lc::JoystickID{2}, lc::JoystickButton::Button1);
	test("Joystick2Button1   ", lc::JoystickID{2}, lc::JoystickButton::Button1);

	// Gamepad Analog
	test("GA1:x+             ", lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GP A1:X+           ", lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GP Analog1:x+      ", lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GPA 1 : x +        ", lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GPA1:x             ", lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x);
	test("GPA1:x+            ", lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GPA1:x-            ", lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus);
	test("Gamepad A1:x+      ", lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GamepadAnalog1:x+  ", lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);

	test("G2A1:x+            ", lc::GamepadID{2}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GP 2 A 1 : x +     ", lc::GamepadID{2}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GP2 A1:X+          ", lc::GamepadID{2}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GP2 Analog1:x+     ", lc::GamepadID{2}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GP2A1:x            ", lc::GamepadID{2}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x);
	test("GP2A1:x+           ", lc::GamepadID{2}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("GP2A1:x-           ", lc::GamepadID{2}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus);
	test("Gamepad2 A1:x+     ", lc::GamepadID{2}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);
	test("Gamepad2Analog1:x+ ", lc::GamepadID{2}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus);

	// Gamepad Button
	test("GB0                ", lc::GamepadID::any, lc::GamepadButton::A);
	test("GB1                ", lc::GamepadID::any, lc::GamepadButton::B);
	test("GBA                ", lc::GamepadID::any, lc::GamepadButton::A);
	test("GBCross            ", lc::GamepadID::any, lc::GamepadButton::Cross);
	test("Gamepad Button 1   ", lc::GamepadID::any, lc::GamepadButton::B);
	test("Gamepad Button A   ", lc::GamepadID::any, lc::GamepadButton::A);

	test("GBB                ", lc::GamepadID::any, lc::GamepadButton::B);
	test("GP B B             ", lc::GamepadID::any, lc::GamepadButton::B);
	test("GP ButtonB         ", lc::GamepadID::any, lc::GamepadButton::B);
	test("GPB B              ", lc::GamepadID::any, lc::GamepadButton::B);
	test("GPBB               ", lc::GamepadID::any, lc::GamepadButton::B);
	test("GPBtnB             ", lc::GamepadID::any, lc::GamepadButton::B);
	test("Gamepad BB         ", lc::GamepadID::any, lc::GamepadButton::B);
	test("GamepadBtnB        ", lc::GamepadID::any, lc::GamepadButton::B);
	test("GamepadButtonB     ", lc::GamepadID::any, lc::GamepadButton::B);

	test("G2BB               ", lc::GamepadID{2}, lc::GamepadButton::B);
	test("GP 2 B B           ", lc::GamepadID{2}, lc::GamepadButton::B);
	test("GP 2 ButtonB       ", lc::GamepadID{2}, lc::GamepadButton::B);
	test("GP2B B             ", lc::GamepadID{2}, lc::GamepadButton::B);
	test("GP2BB              ", lc::GamepadID{2}, lc::GamepadButton::B);
	test("GP2BtnB            ", lc::GamepadID{2}, lc::GamepadButton::B);
	test("Gamepad2 BB        ", lc::GamepadID{2}, lc::GamepadButton::B);
	test("Gamepad2BtnB       ", lc::GamepadID{2}, lc::GamepadButton::B);
	test("Gamepad2ButtonB    ", lc::GamepadID{2}, lc::GamepadButton::B);

	test("Gamepad Button DP Up      ", lc::GamepadID::any, lc::GamepadButton::DPadUp);
	test("Gamepad Button DPUp       ", lc::GamepadID::any, lc::GamepadButton::DPadUp);
	test("Gamepad Button DPad Up    ", lc::GamepadID::any, lc::GamepadButton::DPadUp);
	test("Gamepad Button DPadUp     ", lc::GamepadID::any, lc::GamepadButton::DPadUp);

	test("GB BL                     ", lc::GamepadID::any, lc::GamepadButton::LeftBumper);
	test("GB LB                     ", lc::GamepadID::any, lc::GamepadButton::LeftBumper);
	test("GB TL                     ", lc::GamepadID::any, lc::GamepadButton::LeftThumb);
	test("GB LT                     ", lc::GamepadID::any, lc::GamepadButton::LeftThumb);
	test("Gamepad Button Bumper Left", lc::GamepadID::any, lc::GamepadButton::LeftBumper);
	test("Gamepad Button Left Bumper", lc::GamepadID::any, lc::GamepadButton::LeftBumper);
	test("Gamepad Button LT         ", lc::GamepadID::any, lc::GamepadButton::LeftThumb);
	test("Gamepad Button Left Thumb ", lc::GamepadID::any, lc::GamepadButton::LeftThumb);
	test("Gamepad Button TL         ", lc::GamepadID::any, lc::GamepadButton::LeftThumb);
	test("Gamepad Button Thumb Left ", lc::GamepadID::any, lc::GamepadButton::LeftThumb);

	// Invalid empty
	CHECK(not lc::parse_input_id_optional(""));
	CHECK(not lc::parse_input_id_optional(" "));

	// Invalid Modifiers
	CHECK(not lc::parse_input_id_optional("shift A"));
	CHECK(not lc::parse_input_id_optional("shift no"));
	CHECK(not lc::parse_input_id_optional("A shift"));

	// Invalid Scancode
	CHECK(not lc::parse_input_id_optional("sc"));
	CHECK(not lc::parse_input_id_optional("scA"));
	CHECK(not lc::parse_input_id_optional("scshift"));
	CHECK(not lc::parse_input_id_optional("scxx"));
	CHECK(not lc::parse_input_id_optional("scan"));

	// Invalid Keycode
	CHECK(not lc::parse_input_id_optional("á"));
	CHECK(not lc::parse_input_id_optional("Á"));
	CHECK(not lc::parse_input_id_optional("62"));
	CHECK(not lc::parse_input_id_optional("AA"));
	CHECK(not lc::parse_input_id_optional("\""));
	CHECK(not lc::parse_input_id_optional("kp"));

	// Invalid Codepoint
	CHECK(not lc::parse_input_id_optional(R"(")"));
	CHECK(not lc::parse_input_id_optional(R"("a ")"));
	CHECK(not lc::parse_input_id_optional(R"("mouse x-")"));
	CHECK(not lc::parse_input_id_optional(R"("sc120")"));
	CHECK(not lc::parse_input_id_optional(R"("shift")"));
	CHECK(not lc::parse_input_id_optional(R"("xyz")"));
	CHECK(not lc::parse_input_id_optional(R"(" á")"));
	CHECK(not lc::parse_input_id_optional(R"("á ")"));
	CHECK(not lc::parse_input_id_optional(R"("áá")"));
	CHECK(not lc::parse_input_id_optional(R"("ɫ"));
	CHECK(not lc::parse_input_id_optional(R"("ɫɫ")"));

	// Invalid Mouse Button
	CHECK(not lc::parse_input_id_optional("amb"));
	CHECK(not lc::parse_input_id_optional("mb a"));
	CHECK(not lc::parse_input_id_optional("mmbm"));
	CHECK(not lc::parse_input_id_optional("mmm"));
	CHECK(not lc::parse_input_id_optional("mb0 a"));

	// Invalid Mouse Movement
	CHECK(not lc::parse_input_id_optional("mx+x"));
	CHECK(not lc::parse_input_id_optional("mxx"));
	CHECK(not lc::parse_input_id_optional("mxx+"));

	// Invalid Mouse Scroll
	CHECK(not lc::parse_input_id_optional("S+"));
	CHECK(not lc::parse_input_id_optional("W+"));
	CHECK(not lc::parse_input_id_optional("MWW"));

	// Invalid Gamepad Analog
	CHECK(not lc::parse_input_id_optional("GP A1:X+x"));
	CHECK(not lc::parse_input_id_optional("GP1 Analog"));
	CHECK(not lc::parse_input_id_optional("GAnalog"));
	CHECK(not lc::parse_input_id_optional("GAnalog:"));
	CHECK(not lc::parse_input_id_optional("GAnalog1:"));
	CHECK(not lc::parse_input_id_optional("G2S A1:X+x"));
	CHECK(not lc::parse_input_id_optional("G2S1 Analog"));
	CHECK(not lc::parse_input_id_optional("G2Analog"));
	CHECK(not lc::parse_input_id_optional("G2Analog:"));
	CHECK(not lc::parse_input_id_optional("G2Analog1:"));

	// Invalid Gamepad Button
	CHECK(not lc::parse_input_id_optional("G1"));
	CHECK(not lc::parse_input_id_optional("G1C"));
	CHECK(not lc::parse_input_id_optional("G1C"));
	CHECK(not lc::parse_input_id_optional("G1C"));
	CHECK(not lc::parse_input_id_optional("GA"));
	CHECK(not lc::parse_input_id_optional("GB"));
	CHECK(not lc::parse_input_id_optional("GPA"));
	CHECK(not lc::parse_input_id_optional("GPC"));

	// Invalid Joystick Analog
	CHECK(not lc::parse_input_id_optional("JS A1:X+x"));
	CHECK(not lc::parse_input_id_optional("JS1 Analog"));
	CHECK(not lc::parse_input_id_optional("JAnalog"));
	CHECK(not lc::parse_input_id_optional("JAnalog:"));
	CHECK(not lc::parse_input_id_optional("JAnalog1:"));
	CHECK(not lc::parse_input_id_optional("J2S A1:X+x"));
	CHECK(not lc::parse_input_id_optional("J2S1 Analog"));
	CHECK(not lc::parse_input_id_optional("J2Analog"));
	CHECK(not lc::parse_input_id_optional("J2Analog:"));
	CHECK(not lc::parse_input_id_optional("J2Analog1:"));

	// Invalid Joystick Button
	CHECK(not lc::parse_input_id_optional("J1Btn"));
	CHECK(not lc::parse_input_id_optional("JB 1+"));
	CHECK(not lc::parse_input_id_optional("JB"));
	CHECK(not lc::parse_input_id_optional("JBtn"));
	CHECK(not lc::parse_input_id_optional("JS B 1+"));
	CHECK(not lc::parse_input_id_optional("JS B 1x"));
	CHECK(not lc::parse_input_id_optional("JS1"));
	CHECK(not lc::parse_input_id_optional("Joystick1"));
}

TEST_CASE("Parse inputID with multi word keycode", "[libv.ctrl.parse]") {
	CHECK(lc::parse_input_id_or_throw("NumLck") == lc::InputID{lc::Keycode::NumLock});
	CHECK(lc::parse_input_id_or_throw("NumLk") == lc::InputID{lc::Keycode::NumLock});
	CHECK(lc::parse_input_id_or_throw("NumLock") == lc::InputID{lc::Keycode::NumLock});
	CHECK(lc::parse_input_id_or_throw("num lck") == lc::InputID{lc::Keycode::NumLock});
	CHECK(lc::parse_input_id_or_throw("num lk") == lc::InputID{lc::Keycode::NumLock});
	CHECK(lc::parse_input_id_or_throw("num lock") == lc::InputID{lc::Keycode::NumLock});
	CHECK(lc::parse_input_id_or_throw("numlck") == lc::InputID{lc::Keycode::NumLock});
	CHECK(lc::parse_input_id_or_throw("numlk") == lc::InputID{lc::Keycode::NumLock});
	CHECK(lc::parse_input_id_or_throw("numlock") == lc::InputID{lc::Keycode::NumLock});

	CHECK(lc::parse_input_id_or_throw("Bracket Open") == lc::InputID{lc::Keycode::BracketOpen});
	CHECK(lc::parse_input_id_or_throw("BracketOpen") == lc::InputID{lc::Keycode::BracketOpen});
	CHECK(lc::parse_input_id_or_throw("bracketopen") == lc::InputID{lc::Keycode::BracketOpen});
	CHECK(lc::parse_input_id_or_throw("Open Bracket") == lc::InputID{lc::Keycode::BracketOpen});
	CHECK(lc::parse_input_id_or_throw("OpenBracket") == lc::InputID{lc::Keycode::BracketOpen});
	CHECK(lc::parse_input_id_or_throw("openbracket") == lc::InputID{lc::Keycode::BracketOpen});

	CHECK(lc::parse_input_id_or_throw("alt gr") == lc::InputID{lc::Keycode::AltRight});
	CHECK(lc::parse_input_id_or_throw("altgr") == lc::InputID{lc::Keycode::AltRight});
}

TEST_CASE("Parse inputID with num keycode", "[libv.ctrl.parse]") {
	CHECK(lc::parse_input_id_or_throw("0") == lc::InputID{lc::Keycode::Num0});
	CHECK(lc::parse_input_id_or_throw("1") == lc::InputID{lc::Keycode::Num1});
	CHECK(lc::parse_input_id_or_throw("num0") == lc::InputID{lc::Keycode::Num0});
	CHECK(lc::parse_input_id_or_throw("num 1") == lc::InputID{lc::Keycode::Num1});
	CHECK(lc::parse_input_id_or_throw("NUM 0") == lc::InputID{lc::Keycode::Num0});
	CHECK(lc::parse_input_id_or_throw("Num 1") == lc::InputID{lc::Keycode::Num1});
}

TEST_CASE("Parse inputID with kp keycode", "[libv.ctrl.parse]") {
	CHECK(lc::parse_input_id_or_throw("KP minus") == lc::InputID{lc::Keycode::KPMinus});
	CHECK(lc::parse_input_id_or_throw("KPminus") == lc::InputID{lc::Keycode::KPMinus});
	CHECK(lc::parse_input_id_or_throw("Kp -") == lc::InputID{lc::Keycode::KPMinus});
	CHECK(lc::parse_input_id_or_throw("Kp minus") == lc::InputID{lc::Keycode::KPMinus});
	CHECK(lc::parse_input_id_or_throw("kp -") == lc::InputID{lc::Keycode::KPMinus});
	CHECK(lc::parse_input_id_or_throw("kp minus") == lc::InputID{lc::Keycode::KPMinus});
	CHECK(lc::parse_input_id_or_throw("kpminus") == lc::InputID{lc::Keycode::KPMinus});

	CHECK(lc::parse_input_id_or_throw("kp.") == lc::InputID{lc::Keycode::KPDot});
	CHECK(lc::parse_input_id_or_throw("kp-") == lc::InputID{lc::Keycode::KPMinus});

	CHECK(lc::parse_input_id_or_throw("kp 0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("kp num 0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("kp num0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("kp0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("kpnum 0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("kpnum0") == lc::InputID{lc::Keycode::KPNum0});

	CHECK(lc::parse_input_id_or_throw("keypad 0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("keypad num 0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("keypad num0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("keypad0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("keypadnum 0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("keypadnum0") == lc::InputID{lc::Keycode::KPNum0});

	CHECK(lc::parse_input_id_or_throw("numpad 0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("numpad num 0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("numpad num0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("numpad0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("numpadnum 0") == lc::InputID{lc::Keycode::KPNum0});
	CHECK(lc::parse_input_id_or_throw("numpadnum0") == lc::InputID{lc::Keycode::KPNum0});
}

TEST_CASE("Parse inputID with pg keycode", "[libv.ctrl.parse]") {
	CHECK(lc::parse_input_id_or_throw("PAGE UP") == lc::InputID{lc::Keycode::PageUp});
	CHECK(lc::parse_input_id_or_throw("PAGEUP") == lc::InputID{lc::Keycode::PageUp});
	CHECK(lc::parse_input_id_or_throw("PG Up") == lc::InputID{lc::Keycode::PageUp});
	CHECK(lc::parse_input_id_or_throw("PGUp") == lc::InputID{lc::Keycode::PageUp});
	CHECK(lc::parse_input_id_or_throw("page up") == lc::InputID{lc::Keycode::PageUp});
	CHECK(lc::parse_input_id_or_throw("pageup") == lc::InputID{lc::Keycode::PageUp});
	CHECK(lc::parse_input_id_or_throw("pg up") == lc::InputID{lc::Keycode::PageUp});
	CHECK(lc::parse_input_id_or_throw("pgup") == lc::InputID{lc::Keycode::PageUp});
}

TEST_CASE("Parse inputID with sided keycode", "[libv.ctrl.parse]") {
	CHECK(lc::parse_input_id_or_throw("LCTRL") == lc::InputID{lc::Keycode::ControlLeft});
	CHECK(lc::parse_input_id_or_throw("l ctrl") == lc::InputID{lc::Keycode::ControlLeft});
	CHECK(lc::parse_input_id_or_throw("lctrl") == lc::InputID{lc::Keycode::ControlLeft});
	CHECK(lc::parse_input_id_or_throw("left ctrl") == lc::InputID{lc::Keycode::ControlLeft});

	CHECK(lc::parse_input_id_or_throw("CTRLL") == lc::InputID{lc::Keycode::ControlLeft});
	CHECK(lc::parse_input_id_or_throw("ctrl l") == lc::InputID{lc::Keycode::ControlLeft});
	CHECK(lc::parse_input_id_or_throw("ctrl left") == lc::InputID{lc::Keycode::ControlLeft});
	CHECK(lc::parse_input_id_or_throw("ctrll") == lc::InputID{lc::Keycode::ControlLeft});

	CHECK(lc::parse_input_id_or_throw("RSHIFT") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("Rshift") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("r shift") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("rSHIFT") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("right shift") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("rshift") == lc::InputID{lc::Keycode::ShiftRight});

	CHECK(lc::parse_input_id_or_throw("SHIFTR") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("SHIFTr") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("shift r") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("shift right") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("shiftR") == lc::InputID{lc::Keycode::ShiftRight});
	CHECK(lc::parse_input_id_or_throw("shiftr") == lc::InputID{lc::Keycode::ShiftRight});
}

TEST_CASE("Parse inputID with named keycode", "[libv.ctrl.parse]") {
	CHECK(lc::parse_input_id_or_throw("A") == lc::InputID{lc::Keycode::A});
	CHECK(lc::parse_input_id_or_throw("a") == lc::InputID{lc::Keycode::A});

	CHECK(lc::parse_input_id_or_throw("Apostrophe") == lc::InputID{lc::Keycode::Apostrophe});
	CHECK(lc::parse_input_id_or_throw("'") == lc::InputID{lc::Keycode::Apostrophe});

	CHECK(lc::parse_input_id_or_throw("\\") == lc::InputID{lc::Keycode::Backslash});
	CHECK(lc::parse_input_id_or_throw("backslash") == lc::InputID{lc::Keycode::Backslash});

	CHECK(lc::parse_input_id_or_throw("/") == lc::InputID{lc::Keycode::Slash});
	CHECK(lc::parse_input_id_or_throw("SLASH") == lc::InputID{lc::Keycode::Slash});

	CHECK(lc::parse_input_id_or_throw("Menu") == lc::InputID{lc::Keycode::Menu});
}

// =================================================================================================

TEST_CASE("Parse Input general", "[libv.ctrl.parse]") {
	const auto test = [](std::string_view input, auto... args) {
		const auto result = lc::parse_input_or_throw(input);
		const auto expected = lc::Input{args...};

		CHECK(result == expected);
		// Second pass consistency check, verify round trips
		CHECK(lc::parse_input_or_throw(result.to_string_name()) == result);
		CHECK(lc::parse_input_or_throw(result.to_string_symbol()) == result);
	};

	test("shift           ", lc::Modifier::shift, lc::DigitalInputAction::auto_);
	test("shift [auto]    ", lc::Modifier::shift, lc::DigitalInputAction::auto_);
	test("shift [press]   ", lc::Modifier::shift, lc::DigitalInputAction::press);
	test("shift [repeat]  ", lc::Modifier::shift, lc::DigitalInputAction::repeat);
	test("shift [release] ", lc::Modifier::shift, lc::DigitalInputAction::release);
	test("shift [any]     ", lc::Modifier::shift, lc::DigitalInputAction::any);
	test("shift [hold]    ", lc::Modifier::shift, lc::DigitalInputAction::hold);
	test("shift [free]    ", lc::Modifier::shift, lc::DigitalInputAction::free);

	test("sc150           ", lc::Scancode{150});
	test("sc150 [press]   ", lc::Scancode{150}, lc::DigitalInputAction::press);
	test("X               ", lc::Keycode::X);
	test("X [press]       ", lc::Keycode::X, lc::DigitalInputAction::press);
	test("[               ", lc::Keycode::BracketOpen);
	test("[ [press]       ", lc::Keycode::BracketOpen, lc::DigitalInputAction::press);
	test("[[press]        ", lc::Keycode::BracketOpen, lc::DigitalInputAction::press);
	test("]               ", lc::Keycode::BracketClose);
	test("] [press]       ", lc::Keycode::BracketClose, lc::DigitalInputAction::press);
	test("][press]        ", lc::Keycode::BracketClose, lc::DigitalInputAction::press);
	test(",               ", lc::Keycode::Comma);
	test(", [press]       ", lc::Keycode::Comma, lc::DigitalInputAction::press);
	test(R"("á"          )", utf8_codepoint("á"));
	test(R"("á" [press]  )", utf8_codepoint("á"), lc::DigitalInputAction::press);
	test("LMB             ", lc::MouseButton::Left);
	test("LMB [press]     ", lc::MouseButton::Left, lc::DigitalInputAction::press);
	test("Mouse X+        ", lc::MouseMovement::x_plus);

	test("JS B1           ", lc::JoystickID::any, lc::JoystickButton::Button1);
	test("JS B1 [press]   ", lc::JoystickID::any, lc::JoystickButton::Button1, lc::DigitalInputAction::press);
	test("JS2 B1          ", lc::JoystickID{2}, lc::JoystickButton::Button1);
	test("JS2 B1 [press]  ", lc::JoystickID{2}, lc::JoystickButton::Button1, lc::DigitalInputAction::press);
	test("GP BX           ", lc::GamepadID::any, lc::GamepadButton::X);
	test("GP BX [press]   ", lc::GamepadID::any, lc::GamepadButton::X, lc::DigitalInputAction::press);
	test("GP2 BX          ", lc::GamepadID{2}, lc::GamepadButton::X);
	test("GP2 BX [press]  ", lc::GamepadID{2}, lc::GamepadButton::X, lc::DigitalInputAction::press);

	CHECK(not lc::parse_input_optional(""));
	CHECK(not lc::parse_input_optional(" "));

	CHECK(not lc::parse_input_optional("[press]"));

	CHECK(not lc::parse_input_optional("shift [press"));
	CHECK(not lc::parse_input_optional("shift press]"));

	CHECK(not lc::parse_input_optional("á [press]"));
	CHECK(not lc::parse_input_optional(" á [press]"));
	CHECK(not lc::parse_input_optional("á  [press]"));
	CHECK(not lc::parse_input_optional("ɫ [press]"));
	CHECK(not lc::parse_input_optional("ɫɫ [press]"));

	CHECK(not lc::parse_input_optional("MX+ [press]"));
	CHECK(not lc::parse_input_optional("GP A1:X+ [press]"));
	CHECK(not lc::parse_input_optional("GP2 A1:X+ [press]"));
	CHECK(not lc::parse_input_optional("JS A1:X+ [press]"));
	CHECK(not lc::parse_input_optional("JS2 A1:X+ [press]"));
}

TEST_CASE("Parse Combination general", "[libv.ctrl.parse]") {
	const auto test = [](std::string_view input, auto... args) {
		const auto result = lc::parse_combination_or_throw(input);
		const auto expected = lc::Combination{args...};

		CHECK(result == expected);
		// Second pass consistency check, verify round trips
		CHECK(lc::parse_combination_or_throw(result.to_string_name()) == result);
		CHECK(lc::parse_combination_or_throw(result.to_string_symbol()) == result);
	};

	test("A                    ", lc::Input{lc::Keycode::A});
	test("Ctrl                 ", lc::Input{lc::Modifier::control});
	test("A+Z                  ", lc::Input{lc::Keycode::A}, lc::Input{lc::Keycode::Z});
	test("A+Z+B                ", lc::Input{lc::Keycode::A}, lc::Input{lc::Keycode::Z}, lc::Input{lc::Keycode::B});
	test("Ctrl+Z               ", lc::Input{lc::Modifier::control}, lc::Input{lc::Keycode::Z});
	test("Ctrl + Z             ", lc::Input{lc::Modifier::control}, lc::Input{lc::Keycode::Z});
	test("Ctrl + \"Z\"         ", lc::Input{lc::Modifier::control}, lc::Input{utf8_codepoint("Z")});
	test("A+'                  ", lc::Input{lc::Keycode::A}, lc::Input{lc::Keycode::Apostrophe});
	test("A + '                ", lc::Input{lc::Keycode::A}, lc::Input{lc::Keycode::Apostrophe});
	test("A + \"'\"            ", lc::Input{lc::Keycode::A}, lc::Input{utf8_codepoint("'")});
	test("'+'                  ", lc::Input{utf8_codepoint("+")});
	test("'+ '                 ", lc::Input{lc::Keycode::Apostrophe}, lc::Input{lc::Keycode::Apostrophe});
	test("'+'+'                ", lc::Input{utf8_codepoint("+")}, lc::Input{lc::Keycode::Apostrophe});
	test("'+'+'+'              ", lc::Input{utf8_codepoint("+")}, lc::Input{utf8_codepoint("+")});
	test("'+'+'+'+'            ", lc::Input{utf8_codepoint("+")}, lc::Input{utf8_codepoint("+")}, lc::Input{lc::Keycode::Apostrophe});
	test("'+'+'+'+'+'          ", lc::Input{utf8_codepoint("+")}, lc::Input{utf8_codepoint("+")}, lc::Input{utf8_codepoint("+")});
	test("\"+\"+\"+\"          ", lc::Input{utf8_codepoint("+")}, lc::Input{utf8_codepoint("+")});
	test("'+'+'+A              ", lc::Input{utf8_codepoint("+")}, lc::Input{lc::Keycode::Apostrophe}, lc::Input{lc::Keycode::A});
	test("'+'+Apostrophe       ", lc::Input{utf8_codepoint("+")}, lc::Input{lc::Keycode::Apostrophe});
	test("Apostrophe+'+'       ", lc::Input{lc::Keycode::Apostrophe}, lc::Input{utf8_codepoint("+")});
	test("'+'+'                ", lc::Input{utf8_codepoint("+")}, lc::Input{lc::Keycode::Apostrophe});
	test("'+'+ '               ", lc::Input{utf8_codepoint("+")}, lc::Input{lc::Keycode::Apostrophe});
	test("'+' + '+'            ", lc::Input{utf8_codepoint("+")}, lc::Input{utf8_codepoint("+")});
	test("A+'+'                ", lc::Input{lc::Keycode::A}, lc::Input{utf8_codepoint("+")});
	test("A+'+ '               ", lc::Input{lc::Keycode::A}, lc::Input{lc::Keycode::Apostrophe}, lc::Input{lc::Keycode::Apostrophe});
	test("A+\"+\"              ", lc::Input{lc::Keycode::A}, lc::Input{utf8_codepoint("+")});
	test("MX+'                 ", lc::Input{lc::MouseMovement::x}, lc::Input{lc::Keycode::Apostrophe});
	test("MX+'+'               ", lc::Input{lc::MouseMovement::x}, lc::Input{utf8_codepoint("+")});
	test("MX+\"+\"             ", lc::Input{lc::MouseMovement::x}, lc::Input{utf8_codepoint("+")});
	test("MX++'                ", lc::Input{lc::MouseMovement::x_plus}, lc::Input{lc::Keycode::Apostrophe});
	test("MX+A                 ", lc::Input{lc::MouseMovement::x}, lc::Input{lc::Keycode::A});
	test("Kp+                  ", lc::Input{lc::Keycode::KPPlus});
	test("Kp++A                ", lc::Input{lc::Keycode::KPPlus}, lc::Input{lc::Keycode::A});
	test("A+MX                 ", lc::Input{lc::Keycode::A}, lc::Input{lc::MouseMovement::x});
	test("A+MX+                ", lc::Input{lc::Keycode::A}, lc::Input{lc::MouseMovement::x_plus});
	test("MX++A                ", lc::Input{lc::MouseMovement::x_plus}, lc::Input{lc::Keycode::A});
	test("MX++MX-              ", lc::Input{lc::MouseMovement::x_plus}, lc::Input{lc::MouseMovement::x_minus});
	test("Ctrl + \"+\" [repeat]", lc::Input{lc::Modifier::control}, lc::Input{utf8_codepoint("+"), lc::DigitalInputAction::repeat});
	test("Ctrl + \"Z\" [repeat]", lc::Input{lc::Modifier::control}, lc::Input{utf8_codepoint("Z"), lc::DigitalInputAction::repeat});
	test("Ctrl [hold] + \"+\" [repeat]", lc::Input{lc::Modifier::control, lc::DigitalInputAction::hold}, lc::Input{utf8_codepoint("+"), lc::DigitalInputAction::repeat});
	test("Ctrl [hold] + \"Z\" [repeat]", lc::Input{lc::Modifier::control, lc::DigitalInputAction::hold}, lc::Input{utf8_codepoint("Z"), lc::DigitalInputAction::repeat});

	// Space test
	test("MX", lc::Input{lc::MouseMovement::x});
	test("MX+", lc::Input{lc::MouseMovement::x_plus});
	test("MX+ ", lc::Input{lc::MouseMovement::x_plus});
	test(" MX+", lc::Input{lc::MouseMovement::x_plus});
	test(" MX+ ", lc::Input{lc::MouseMovement::x_plus});

	CHECK(not lc::parse_combination_optional(""));
	CHECK(not lc::parse_combination_optional(" "));

	CHECK(not lc::parse_combination_optional("KP++"));
	CHECK(not lc::parse_combination_optional("KP+ +"));
	CHECK(not lc::parse_combination_optional("KP+++"));
	CHECK(not lc::parse_combination_optional("KP+++A"));
	CHECK(not lc::parse_combination_optional("MX++"));
	CHECK(not lc::parse_combination_optional("MX+'+"));
	CHECK(not lc::parse_combination_optional("MX+\"+"));
	CHECK(not lc::parse_combination_optional("MX++\""));

	CHECK(not lc::parse_combination_optional("\"+\"+\""));
	CHECK(not lc::parse_combination_optional("++'+'"));
	CHECK(not lc::parse_combination_optional("'+'++"));
}

TEST_CASE("Parse Sequence general", "[libv.ctrl.parse]") {
	const auto test = [](std::string_view input, auto... args) {
		const auto result = lc::parse_sequence_or_throw(input);
		const auto expected = lc::Sequence{args...};

		CHECK(result == expected);
		// Second pass consistency check, verify round trips
		CHECK(lc::parse_sequence_or_throw(result.to_string_name()) == result);
		CHECK(lc::parse_sequence_or_throw(result.to_string_symbol()) == result);
	};

	test("Ctrl                 ", lc::Combination{lc::Input{lc::Modifier::control}});
	test("A+Z+B                ", lc::Combination{lc::Input{lc::Keycode::A}, lc::Input{lc::Keycode::Z}, lc::Input{lc::Keycode::B}});
	test("Ctrl + Z             ", lc::Combination{lc::Input{lc::Modifier::control}, lc::Input{lc::Keycode::Z}});
	test("A + \"'\"            ", lc::Combination{lc::Input{lc::Keycode::A}, lc::Input{utf8_codepoint("'")}});
	test("','                  ", lc::Combination{lc::Input{utf8_codepoint(",")}});
	test("', '                 ", lc::Combination{lc::Input{lc::Keycode::Apostrophe}}, lc::Combination{lc::Input{lc::Keycode::Apostrophe}});
	test("Ctrl + U, U          ", lc::Combination{lc::Input{lc::Modifier::control}, lc::Input{lc::Keycode::U}}, lc::Combination{lc::Input{lc::Keycode::U}});
	test("Ctrl + U, ','        ", lc::Combination{lc::Input{lc::Modifier::control}, lc::Input{lc::Keycode::U}}, lc::Combination{lc::Input{utf8_codepoint(",")}});
	test("',','                ", lc::Combination{lc::Input{utf8_codepoint(",")}}, lc::Combination{lc::Input{lc::Keycode::Apostrophe}});
	test("',',','              ", lc::Combination{lc::Input{utf8_codepoint(",")}}, lc::Combination{lc::Input{utf8_codepoint(",")}});
	test("',', ','             ", lc::Combination{lc::Input{utf8_codepoint(",")}}, lc::Combination{lc::Input{utf8_codepoint(",")}});
	test("Ctrl + U, ,          ", lc::Combination{lc::Input{lc::Modifier::control}, lc::Input{lc::Keycode::U}}, lc::Combination{lc::Input{lc::Keycode::Comma}});
	test(",,,                  ", lc::Combination{lc::Input{lc::Keycode::Comma}}, lc::Combination{lc::Input{lc::Keycode::Comma}});
	test("Ctrl + ,, ,          ", lc::Combination{lc::Input{lc::Modifier::control}, lc::Input{lc::Keycode::Comma}}, lc::Combination{lc::Input{lc::Keycode::Comma}});
	test("Ctrl + ,,,           ", lc::Combination{lc::Input{lc::Modifier::control}, lc::Input{lc::Keycode::Comma}}, lc::Combination{lc::Input{lc::Keycode::Comma}});

	CHECK(not lc::parse_sequence_optional(""));
	CHECK(not lc::parse_sequence_optional(" "));

	CHECK(not lc::parse_sequence_optional("a,,,"));
	CHECK(not lc::parse_sequence_optional(",,"));
	CHECK(not lc::parse_sequence_optional(",',',"));
	CHECK(not lc::parse_sequence_optional(",, "));
	CHECK(not lc::parse_sequence_optional(",,,,"));
	CHECK(not lc::parse_sequence_optional("Ctrl + ,',',"));
}

TEST_CASE("InputID test to string round trip", "[libv.ctrl.parse]") {
	const auto test = [](auto... args) {
		const auto expected = lc::InputID{args...};

		CHECK(lc::parse_input_id_or_throw(expected.to_string_name()) == expected);
//		CHECK(lc::parse_input_id_or_throw(expected.to_string_identifier()) == expected);
		CHECK(lc::parse_input_id_or_throw(expected.to_string_symbol()) == expected);
	};

	test(lc::Keycode::Unknown      );

	test(lc::Keycode::Space        );
	test(lc::Keycode::Apostrophe   );
	test(lc::Keycode::Comma        );
	test(lc::Keycode::Minus        );
	test(lc::Keycode::Period       );
	test(lc::Keycode::Slash        );
	test(lc::Keycode::Num0         );
	test(lc::Keycode::Num1         );
	test(lc::Keycode::Num2         );
	test(lc::Keycode::Num3         );
	test(lc::Keycode::Num4         );
	test(lc::Keycode::Num5         );
	test(lc::Keycode::Num6         );
	test(lc::Keycode::Num7         );
	test(lc::Keycode::Num8         );
	test(lc::Keycode::Num9         );
	test(lc::Keycode::Semicolon    );
	test(lc::Keycode::Equals       );
	test(lc::Keycode::A            );
	test(lc::Keycode::B            );
	test(lc::Keycode::C            );
	test(lc::Keycode::D            );
	test(lc::Keycode::E            );
	test(lc::Keycode::F            );
	test(lc::Keycode::G            );
	test(lc::Keycode::H            );
	test(lc::Keycode::I            );
	test(lc::Keycode::J            );
	test(lc::Keycode::K            );
	test(lc::Keycode::L            );
	test(lc::Keycode::M            );
	test(lc::Keycode::N            );
	test(lc::Keycode::O            );
	test(lc::Keycode::P            );
	test(lc::Keycode::Q            );
	test(lc::Keycode::R            );
	test(lc::Keycode::S            );
	test(lc::Keycode::T            );
	test(lc::Keycode::U            );
	test(lc::Keycode::V            );
	test(lc::Keycode::W            );
	test(lc::Keycode::X            );
	test(lc::Keycode::Y            );
	test(lc::Keycode::Z            );
	test(lc::Keycode::BracketOpen  );
	test(lc::Keycode::Backslash    );
	test(lc::Keycode::BracketClose );
	test(lc::Keycode::Backtick     );
	test(lc::Keycode::World1       );
	test(lc::Keycode::World2       );
	test(lc::Keycode::Escape       );
	test(lc::Keycode::Enter        );
	test(lc::Keycode::Tab          );
	test(lc::Keycode::Backspace    );
	test(lc::Keycode::Insert       );
	test(lc::Keycode::Delete       );
	test(lc::Keycode::Right        );
	test(lc::Keycode::Left         );
	test(lc::Keycode::Down         );
	test(lc::Keycode::Up           );
	test(lc::Keycode::PageUp       );
	test(lc::Keycode::PageDown     );
	test(lc::Keycode::Home         );
	test(lc::Keycode::End          );
	test(lc::Keycode::CapsLock     );
	test(lc::Keycode::ScrollLock   );
	test(lc::Keycode::NumLock      );
	test(lc::Keycode::PrintScreen  );
	test(lc::Keycode::Pause        );
	test(lc::Keycode::F1           );
	test(lc::Keycode::F2           );
	test(lc::Keycode::F3           );
	test(lc::Keycode::F4           );
	test(lc::Keycode::F5           );
	test(lc::Keycode::F6           );
	test(lc::Keycode::F7           );
	test(lc::Keycode::F8           );
	test(lc::Keycode::F9           );
	test(lc::Keycode::F10          );
	test(lc::Keycode::F11          );
	test(lc::Keycode::F12          );
	test(lc::Keycode::F13          );
	test(lc::Keycode::F14          );
	test(lc::Keycode::F15          );
	test(lc::Keycode::F16          );
	test(lc::Keycode::F17          );
	test(lc::Keycode::F18          );
	test(lc::Keycode::F19          );
	test(lc::Keycode::F20          );
	test(lc::Keycode::F21          );
	test(lc::Keycode::F22          );
	test(lc::Keycode::F23          );
	test(lc::Keycode::F24          );
	test(lc::Keycode::F25          );
	test(lc::Keycode::KPNum0       );
	test(lc::Keycode::KPNum1       );
	test(lc::Keycode::KPNum2       );
	test(lc::Keycode::KPNum3       );
	test(lc::Keycode::KPNum4       );
	test(lc::Keycode::KPNum5       );
	test(lc::Keycode::KPNum6       );
	test(lc::Keycode::KPNum7       );
	test(lc::Keycode::KPNum8       );
	test(lc::Keycode::KPNum9       );
	test(lc::Keycode::KPDot        );
	test(lc::Keycode::KPSlash      );
	test(lc::Keycode::KPAsterisk   );
	test(lc::Keycode::KPMinus      );
	test(lc::Keycode::KPPlus       );
	test(lc::Keycode::KPEnter      );
	test(lc::Keycode::KPEqual      );
	test(lc::Keycode::ShiftLeft    );
	test(lc::Keycode::ControlLeft  );
	test(lc::Keycode::AltLeft      );
	test(lc::Keycode::SuperLeft    );
	test(lc::Keycode::ShiftRight   );
	test(lc::Keycode::ControlRight );
	test(lc::Keycode::AltRight     );
	test(lc::Keycode::SuperRight   );
	test(lc::Keycode::Menu         );

	test(lc::Modifier::shift       );
	test(lc::Modifier::control     );
	test(lc::Modifier::alt         );
	test(lc::Modifier::super       );

	test(lc::MouseButton::Left     );
	test(lc::MouseButton::Right    );
	test(lc::MouseButton::Middle   );

	test(lc::MouseButton::Button0  );
	test(lc::MouseButton::Button1  );
	test(lc::MouseButton::Button2  );
	test(lc::MouseButton::Button3  );
	test(lc::MouseButton::Button4  );
	test(lc::MouseButton::Button5  );
	test(lc::MouseButton::Button6  );
	test(lc::MouseButton::Button7  );
	test(lc::MouseButton::Button8  );
	test(lc::MouseButton::Button9  );
	test(lc::MouseButton::Button10 );
	test(lc::MouseButton::Button11 );
	test(lc::MouseButton::Button12 );
	test(lc::MouseButton::Button13 );
	test(lc::MouseButton::Button14 );
	test(lc::MouseButton::Button15 );
	test(lc::MouseButton::Button16 );
	test(lc::MouseButton::Button17 );
	test(lc::MouseButton::Button18 );
	test(lc::MouseButton::Button19 );
	test(lc::MouseButton::Button20 );
	test(lc::MouseButton::Button21 );
	test(lc::MouseButton::Button22 );
	test(lc::MouseButton::Button23 );
	test(lc::MouseButton::Button24 );
	test(lc::MouseButton::Button25 );
	test(lc::MouseButton::Button26 );
	test(lc::MouseButton::Button27 );
	test(lc::MouseButton::Button28 );
	test(lc::MouseButton::Button29 );
	test(lc::MouseButton::Button30 );
	test(lc::MouseButton::Button31 );
	test(lc::MouseButton::Button32 );
	test(lc::MouseButton::Button33 );
	test(lc::MouseButton::Button34 );
	test(lc::MouseButton::Button35 );
	test(lc::MouseButton::Button36 );
	test(lc::MouseButton::Button37 );
	test(lc::MouseButton::Button38 );
	test(lc::MouseButton::Button39 );
	test(lc::MouseButton::Button40 );
	test(lc::MouseButton::Button41 );
	test(lc::MouseButton::Button42 );
	test(lc::MouseButton::Button43 );
	test(lc::MouseButton::Button44 );
	test(lc::MouseButton::Button45 );
	test(lc::MouseButton::Button46 );
	test(lc::MouseButton::Button47 );
	test(lc::MouseButton::Button48 );
	test(lc::MouseButton::Button49 );
	test(lc::MouseButton::Button50 );
	test(lc::MouseButton::Button51 );
	test(lc::MouseButton::Button52 );
	test(lc::MouseButton::Button53 );
	test(lc::MouseButton::Button54 );
	test(lc::MouseButton::Button55 );
	test(lc::MouseButton::Button56 );
	test(lc::MouseButton::Button57 );
	test(lc::MouseButton::Button58 );
	test(lc::MouseButton::Button59 );
	test(lc::MouseButton::Button60 );
	test(lc::MouseButton::Button61 );
	test(lc::MouseButton::Button62 );
	test(lc::MouseButton::Button63 );

	test(lc::MouseMovement::x      );
	test(lc::MouseMovement::x_plus );
	test(lc::MouseMovement::x_minus);
	test(lc::MouseMovement::y      );
	test(lc::MouseMovement::y_plus );
	test(lc::MouseMovement::y_minus);

	test(lc::MouseScroll::x        );
	test(lc::MouseScroll::x_plus   );
	test(lc::MouseScroll::x_minus  );
	test(lc::MouseScroll::y        );
	test(lc::MouseScroll::y_plus   );
	test(lc::MouseScroll::y_minus  );

	test(lc::GamepadID::any, lc::GamepadButton::A          );
	test(lc::GamepadID::any, lc::GamepadButton::B          );
	test(lc::GamepadID::any, lc::GamepadButton::X          );
	test(lc::GamepadID::any, lc::GamepadButton::Y          );
	test(lc::GamepadID::any, lc::GamepadButton::LeftBumper );
	test(lc::GamepadID::any, lc::GamepadButton::RightBumper);
	test(lc::GamepadID::any, lc::GamepadButton::Back       );
	test(lc::GamepadID::any, lc::GamepadButton::Start      );
	test(lc::GamepadID::any, lc::GamepadButton::Guide      );
	test(lc::GamepadID::any, lc::GamepadButton::LeftThumb  );
	test(lc::GamepadID::any, lc::GamepadButton::RightThumb );
	test(lc::GamepadID::any, lc::GamepadButton::DPadUp     );
	test(lc::GamepadID::any, lc::GamepadButton::DPadRight  );
	test(lc::GamepadID::any, lc::GamepadButton::DPadDown   );
	test(lc::GamepadID::any, lc::GamepadButton::DPadLeft   );

	test(lc::GamepadID{10}, lc::GamepadButton::A          );
	test(lc::GamepadID{10}, lc::GamepadButton::B          );
	test(lc::GamepadID{10}, lc::GamepadButton::X          );
	test(lc::GamepadID{10}, lc::GamepadButton::Y          );
	test(lc::GamepadID{10}, lc::GamepadButton::LeftBumper );
	test(lc::GamepadID{10}, lc::GamepadButton::RightBumper);
	test(lc::GamepadID{10}, lc::GamepadButton::Back       );
	test(lc::GamepadID{10}, lc::GamepadButton::Start      );
	test(lc::GamepadID{10}, lc::GamepadButton::Guide      );
	test(lc::GamepadID{10}, lc::GamepadButton::LeftThumb  );
	test(lc::GamepadID{10}, lc::GamepadButton::RightThumb );
	test(lc::GamepadID{10}, lc::GamepadButton::DPadUp     );
	test(lc::GamepadID{10}, lc::GamepadButton::DPadRight  );
	test(lc::GamepadID{10}, lc::GamepadButton::DPadDown   );
	test(lc::GamepadID{10}, lc::GamepadButton::DPadLeft   );

	test(lc::GamepadID::any, lc::GamepadAnalogID{11}, lc::AnalogDimension::x      );
	test(lc::GamepadID::any, lc::GamepadAnalogID{11}, lc::AnalogDimension::x_plus );
	test(lc::GamepadID::any, lc::GamepadAnalogID{11}, lc::AnalogDimension::x_minus);
	test(lc::GamepadID::any, lc::GamepadAnalogID{11}, lc::AnalogDimension::y      );
	test(lc::GamepadID::any, lc::GamepadAnalogID{11}, lc::AnalogDimension::y_plus );
	test(lc::GamepadID::any, lc::GamepadAnalogID{11}, lc::AnalogDimension::y_minus);

	test(lc::GamepadID{10}, lc::GamepadAnalogID{11}, lc::AnalogDimension::x      );
	test(lc::GamepadID{10}, lc::GamepadAnalogID{11}, lc::AnalogDimension::x_plus );
	test(lc::GamepadID{10}, lc::GamepadAnalogID{11}, lc::AnalogDimension::x_minus);
	test(lc::GamepadID{10}, lc::GamepadAnalogID{11}, lc::AnalogDimension::y      );
	test(lc::GamepadID{10}, lc::GamepadAnalogID{11}, lc::AnalogDimension::y_plus );
	test(lc::GamepadID{10}, lc::GamepadAnalogID{11}, lc::AnalogDimension::y_minus);

	test(lc::JoystickID::any, lc::JoystickButton::Button0  );
	test(lc::JoystickID::any, lc::JoystickButton::Button1  );
	test(lc::JoystickID::any, lc::JoystickButton::Button2  );
	test(lc::JoystickID::any, lc::JoystickButton::Button3  );
	test(lc::JoystickID::any, lc::JoystickButton::Button4  );
	test(lc::JoystickID::any, lc::JoystickButton::Button5  );
	test(lc::JoystickID::any, lc::JoystickButton::Button6  );
	test(lc::JoystickID::any, lc::JoystickButton::Button7  );
	test(lc::JoystickID::any, lc::JoystickButton::Button8  );
	test(lc::JoystickID::any, lc::JoystickButton::Button9  );
	test(lc::JoystickID::any, lc::JoystickButton::Button10 );
	test(lc::JoystickID::any, lc::JoystickButton::Button11 );
	test(lc::JoystickID::any, lc::JoystickButton::Button12 );
	test(lc::JoystickID::any, lc::JoystickButton::Button13 );
	test(lc::JoystickID::any, lc::JoystickButton::Button14 );
	test(lc::JoystickID::any, lc::JoystickButton::Button15 );

	test(lc::JoystickID{10}, lc::JoystickButton::Button0  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button1  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button2  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button3  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button4  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button5  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button6  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button7  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button8  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button9  );
	test(lc::JoystickID{10}, lc::JoystickButton::Button10 );
	test(lc::JoystickID{10}, lc::JoystickButton::Button11 );
	test(lc::JoystickID{10}, lc::JoystickButton::Button12 );
	test(lc::JoystickID{10}, lc::JoystickButton::Button13 );
	test(lc::JoystickID{10}, lc::JoystickButton::Button14 );
	test(lc::JoystickID{10}, lc::JoystickButton::Button15 );

	test(lc::JoystickID::any, lc::JoystickAnalogID{11}, lc::AnalogDimension::x      );
	test(lc::JoystickID::any, lc::JoystickAnalogID{11}, lc::AnalogDimension::x_plus );
	test(lc::JoystickID::any, lc::JoystickAnalogID{11}, lc::AnalogDimension::x_minus);
	test(lc::JoystickID::any, lc::JoystickAnalogID{11}, lc::AnalogDimension::y      );
	test(lc::JoystickID::any, lc::JoystickAnalogID{11}, lc::AnalogDimension::y_plus );
	test(lc::JoystickID::any, lc::JoystickAnalogID{11}, lc::AnalogDimension::y_minus);

	test(lc::JoystickID{10}, lc::JoystickAnalogID{11}, lc::AnalogDimension::x      );
	test(lc::JoystickID{10}, lc::JoystickAnalogID{11}, lc::AnalogDimension::x_plus );
	test(lc::JoystickID{10}, lc::JoystickAnalogID{11}, lc::AnalogDimension::x_minus);
	test(lc::JoystickID{10}, lc::JoystickAnalogID{11}, lc::AnalogDimension::y      );
	test(lc::JoystickID{10}, lc::JoystickAnalogID{11}, lc::AnalogDimension::y_plus );
	test(lc::JoystickID{10}, lc::JoystickAnalogID{11}, lc::AnalogDimension::y_minus);
}
