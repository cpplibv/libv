// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/input/parse_key.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Parse multi word keys", "[libv.input.parse_key]") {
	CHECK(libv::input::parse_key_or_throw("num lock") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("numlock") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("NumLock") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("num lck") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("numlck") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("NumLck") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("num lck.") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("numlck.") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("NumLck.") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("num lk") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("numlk") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("NumLk") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("num lk.") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("numlk.") == libv::input::Key::NumLock);
	CHECK(libv::input::parse_key_or_throw("NumLk.") == libv::input::Key::NumLock);

	CHECK(libv::input::parse_key_or_throw("Bracket Open") == libv::input::Key::BracketOpen);
	CHECK(libv::input::parse_key_or_throw("BracketOpen") == libv::input::Key::BracketOpen);
	CHECK(libv::input::parse_key_or_throw("bracketopen") == libv::input::Key::BracketOpen);

	CHECK(libv::input::parse_key_or_throw("altgr") == libv::input::Key::AltRight);
	CHECK(libv::input::parse_key_or_throw("altgr.") == libv::input::Key::AltRight);
	CHECK(libv::input::parse_key_or_throw("alt gr") == libv::input::Key::AltRight);
	CHECK(libv::input::parse_key_or_throw("alt gr.") == libv::input::Key::AltRight);
}

TEST_CASE("Parse num keys", "[libv.input.parse_key]") {
	CHECK(libv::input::parse_key_or_throw("0") == libv::input::Key::Num0);
	CHECK(libv::input::parse_key_or_throw("1") == libv::input::Key::Num1);
	CHECK(libv::input::parse_key_or_throw("num0") == libv::input::Key::Num0);
	CHECK(libv::input::parse_key_or_throw("num 1") == libv::input::Key::Num1);
	CHECK(libv::input::parse_key_or_throw("NUM 0") == libv::input::Key::Num0);
	CHECK(libv::input::parse_key_or_throw("Num 1") == libv::input::Key::Num1);
}

TEST_CASE("Parse kp keys", "[libv.input.parse_key]") {
	CHECK(libv::input::parse_key_or_throw("KP minus") == libv::input::Key::KPMinus);
	CHECK(libv::input::parse_key_or_throw("KP. minus") == libv::input::Key::KPMinus);
	CHECK(libv::input::parse_key_or_throw("KP.minus") == libv::input::Key::KPMinus);
	CHECK(libv::input::parse_key_or_throw("KPminus") == libv::input::Key::KPMinus);
	CHECK(libv::input::parse_key_or_throw("Kp -") == libv::input::Key::KPMinus);
	CHECK(libv::input::parse_key_or_throw("Kp minus") == libv::input::Key::KPMinus);
	CHECK(libv::input::parse_key_or_throw("kp -") == libv::input::Key::KPMinus);
	CHECK(libv::input::parse_key_or_throw("kp minus") == libv::input::Key::KPMinus);
	CHECK(libv::input::parse_key_or_throw("kpminus") == libv::input::Key::KPMinus);

	CHECK(libv::input::parse_key_or_throw("kp-") == libv::input::Key::KPMinus);
	CHECK(libv::input::parse_key_or_throw("kp.") == libv::input::Key::KPDot);
	CHECK(libv::input::parse_key_or_throw("kp.-") == libv::input::Key::KPMinus);

	CHECK(libv::input::parse_key_or_throw("kp.0") == libv::input::Key::KPNum0);
	CHECK(libv::input::parse_key_or_throw("kp0") == libv::input::Key::KPNum0);
	CHECK(libv::input::parse_key_or_throw("kp 0") == libv::input::Key::KPNum0);
	CHECK(libv::input::parse_key_or_throw("kpnum0") == libv::input::Key::KPNum0);
	CHECK(libv::input::parse_key_or_throw("kp num0") == libv::input::Key::KPNum0);
	CHECK(libv::input::parse_key_or_throw("kpnum 0") == libv::input::Key::KPNum0);
	CHECK(libv::input::parse_key_or_throw("kp num 0") == libv::input::Key::KPNum0);
}

TEST_CASE("Parse pg keys", "[libv.input.parse_key]") {
	CHECK(libv::input::parse_key_or_throw("pg up") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("pgup") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("PG Up") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("PGUp") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("pg. up") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("pg.up") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("PG. Up") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("PG.Up") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("page up") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("pageup") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("PAGE UP") == libv::input::Key::PageUp);
	CHECK(libv::input::parse_key_or_throw("PAGEUP") == libv::input::Key::PageUp);
}

TEST_CASE("Parse sided keys", "[libv.input.parse_key]") {
	CHECK(libv::input::parse_key_or_throw("left ctrl") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("l. ctrl") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("l.ctrl") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("lctrl") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("L.CTRL") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("LCTRL") == libv::input::Key::ControlLeft);

	CHECK(libv::input::parse_key_or_throw("ctrl left") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("ctrl l.") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("ctrl l") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("ctrll.") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("ctrll") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("CTRLL.") == libv::input::Key::ControlLeft);
	CHECK(libv::input::parse_key_or_throw("CTRLL") == libv::input::Key::ControlLeft);

	CHECK(libv::input::parse_key_or_throw("right shift") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("r. shift") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("r.shift") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("rSHIFT") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("R.SHIFT") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("Rshift") == libv::input::Key::ShiftRight);

	CHECK(libv::input::parse_key_or_throw("shift right") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("shift r.") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("shift r") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("shiftr.") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("SHIFTr") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("SHIFTR.") == libv::input::Key::ShiftRight);
	CHECK(libv::input::parse_key_or_throw("shiftR") == libv::input::Key::ShiftRight);
}

TEST_CASE("Parse named keys", "[libv.input.parse_key]") {
	CHECK(libv::input::parse_key_or_throw("A") == libv::input::Key::A);
	CHECK(libv::input::parse_key_or_throw("a") == libv::input::Key::A);

	CHECK(libv::input::parse_key_or_throw("Apostrophe") == libv::input::Key::Apostrophe);
	CHECK(libv::input::parse_key_or_throw("'") == libv::input::Key::Apostrophe);

	CHECK(libv::input::parse_key_or_throw("\\") == libv::input::Key::Backslash);
	CHECK(libv::input::parse_key_or_throw("backslash") == libv::input::Key::Backslash);

	CHECK(libv::input::parse_key_or_throw("/") == libv::input::Key::Slash);
	CHECK(libv::input::parse_key_or_throw("SLASH") == libv::input::Key::Slash);

	CHECK(libv::input::parse_key_or_throw("Menu") == libv::input::Key::Menu);
}
