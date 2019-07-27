// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
// pro
#include <libv/control/controls.hpp>
#include <libv/control/parse.hpp>
#include <libv/control/sequence.hpp>
#include "mock_feature.hpp"


// -------------------------------------------------------------------------------------------------

namespace {

namespace lc = libv::control;
namespace li = libv::input;

auto parse(std::string_view str) {
	return lc::parse_sequence_or_throw(str);
}

using MockFeatureAction = MockFeature<lc::arg_action_t>;
using MockFeatureAnalog = MockFeature<lc::arg_analog_t>;
using MockFeatureBinary = MockFeature<lc::arg_binary_t>;

const auto event_key_a_press   = li::EventKey{li::Key::A, li::Scancode{30}, li::Action::press, li::KeyModifier::none};
const auto event_key_a_release = li::EventKey{li::Key::A, li::Scancode{30}, li::Action::release, li::KeyModifier::none};
//const auto event_key_a_repeat  = li::EventKey{li::Key::A, li::Scancode{30}, li::Action::repeat, li::KeyModifier::none};
//const auto event_key_s_press   = li::EventKey{li::Key::S, li::Scancode{31}, li::Action::press, li::KeyModifier::none};
//const auto event_key_s_release = li::EventKey{li::Key::S, li::Scancode{31}, li::Action::release, li::KeyModifier::none};
//const auto event_key_s_repeat  = li::EventKey{li::Key::S, li::Scancode{31}, li::Action::repeat, li::KeyModifier::none};
//const auto event_key_d_press   = li::EventKey{li::Key::D, li::Scancode{32}, li::Action::press, li::KeyModifier::none};
//const auto event_key_d_release = li::EventKey{li::Key::D, li::Scancode{32}, li::Action::release, li::KeyModifier::none};
//const auto event_key_d_repeat  = li::EventKey{li::Key::D, li::Scancode{32}, li::Action::repeat, li::KeyModifier::none};
//const auto event_key_f_press   = li::EventKey{li::Key::F, li::Scancode{33}, li::Action::press, li::KeyModifier::none};
//const auto event_key_f_release = li::EventKey{li::Key::F, li::Scancode{33}, li::Action::release, li::KeyModifier::none};
//const auto event_key_f_repeat  = li::EventKey{li::Key::F, li::Scancode{33}, li::Action::repeat, li::KeyModifier::none};
//const auto event_key_ctrl_press   = li::EventKey{li::Key::ControlLeft, li::Scancode{29}, li::Action::press, li::KeyModifier::none};
//const auto event_key_ctrl_release = li::EventKey{li::Key::ControlLeft, li::Scancode{29}, li::Action::release, li::KeyModifier::none};
//const auto event_key_ctrl_repeat  = li::EventKey{li::Key::ControlLeft, li::Scancode{29}, li::Action::repeat, li::KeyModifier::none};

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("Bind to missing feature", "[libv.control]") {
	lc::Controls controls;
	controls.bind("test.missing", parse("A"));
}

TEST_CASE("Binding keycode", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding codepoint", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding modifier", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding scancode", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mouse button", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mouse scroll", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mouse move", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding gamepad button", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding gamepad any button", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding gamepad analog", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding gamepad any analog", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}

TEST_CASE("Binding timed: button + time", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding timed: analog + time", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding timed mix: button + button + time", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding timed mix: button + analog + time", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}

TEST_CASE("Binding mix: analog + analog", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mix: analog + button", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mix: button + button", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mix: analog + analog + analog", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mix: analog + analog + button", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mix: analog + button + button", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mix: button + analog + analog", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mix: button + analog + button", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
TEST_CASE("Binding mix: button + button + button", "[libv.control]") {
	SECTION("to action feature") { }
	SECTION("to analog feature") { }
	SECTION("to binary feature") { }
}
