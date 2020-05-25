// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
#include <libv/utility/approx.hpp>
// pro
#include <libv/ctrl/combination.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/input.hpp>
#include <libv/ctrl/parse.hpp>
#include <libv/ctrl/sequence.hpp>
#include "mock_feature.hpp"


// -------------------------------------------------------------------------------------------------

namespace {

namespace lc = libv::ctrl;
namespace li = libv::input;

auto parse(std::string_view str) {
	return lc::parse_sequence_or_throw(str);
}

using MockFeatureAction = MockFeature<lc::arg_action_t>;
using MockFeatureAnalog = MockFeature<lc::arg_analog_t>;
using MockFeatureBinary = MockFeature<lc::arg_binary_t>;

const auto event_key_a_press   = li::EventKey{li::Key::A, li::Scancode{30}, li::Action::press, li::KeyModifier::none};
const auto event_key_a_release = li::EventKey{li::Key::A, li::Scancode{30}, li::Action::release, li::KeyModifier::none};
const auto event_key_a_repeat  = li::EventKey{li::Key::A, li::Scancode{30}, li::Action::repeat, li::KeyModifier::none};
const auto event_key_s_press   = li::EventKey{li::Key::S, li::Scancode{31}, li::Action::press, li::KeyModifier::none};
const auto event_key_s_release = li::EventKey{li::Key::S, li::Scancode{31}, li::Action::release, li::KeyModifier::none};
const auto event_key_s_repeat  = li::EventKey{li::Key::S, li::Scancode{31}, li::Action::repeat, li::KeyModifier::none};
//const auto event_key_d_press   = li::EventKey{li::Key::D, li::Scancode{32}, li::Action::press, li::KeyModifier::none};
//const auto event_key_d_release = li::EventKey{li::Key::D, li::Scancode{32}, li::Action::release, li::KeyModifier::none};
//const auto event_key_d_repeat  = li::EventKey{li::Key::D, li::Scancode{32}, li::Action::repeat, li::KeyModifier::none};
//const auto event_key_f_press   = li::EventKey{li::Key::F, li::Scancode{33}, li::Action::press, li::KeyModifier::none};
//const auto event_key_f_release = li::EventKey{li::Key::F, li::Scancode{33}, li::Action::release, li::KeyModifier::none};
//const auto event_key_f_repeat  = li::EventKey{li::Key::F, li::Scancode{33}, li::Action::repeat, li::KeyModifier::none};
const auto event_key_ctrl_press   = li::EventKey{li::Key::ControlLeft, li::Scancode{29}, li::Action::press, li::KeyModifier::none};
const auto event_key_ctrl_release = li::EventKey{li::Key::ControlLeft, li::Scancode{29}, li::Action::release, li::KeyModifier::none};
const auto event_key_ctrl_repeat  = li::EventKey{li::Key::ControlLeft, li::Scancode{29}, li::Action::repeat, li::KeyModifier::none};

const auto event_mouse_x71_y73 = li::EventMousePosition{71.0, 73.0};
const auto event_mouse_x_p = li::EventMousePosition{1.0, 0.0};
const auto event_mouse_y_p = li::EventMousePosition{0.0, 1.0};
const auto event_mouse_x_m = li::EventMousePosition{-1.0, 0.0};
const auto event_mouse_y_m = li::EventMousePosition{0.0, -1.0};
const auto event_mouse_xy = li::EventMousePosition{1.0, 1.0};

const auto event_mouse_left_press = li::EventMouseButton{li::MouseButton::Left, li::Action::press, li::KeyModifier::none};
const auto event_mouse_left_release = li::EventMouseButton{li::MouseButton::Left, li::Action::release, li::KeyModifier::none};

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("Control shall not execute any feature without input event", "[libv.control]") {
	MockContext context;
	lc::Controls controls;
	MockFeatureAction feature_action;
	MockFeatureAnalog feature_analog;
	MockFeatureBinary feature_binary;

	controls.feature_action<MockContext>("test.action", feature_action.callback());
	controls.feature_analog<MockContext>("test.analog", feature_analog.callback());
	controls.feature_binary<MockContext>("test.binary", feature_binary.callback());

	controls.context_enter(&context);

	controls.bind("test.action", parse("A"));
	controls.bind("test.analog", parse("S"));
	controls.bind("test.binary", parse("D"));

	CHECK(feature_action.empty());
	CHECK(feature_analog.empty());
	CHECK(feature_binary.empty());

	CHECK(context.access_count == 0);
}

TEST_CASE("Control shall execute features", "[libv.control]") {
	MockContext context;
	lc::Controls controls;
	MockFeatureAction feature_action;
	MockFeatureAnalog feature_analog;
	MockFeatureBinary feature_binary;
	MockFeatureAction feature_never_action;
	MockFeatureAnalog feature_never_analog;
	MockFeatureBinary feature_never_binary;

	controls.feature_action<MockContext>("test.action", feature_action.callback());
	controls.feature_analog<MockContext>("test.analog", feature_analog.callback());
	controls.feature_binary<MockContext>("test.binary", feature_binary.callback());
	controls.feature_action<MockContext>("never.action", feature_action.callback());
	controls.feature_analog<MockContext>("never.analog", feature_analog.callback());
	controls.feature_binary<MockContext>("never.binary", feature_binary.callback());

	controls.context_enter(&context);

	// --- Single Button -------------------------------------------------------------------------------

	SECTION("Button [auto] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("A [auto]"));
		controls.bind("test.analog", parse("A [auto]"));
		controls.bind("test.binary", parse("A [auto]"));
		controls.bind("never.action", parse("B [auto]"));
		controls.bind("never.analog", parse("B [auto]"));
		controls.bind("never.binary", parse("B [auto]"));

		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_repeat);
		CHECK(feature_action.consume_first());

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_release);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.update(std::chrono::seconds{1});
		CHECK(context.access_count == 6);
	}

	SECTION("Button [press] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("A [press]"));
		controls.bind("test.analog", parse("A [press]"));
		controls.bind("test.binary", parse("A [press]"));
		controls.bind("never.action", parse("B [press]"));
		controls.bind("never.analog", parse("B [press]"));
		controls.bind("never.binary", parse("B [press]"));

		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_repeat);
		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 3);
	}

	SECTION("Button [repeat] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("A [repeat]"));
		controls.bind("test.analog", parse("A [repeat]"));
		controls.bind("test.binary", parse("A [repeat]"));
		controls.bind("never.action", parse("B [repeat]"));
		controls.bind("never.analog", parse("B [repeat]"));
		controls.bind("never.binary", parse("B [repeat]"));

		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		controls.input(event_key_a_repeat);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	SECTION("Button [release] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("A [release]"));
		controls.bind("test.analog", parse("A [release]"));
		controls.bind("test.binary", parse("A [release]"));
		controls.bind("never.action", parse("B [release]"));
		controls.bind("never.analog", parse("B [release]"));
		controls.bind("never.binary", parse("B [release]"));

		controls.input(event_key_a_press);
		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_repeat);
		controls.update(std::chrono::seconds{1});

		controls.input(event_key_a_release);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 3);
	}

	SECTION("Button [any] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("A [any]"));
		controls.bind("test.analog", parse("A [any]"));
		controls.bind("test.binary", parse("A [any]"));
		controls.bind("never.action", parse("B [any]"));
		controls.bind("never.analog", parse("B [any]"));
		controls.bind("never.binary", parse("B [any]"));

		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_repeat);
		controls.update(std::chrono::seconds{1});

		controls.input(event_key_a_release);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	SECTION("Button [hold] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("A [hold]"));
		controls.bind("test.analog", parse("A [hold]"));
		controls.bind("test.binary", parse("A [hold]"));
		controls.bind("never.action", parse("B [hold]"));
		controls.bind("never.analog", parse("B [hold]"));
		controls.bind("never.binary", parse("B [hold]"));

		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_repeat);
		CHECK(feature_action.consume_first());

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_release);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

