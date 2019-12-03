// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

//// hpp
//#include <catch/catch.hpp>
//// pro
//#include <libv/input/parse_combination.hpp>
//
//
//// -------------------------------------------------------------------------------------------------
//
//#include <libv/input/input.hpp>
//
//namespace libv {
//namespace input {
//
//struct Combination {
//	boost::container::small_vector<libv::input::Key> key;
//	libv::input::KeyModifier modifier;
//};
//
//} // namespace input
//} // namespace libv
//
//// -------------------------------------------------------------------------------------------------
//
//TEST_CASE("Parse sided keys", "[libv.input.parse_combination]") {
//	const auto ctrl_a = libv::input::Combination(
//		libv::input::Key::ControlLeft,
//		libv::input::Key::A
//	);
//	const auto shift_r_a = libv::input::Combination(
//		libv::input::Key::ShiftRight,
//		libv::input::Key::A
//	);
//	const auto shift_a = libv::input::Combination(
//		libv::input::KeyModifier::shift,
//		libv::input::Key::A
//	);
//
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("left ctrl + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("l. ctrl + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("l.ctrl + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("lctrl + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("L.CTRL + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("LCTRL + A"));
//
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("ctrl left + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("ctrl l. + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("ctrl l + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("ctrll. + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("ctrll + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("CTRLL. + A"));
//	CHECK(ctrl_a == libv::input::parse_combination_or_throw("CTRLL + A"));
//
//	CHECK(shift_r_a == libv::input::parse_combination_or_throw("right shift + A"));
//
//	CHECK(shift_a == libv::input::parse_combination_or_throw("shift + A"));
//	CHECK(shift_a == libv::input::parse_combination_or_throw("right shift + A"));
//	CHECK(shift_a == libv::input::parse_combination_or_throw("left shift + A"));
//}