//	SECTION("Button [free] bindings with press-T-repeat-T-release pass") {
//		controls.bind("test.action", parse("A [free]"));
//		controls.bind("test.analog", parse("A [free]"));
//		controls.bind("test.binary", parse("A [free]"));
//		controls.bind("never.action", parse("B [free]"));
//		controls.bind("never.analog", parse("B [free]"));
//		controls.bind("never.binary", parse("B [free]"));
//
//		controls.input(event_key_a_press);
//		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});
//
//		controls.update(std::chrono::seconds{1});
//		controls.input(event_key_a_repeat);
//		controls.update(std::chrono::seconds{1});
//
//		controls.input(event_key_a_release);
//		CHECK(feature_action.consume_first());
//		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
//
//		controls.update(std::chrono::seconds{1});
//		CHECK(feature_analog.consume_first());
//
//		CHECK(context.access_count == 4);
//	}

	SECTION("Analog [X] bindings with plus-T-minus pass") {
		controls.bind("test.action", parse("MX"));
		controls.bind("test.analog", parse("MX"));
		controls.bind("test.binary", parse("MX"));
		controls.bind("never.action", parse("MY"));
		controls.bind("never.analog", parse("MY"));
		controls.bind("never.binary", parse("MY"));

		controls.input(event_mouse_x_p);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		controls.input(event_mouse_x_m);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	SECTION("Analog [X+] bindings with plus-T-minus pass") {
		controls.bind("test.action", parse("MX+"));
		controls.bind("test.analog", parse("MX+"));
		controls.bind("test.binary", parse("MX+"));
		controls.bind("never.action", parse("MY+"));
		controls.bind("never.analog", parse("MY+"));
		controls.bind("never.binary", parse("MY+"));

		controls.input(event_mouse_x_p);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		controls.input(event_mouse_x_m);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 4);
	}

	SECTION("Analog [X-] bindings with plus-T-minus pass") {
		controls.bind("test.action", parse("MX-"));
		controls.bind("test.analog", parse("MX-"));
		controls.bind("test.binary", parse("MX-"));
		controls.bind("never.action", parse("MY-"));
		controls.bind("never.analog", parse("MY-"));
		controls.bind("never.binary", parse("MY-"));

		controls.input(event_mouse_x_p);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.update(std::chrono::seconds{1});

		controls.input(event_mouse_x_m);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 4);
	}

	// --- Modifier with Single Button -----------------------------------------------------------------

	SECTION("Modifier + Button [auto] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("Ctrl [auto] + A [auto]"));
		controls.bind("test.analog", parse("Ctrl [auto] + A [auto]"));
		controls.bind("test.binary", parse("Ctrl [auto] + A [auto]"));
		controls.bind("never.action", parse("Ctrl [auto] + B [auto]"));
		controls.bind("never.analog", parse("Ctrl [auto] + B [auto]"));
		controls.bind("never.binary", parse("Ctrl [auto] + B [auto]"));
		controls.bind("never.action", parse("Shft [auto] + A [auto]"));
		controls.bind("never.analog", parse("Shft [auto] + A [auto]"));
		controls.bind("never.binary", parse("Shft [auto] + A [auto]"));

		controls.input(event_key_ctrl_press);
		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_repeat);
		CHECK(feature_action.consume_first());

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_release);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.input(event_key_ctrl_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	SECTION("Modifier + Button [press] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("Ctrl [auto] + A [press]"));
		controls.bind("test.analog", parse("Ctrl [auto] + A [press]"));
		controls.bind("test.binary", parse("Ctrl [auto] + A [press]"));
		controls.bind("never.action", parse("Ctrl [auto] + B [press]"));
		controls.bind("never.analog", parse("Ctrl [auto] + B [press]"));
		controls.bind("never.binary", parse("Ctrl [auto] + B [press]"));
		controls.bind("never.action", parse("Shft [auto] + A [press]"));
		controls.bind("never.analog", parse("Shft [auto] + A [press]"));
		controls.bind("never.binary", parse("Shft [auto] + A [press]"));

		controls.input(event_key_ctrl_press);
		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_repeat);
		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_release);
		controls.input(event_key_ctrl_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 3);
	}

	SECTION("Modifier + Button [repeat] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("Ctrl [auto] + A [repeat]"));
		controls.bind("test.analog", parse("Ctrl [auto] + A [repeat]"));
		controls.bind("test.binary", parse("Ctrl [auto] + A [repeat]"));
		controls.bind("never.action", parse("Ctrl [auto] + B [repeat]"));
		controls.bind("never.analog", parse("Ctrl [auto] + B [repeat]"));
		controls.bind("never.binary", parse("Ctrl [auto] + B [repeat]"));
		controls.bind("never.action", parse("Shft [auto] + A [repeat]"));
		controls.bind("never.analog", parse("Shft [auto] + A [repeat]"));
		controls.bind("never.binary", parse("Shft [auto] + A [repeat]"));

		controls.input(event_key_ctrl_press);
		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		controls.input(event_key_a_repeat);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_release);
		controls.input(event_key_ctrl_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	SECTION("Modifier + Button [release] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("Ctrl [auto] + A [release]"));
		controls.bind("test.analog", parse("Ctrl [auto] + A [release]"));
		controls.bind("test.binary", parse("Ctrl [auto] + A [release]"));
		controls.bind("never.action", parse("Ctrl [auto] + B [release]"));
		controls.bind("never.analog", parse("Ctrl [auto] + B [release]"));
		controls.bind("never.binary", parse("Ctrl [auto] + B [release]"));
		controls.bind("never.action", parse("Shft [auto] + A [release]"));
		controls.bind("never.analog", parse("Shft [auto] + A [release]"));
		controls.bind("never.binary", parse("Shft [auto] + A [release]"));

		controls.input(event_key_ctrl_press);
		controls.input(event_key_a_press);
		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_repeat);
		controls.update(std::chrono::seconds{1});

		controls.input(event_key_a_release);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_ctrl_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 3);
	}

	SECTION("Modifier + Button [any] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("Ctrl [auto] + A [any]"));
		controls.bind("test.analog", parse("Ctrl [auto] + A [any]"));
		controls.bind("test.binary", parse("Ctrl [auto] + A [any]"));
		controls.bind("never.action", parse("Ctrl [auto] + B [any]"));
		controls.bind("never.analog", parse("Ctrl [auto] + B [any]"));
		controls.bind("never.binary", parse("Ctrl [auto] + B [any]"));
		controls.bind("never.action", parse("Shft [auto] + A [any]"));
		controls.bind("never.analog", parse("Shft [auto] + A [any]"));
		controls.bind("never.binary", parse("Shft [auto] + A [any]"));

		controls.input(event_key_ctrl_press);
		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_repeat);
		controls.update(std::chrono::seconds{1});

		controls.input(event_key_a_release);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_ctrl_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	SECTION("Modifier + Button [hold] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("Ctrl [auto] + A [hold]"));
		controls.bind("test.analog", parse("Ctrl [auto] + A [hold]"));
		controls.bind("test.binary", parse("Ctrl [auto] + A [hold]"));
		controls.bind("never.action", parse("Ctrl [auto] + B [hold]"));
		controls.bind("never.analog", parse("Ctrl [auto] + B [hold]"));
		controls.bind("never.binary", parse("Ctrl [auto] + B [hold]"));
		controls.bind("never.action", parse("Shft [auto] + A [hold]"));
		controls.bind("never.analog", parse("Shft [auto] + A [hold]"));
		controls.bind("never.binary", parse("Shft [auto] + A [hold]"));

		controls.input(event_key_ctrl_press);
		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_repeat);
		CHECK(feature_action.consume_first());

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_release);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.input(event_key_ctrl_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

//	SECTION("Modifier + Button [free] bindings with press-T-repeat-T-release pass") {
//		controls.bind("test.action", parse("Ctrl [auto] + A [free]"));
//		controls.bind("test.analog", parse("Ctrl [auto] + A [free]"));
//		controls.bind("test.binary", parse("Ctrl [auto] + A [free]"));
//		controls.bind("never.action", parse("Ctrl [auto] + B [free]"));
//		controls.bind("never.analog", parse("Ctrl [auto] + B [free]"));
//		controls.bind("never.binary", parse("Ctrl [auto] + B [free]"));
//		controls.bind("never.action", parse("Shft [auto] + A [free]"));
//		controls.bind("never.analog", parse("Shft [auto] + A [free]"));
//		controls.bind("never.binary", parse("Shft [auto] + A [free]"));
//
//		controls.input(event_key_ctrl_press);
//		controls.input(event_key_a_press);
//		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});
//
//		controls.update(std::chrono::seconds{1});
//		controls.input(event_key_a_repeat);
//		controls.update(std::chrono::seconds{1});
//
//		controls.input(event_key_a_release);
//		CHECK(feature_action.consume_first());
//		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
//
//		controls.input(event_key_ctrl_release);
//		controls.update(std::chrono::seconds{1});
//		CHECK(feature_analog.consume_first());
//
//		CHECK(context.access_count == 4);
//	}

	SECTION("Modifier + Analog [X] bindings with plus-T-minus pass") {
		controls.bind("test.action", parse("Ctrl + MX"));
		controls.bind("test.analog", parse("Ctrl + MX"));
		controls.bind("test.binary", parse("Ctrl + MX"));
		controls.bind("never.action", parse("Ctrl + A"));
		controls.bind("never.analog", parse("Ctrl + A"));
		controls.bind("never.binary", parse("Ctrl + A"));
		controls.bind("never.action", parse("Ctrl + MY"));
		controls.bind("never.analog", parse("Ctrl + MY"));
		controls.bind("never.binary", parse("Ctrl + MY"));
		controls.bind("never.action", parse("Shft + MX"));
		controls.bind("never.analog", parse("Shft + MX"));
		controls.bind("never.binary", parse("Shft + MX"));

		controls.input(event_key_ctrl_press);
		controls.input(event_mouse_x_p);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		controls.input(event_mouse_x_m);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_ctrl_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	SECTION("Modifier + Analog [X+] bindings with plus-T-minus pass") {
		controls.bind("test.action", parse("Ctrl + MX+"));
		controls.bind("test.analog", parse("Ctrl + MX+"));
		controls.bind("test.binary", parse("Ctrl + MX+"));
		controls.bind("never.action", parse("Ctrl + A"));
		controls.bind("never.analog", parse("Ctrl + A"));
		controls.bind("never.binary", parse("Ctrl + A"));
		controls.bind("never.action", parse("Ctrl + MY+"));
		controls.bind("never.analog", parse("Ctrl + MY+"));
		controls.bind("never.binary", parse("Ctrl + MY+"));
		controls.bind("never.action", parse("Shft + MX+"));
		controls.bind("never.analog", parse("Shft + MX+"));
		controls.bind("never.binary", parse("Shft + MX+"));

		controls.input(event_key_ctrl_press);
		controls.input(event_mouse_x_p);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		controls.input(event_mouse_x_m);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.input(event_key_ctrl_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 4);
	}

	SECTION("Modifier + Analog [X-] bindings with plus-T-minus pass") {
		controls.bind("test.action", parse("Ctrl + MX-"));
		controls.bind("test.analog", parse("Ctrl + MX-"));
		controls.bind("test.binary", parse("Ctrl + MX-"));
		controls.bind("never.action", parse("Ctrl + A"));
		controls.bind("never.analog", parse("Ctrl + A"));
		controls.bind("never.binary", parse("Ctrl + A"));
		controls.bind("never.action", parse("Ctrl + MY-"));
		controls.bind("never.analog", parse("Ctrl + MY-"));
		controls.bind("never.binary", parse("Ctrl + MY-"));
		controls.bind("never.action", parse("Shft + MX-"));
		controls.bind("never.analog", parse("Shft + MX-"));
		controls.bind("never.binary", parse("Shft + MX-"));

		controls.input(event_key_ctrl_press);
		controls.input(event_mouse_x_p);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.update(std::chrono::seconds{1});

		controls.input(event_mouse_x_m);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_ctrl_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 4);
	}

	// --- Modifier + Single Button into Single Button sequence ---------------------------------

	SECTION("Modifier + Button [auto], Button [auto] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("Ctrl [auto] + A [auto], A [auto]"));
		controls.bind("test.analog", parse("Ctrl [auto] + A [auto], A [auto]"));
		controls.bind("test.binary", parse("Ctrl [auto] + A [auto], A [auto]"));
		controls.bind("never.action", parse("Ctrl [auto] + A [auto], B [auto]"));
		controls.bind("never.analog", parse("Ctrl [auto] + A [auto], B [auto]"));
		controls.bind("never.binary", parse("Ctrl [auto] + A [auto], B [auto]"));
		controls.bind("never.action", parse("Ctrl [auto] + B [auto], A [auto]"));
		controls.bind("never.analog", parse("Ctrl [auto] + B [auto], A [auto]"));
		controls.bind("never.binary", parse("Ctrl [auto] + B [auto], A [auto]"));
		controls.bind("never.action", parse("Shft [auto] + A [auto], A [auto]"));
		controls.bind("never.analog", parse("Shft [auto] + A [auto], A [auto]"));
		controls.bind("never.binary", parse("Shft [auto] + A [auto], A [auto]"));
		controls.bind("never.action", parse("Shft [auto] + A [auto]"));
		controls.bind("never.analog", parse("Shft [auto] + A [auto]"));
		controls.bind("never.binary", parse("Shft [auto] + A [auto]"));

		SECTION("Sequence activation 1a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
		}
		SECTION("Sequence activation 1b") {
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_press);
		}
		SECTION("Sequence activation 2a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_release);
		}
		SECTION("Sequence activation 2b") {
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_press);
			controls.input(event_key_ctrl_release);
		}
		SECTION("Sequence activation 3a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
		}
		SECTION("Sequence activation 3b") {
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_release);
		}
		SECTION("Sequence activation 4a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);
		}
		SECTION("Sequence activation 4b") {
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_press);
			controls.input(event_key_ctrl_release);
			controls.input(event_key_a_release);
		}
		SECTION("Sequence activation 5a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_release);
			controls.input(event_key_a_release);
		}
		SECTION("Sequence activation 5b") {
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);
		}

		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_repeat);
		CHECK(feature_action.consume_first());

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_release);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	SECTION("Modifier + Button [press], Button [auto] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("Ctrl [auto] + A [press], A [auto]"));
		controls.bind("test.analog", parse("Ctrl [auto] + A [press], A [auto]"));
		controls.bind("test.binary", parse("Ctrl [auto] + A [press], A [auto]"));
		controls.bind("never.action", parse("Ctrl [auto] + A [press], B [auto]"));
		controls.bind("never.analog", parse("Ctrl [auto] + A [press], B [auto]"));
		controls.bind("never.binary", parse("Ctrl [auto] + A [press], B [auto]"));
		controls.bind("never.action", parse("Ctrl [auto] + B [press], A [auto]"));
		controls.bind("never.analog", parse("Ctrl [auto] + B [press], A [auto]"));
		controls.bind("never.binary", parse("Ctrl [auto] + B [press], A [auto]"));
		controls.bind("never.action", parse("Shft [auto] + A [press], A [auto]"));
		controls.bind("never.analog", parse("Shft [auto] + A [press], A [auto]"));
		controls.bind("never.binary", parse("Shft [auto] + A [press], A [auto]"));
		controls.bind("never.action", parse("Shft [auto] + A [press]"));
		controls.bind("never.analog", parse("Shft [auto] + A [press]"));
		controls.bind("never.binary", parse("Shft [auto] + A [press]"));

		SECTION("Sequence activation 1a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
		}
		SECTION("Sequence activation 2a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_release);
		}
		SECTION("Sequence activation 3a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
		}
		SECTION("Sequence activation 4a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);
		}
		SECTION("Sequence activation 5a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_release);
			controls.input(event_key_a_release);
		}

		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_repeat);
		CHECK(feature_action.consume_first());

		controls.update(std::chrono::seconds{1});
		CHECK(feature_analog.consume_first());

		controls.input(event_key_a_release);
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	SECTION("Modifier + Button [release], Button [repeat] bindings with press-T-repeat-T-release pass") {
		controls.bind("test.action", parse("Ctrl [auto] + A [release], A [repeat]"));
		controls.bind("test.analog", parse("Ctrl [auto] + A [release], A [repeat]"));
		controls.bind("test.binary", parse("Ctrl [auto] + A [release], A [repeat]"));
		controls.bind("never.action", parse("Ctrl [auto] + A [release], B [repeat]"));
		controls.bind("never.analog", parse("Ctrl [auto] + A [release], B [repeat]"));
		controls.bind("never.binary", parse("Ctrl [auto] + A [release], B [repeat]"));
		controls.bind("never.action", parse("Ctrl [auto] + B [release], A [repeat]"));
		controls.bind("never.analog", parse("Ctrl [auto] + B [release], A [repeat]"));
		controls.bind("never.binary", parse("Ctrl [auto] + B [release], A [repeat]"));
		controls.bind("never.action", parse("Shft [auto] + A [release], A [repeat]"));
		controls.bind("never.analog", parse("Shft [auto] + A [release], A [repeat]"));
		controls.bind("never.binary", parse("Shft [auto] + A [release], A [repeat]"));
		controls.bind("never.action", parse("Shft [auto] + A [release]"));
		controls.bind("never.analog", parse("Shft [auto] + A [release]"));
		controls.bind("never.binary", parse("Shft [auto] + A [release]"));

		SECTION("Sequence activation 3a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
		}
		SECTION("Sequence activation 3b") {
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_release);
		}
		SECTION("Sequence activation 4a") {
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);
		}
		SECTION("Sequence activation 5b") {
			controls.input(event_key_a_press);
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);
		}

		controls.input(event_key_a_press);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});

		controls.input(event_key_a_repeat);
		CHECK(feature_action.consume_first());
		CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});
		CHECK(feature_analog.consume_first());

		controls.update(std::chrono::seconds{1});
		controls.input(event_key_a_release);
		controls.update(std::chrono::seconds{1});

		CHECK(context.access_count == 6);
	}

	// -------------------------------------------------------------------------------------------------

	CHECK(feature_action.empty());
	CHECK(feature_analog.empty());
	CHECK(feature_binary.empty());
	CHECK(feature_never_action.empty());
	CHECK(feature_never_analog.empty());
	CHECK(feature_never_binary.empty());
}

TEST_CASE("Select more specialized binding in case of collision", "[libv.control]") {
	MockContext context;
	lc::Controls controls;
	MockFeatureAction feature_action;
	MockFeatureAction feature_never_action;

	controls.feature_action<MockContext>("test.action", feature_action.callback());
	controls.feature_action<MockContext>("never.action", feature_action.callback());

	controls.context_enter(&context);

	controls.bind("test.action", parse("Ctrl + A"));
	controls.bind("never.action", parse("A"));

	controls.input(event_key_ctrl_press);
	controls.input(event_key_a_press);
	controls.input(event_key_a_release);
	controls.input(event_key_ctrl_release);

	CHECK(feature_action.consume_first());
	CHECK(context.access_count == 1);

	CHECK(feature_action.empty());
	CHECK(feature_never_action.empty());
}

TEST_CASE("Reset sequence on unrelated event", "[libv.control]") {
	MockContext context;
	lc::Controls controls;
	MockFeatureAction feature_action;
	MockFeatureAnalog feature_analog;
	MockFeatureBinary feature_binary;
	MockFeatureAction feature_never_action;
	MockFeatureAnalog feature_never_analog;
	MockFeatureBinary feature_never_binary;

	controls.feature_action<MockContext>("test.action", feature_action.callback());
	controls.feature_analog<MockContext>("test.analog", feature_analog.callback());
	controls.feature_binary<MockContext>("test.binary", feature_binary.callback());
	controls.feature_action<MockContext>("never.action", feature_action.callback());
	controls.feature_analog<MockContext>("never.analog", feature_analog.callback());
	controls.feature_binary<MockContext>("never.binary", feature_binary.callback());

	controls.context_enter(&context);

	SECTION("Unrelated event and timeout cancels sequence") {
		controls.bind("test.action", parse("Ctrl + A, A"));
		controls.bind("test.analog", parse("Ctrl + A, A"));
		controls.bind("test.binary", parse("Ctrl + A, A"));

		{
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);

			controls.input(event_key_s_press); // <- unrelated button event

			controls.input(event_key_a_press);
			controls.update(std::chrono::seconds{1});
			controls.input(event_key_a_repeat);
			controls.update(std::chrono::seconds{1});
			controls.input(event_key_a_release);
			controls.update(std::chrono::seconds{1});
		}

		// Nothing so far, try again

		{
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);

			controls.input(event_key_a_press);
			CHECK(feature_action.consume_first());
			CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});

			controls.update(std::chrono::seconds{1});
			CHECK(feature_analog.consume_first());

			controls.input(event_key_a_repeat);
			CHECK(feature_action.consume_first());

			controls.update(std::chrono::seconds{1});
			CHECK(feature_analog.consume_first());

			controls.input(event_key_a_release);
			CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

			controls.update(std::chrono::seconds{1});
		}

		// Try again with cancelling

		{
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);

			controls.input(event_mouse_x_p); // <- unrelated analog event

			controls.input(event_key_a_press);
			controls.update(std::chrono::seconds{1});
			controls.input(event_key_a_repeat);
			controls.update(std::chrono::seconds{1});
			controls.input(event_key_a_release);
			controls.update(std::chrono::seconds{1});
		}

		// Nothing so far, try again

		{
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);

			controls.input(event_key_a_press);
			CHECK(feature_action.consume_first());
			CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});

			controls.update(std::chrono::seconds{1});
			CHECK(feature_analog.consume_first());

			controls.input(event_key_a_repeat);
			CHECK(feature_action.consume_first());

			controls.update(std::chrono::seconds{1});
			CHECK(feature_analog.consume_first());

			controls.input(event_key_a_release);
			CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

			controls.update(std::chrono::seconds{1});
		}

		// Try again with cancelling

		{
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);

			controls.update(std::chrono::seconds{300}); // <- timeout

			controls.input(event_key_a_press);
			controls.update(std::chrono::seconds{1});
			controls.input(event_key_a_repeat);
			controls.update(std::chrono::seconds{1});
			controls.input(event_key_a_release);
			controls.update(std::chrono::seconds{1});
		}

		// Nothing so far, try again

		{
			controls.input(event_key_ctrl_press);
			controls.input(event_key_a_press);
			controls.input(event_key_a_release);
			controls.input(event_key_ctrl_release);

			controls.input(event_key_a_press);
			CHECK(feature_action.consume_first());
			CHECK(feature_binary.consume_first() == lc::arg_binary_t{true});

			controls.update(std::chrono::seconds{1});
			CHECK(feature_analog.consume_first());

			controls.input(event_key_a_repeat);
			CHECK(feature_action.consume_first());

			controls.update(std::chrono::seconds{1});
			CHECK(feature_analog.consume_first());

			controls.input(event_key_a_release);
			CHECK(feature_binary.consume_first() == lc::arg_binary_t{false});

			controls.update(std::chrono::seconds{1});
		}

		CHECK(context.access_count == 18);
	}

	CHECK(feature_action.empty());
	CHECK(feature_analog.empty());
	CHECK(feature_binary.empty());
	CHECK(feature_never_action.empty());
	CHECK(feature_never_analog.empty());
	CHECK(feature_never_binary.empty());
}

TEST_CASE("Routing scale and impulse", "[libv.control]") {
	MockContext context;
	lc::Controls controls;
	MockFeatureAnalog feature_abi;
	MockFeatureAnalog feature_abt0;
	MockFeatureAnalog feature_abt1;
	MockFeatureAnalog feature_aas;

	controls.feature_analog<MockContext>("test.abi", feature_abi.callback(), 2, 3, 59);
	controls.feature_analog<MockContext>("test.abt0", feature_abt0.callback(), 5, 7, 61);
	controls.feature_analog<MockContext>("test.abt1", feature_abt1.callback(), 83, 89, 97);
	controls.feature_analog<MockContext>("test.aas", feature_aas.callback(), 11, 13, 67);

	controls.context_enter(&context);

	//	2, 3, 5, 7, 11, 13,
	//	17, 19, 23, 29, 37, 41,
	//	43, 47, 53,
	//	59, 61, 67,
	//	71, 73,
	//	79, 83, 89, 97,
	//	101,

	//	31, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
	//	157, 163, 167, 173, 179, 181, 191, 193, 197, 199

	controls.bind("test.abi", parse("A [repeat]"), 17);
	controls.bind("test.abt0", parse("A [hold]"), 19);
	controls.bind("test.abt1", parse("LMB"), 79);
	controls.bind("test.aas", parse("Mouse X"), 23);

	using ap = libv::Approx<lc::scale_type>;
	const auto get_or_nan = [](auto& feature) {
		const auto value = feature.consume_first();
		return value ? value->value : std::numeric_limits<lc::scale_type>::quiet_NaN();
	};

	controls.impulse_keyboard(29);
	controls.impulse_mouse_button(101);
	controls.scale_mouse_move(37);
	controls.scale_time(41);

	{
		controls.input(event_key_a_press);
		CHECK(get_or_nan(feature_abi) == ap(2 * 17 * 29));

		controls.input(event_mouse_x71_y73);
		CHECK(get_or_nan(feature_aas) == ap(67 * 23 * 37 * 71 * lc::MOUSE_NORMALIZATION_SCALE));

		controls.update(std::chrono::seconds{43});
		CHECK(get_or_nan(feature_abt0) == ap(7 * 19 * 41 * 43));

		controls.input(event_key_a_repeat);
		CHECK(get_or_nan(feature_abi) == ap(2 * 17 * 29));

		controls.update(std::chrono::seconds{47});
		CHECK(get_or_nan(feature_abt0) == ap(7 * 19 * 41 * 47));

		controls.input(event_key_a_release);
		controls.input(event_mouse_left_press);

		controls.update(std::chrono::seconds{107});
		CHECK(get_or_nan(feature_abt1) == ap(41 * 107 * 79 * 89));

		controls.input(event_mouse_left_release);
		controls.update(std::chrono::seconds{53});

		CHECK(context.access_count == 6);
	}

	CHECK(feature_abi.empty());
	CHECK(feature_abt0.empty());
	CHECK(feature_aas.empty());
}

//TEST_CASE("Feature execution with context", "[libv.control]") { }
//TEST_CASE("Feature execution with overloaded context", "[libv.control]") { }
//TEST_CASE("Feature execution with void context", "[libv.control]") { }
//TEST_CASE("Feature no execution without context", "[libv.control]") { }
//
//TEST_CASE("Context overloading features with the same names", "[libv.control]") { }
//TEST_CASE("Context overloading features stack push", "[libv.control]") { }
//TEST_CASE("Context overloading features stack pop", "[libv.control]") { }
//
//TEST_CASE("Missing context means no features activity", "[libv.control]") { }
//TEST_CASE("Enter context with instant binding fulfilment causes features activity", "[libv.control]") { }
//TEST_CASE("Leave context with fulfilled bindings causes features activity", "[libv.control]") { }
//TEST_CASE("New binding with instant fulfilment causes features activity", "[libv.control]") { }
