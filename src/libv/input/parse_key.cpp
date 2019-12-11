// File: events.hpp, Created on 2019. 02. 04. 23:42, Author: Vader

// hpp
#include <libv/input/parse_key.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <stdexcept>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

Key parse_key_or(const std::string_view str, const Key fallback) {
	const auto result = parse_key_optional(str);
	return result ? *result : fallback;
}

Key parse_key_or_throw(const std::string_view str) {
	const auto result = parse_key_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat("\"", str, "\" is not a valid Key"));
}

std::optional<Key> parse_key_optional(const std::string_view str) {
	namespace x3 = boost::spirit::x3;

	struct named_keys_ : x3::symbols<Key> { named_keys_() { add
//		("Unknown",      Key::Unknown)
		("Space",        Key::Space)
		("Apostrophe",   Key::Apostrophe)
		("'",            Key::Apostrophe)
		("Comma",        Key::Comma)
		(",",            Key::Comma)
		("Minus",        Key::Minus)
		("-",            Key::Minus)
		// <editor-fold defaultstate="collapsed" desc="...">
		("Period",       Key::Period)
		(".",            Key::Period)
		("Slash",        Key::Slash)
		("/",            Key::Slash)
//		("Num0",         Key::Num0)
//		("0",            Key::Num0)
//		("Num1",         Key::Num1)
//		("1",            Key::Num1)
//		("Num2",         Key::Num2)
//		("2",            Key::Num2)
//		("Num3",         Key::Num3)
//		("3",            Key::Num3)
//		("Num4",         Key::Num4)
//		("4",            Key::Num4)
//		("Num5",         Key::Num5)
//		("5",            Key::Num5)
//		("Num6",         Key::Num6)
//		("6",            Key::Num6)
//		("Num7",         Key::Num7)
//		("7",            Key::Num7)
//		("Num8",         Key::Num8)
//		("8",            Key::Num8)
//		("Num9",         Key::Num9)
//		("9",            Key::Num9)
		("Semicolon",    Key::Semicolon)
		(";",            Key::Semicolon)
		("Equals",       Key::Equals)
		("=",            Key::Equals)
		("A",            Key::A)
		("B",            Key::B)
		("C",            Key::C)
		("D",            Key::D)
		("E",            Key::E)
		("F",            Key::F)
		("G",            Key::G)
		("H",            Key::H)
		("I",            Key::I)
		("J",            Key::J)
		("K",            Key::K)
		("L",            Key::L)
		("M",            Key::M)
		("N",            Key::N)
		("O",            Key::O)
		("P",            Key::P)
		("Q",            Key::Q)
		("R",            Key::R)
		("S",            Key::S)
		("T",            Key::T)
		("U",            Key::U)
		("V",            Key::V)
		("W",            Key::W)
		("X",            Key::X)
		("Y",            Key::Y)
		("Z",            Key::Z)
//		("BracketOpen",  Key::BracketOpen)
		("[",            Key::BracketOpen)
		("Backslash",    Key::Backslash)
		("\\",           Key::Backslash)
//		("BracketClose", Key::BracketClose)
		("]",            Key::BracketClose)
		("Backtick",     Key::Backtick)
		("`",            Key::Backtick)
		("World1",       Key::World1)
		("World2",       Key::World2)
		("Escape",       Key::Escape)
		("Esc",          Key::Escape)
		("Enter",        Key::Enter)
		("Tab",          Key::Tab)
		("Backspace",    Key::Backspace)
		("Insert",       Key::Insert)
		("Delete",       Key::Delete)
		("Right",        Key::Right)
		("Left",         Key::Left)
		("Down",         Key::Down)
		("Up",           Key::Up)
//		("PageUp",       Key::PageUp)
//		("PageDown",     Key::PageDown)
		("Home",         Key::Home)
		("End",          Key::End)
//		("CapsLock",     Key::CapsLock)
//		("ScrollLock",   Key::ScrollLock)
//		("NumLock",      Key::NumLock)
//		("PrintScreen",  Key::PrintScreen)
		("Pause",        Key::Pause)
		("F1",           Key::F1)
		("F2",           Key::F2)
		("F3",           Key::F3)
		("F4",           Key::F4)
		("F5",           Key::F5)
		("F6",           Key::F6)
		("F7",           Key::F7)
		("F8",           Key::F8)
		("F9",           Key::F9)
		("F10",          Key::F10)
		("F11",          Key::F11)
		("F12",          Key::F12)
		("F13",          Key::F13)
		("F14",          Key::F14)
		("F15",          Key::F15)
		("F16",          Key::F16)
		("F17",          Key::F17)
		("F18",          Key::F18)
		("F19",          Key::F19)
		("F20",          Key::F20)
		("F21",          Key::F21)
		("F22",          Key::F22)
		("F23",          Key::F23)
		("F24",          Key::F24)
		("F25",          Key::F25)
//		("KPNum0",       Key::KPNum0)
//		("KP0",          Key::KPNum0)
//		("KPNum1",       Key::KPNum1)
//		("KP1",          Key::KPNum1)
//		("KPNum2",       Key::KPNum2)
//		("KP2",          Key::KPNum2)
//		("KPNum3",       Key::KPNum3)
//		("KP3",          Key::KPNum3)
//		("KPNum4",       Key::KPNum4)
//		("KP4",          Key::KPNum4)
//		("KPNum5",       Key::KPNum5)
//		("KP5",          Key::KPNum5)
//		("KPNum6",       Key::KPNum6)
//		("KP6",          Key::KPNum6)
//		("KPNum7",       Key::KPNum7)
//		("KP7",          Key::KPNum7)
//		("KPNum8",       Key::KPNum8)
//		("KP8",          Key::KPNum8)
//		("KPNum9",       Key::KPNum9)
//		("KP9",          Key::KPNum9)
//		("KPDot",        Key::KPDot)
//		("KPSlash",      Key::KPSlash)
//		("KPAsterisk",   Key::KPAsterisk)
//		("KPMinus",      Key::KPMinus)
//		("KPPlus",       Key::KPPlus)
//		("KPEnter",      Key::KPEnter)
//		("KPEqual",      Key::KPEqual)
//		("ShiftLeft",    Key::ShiftLeft)
//		("ControlLeft",  Key::ControlLeft)
//		("AltLeft",      Key::AltLeft)
//		("SuperLeft",    Key::SuperLeft)
//		("ShiftRight",   Key::ShiftRight)
//		("ControlRight", Key::ControlRight)
//		("AltRight",     Key::AltRight)
//		("AltGr",        Key::AltRight)
//		("SuperRight",   Key::SuperRight)
		("Menu",         Key::Menu)

		// Extra aliases:
		("Dash",         Key::Minus)
		("Hyphen",       Key::Minus)
		// </editor-fold>
	; }	} named_keys;

	// ---------------------------------------------------------------------------------------------

	const auto kp_base_keys = x3::rule<class kp_base_keys, Key>{} =
			(-x3::lit("num") >> x3::lit("0") >> x3::attr(Key::KPNum0)) |
			(-x3::lit("num") >> x3::lit("1") >> x3::attr(Key::KPNum1)) |
			(-x3::lit("num") >> x3::lit("2") >> x3::attr(Key::KPNum2)) |
			(-x3::lit("num") >> x3::lit("3") >> x3::attr(Key::KPNum3)) |
			(-x3::lit("num") >> x3::lit("4") >> x3::attr(Key::KPNum4)) |
			(-x3::lit("num") >> x3::lit("5") >> x3::attr(Key::KPNum5)) |
			(-x3::lit("num") >> x3::lit("6") >> x3::attr(Key::KPNum6)) |
			(-x3::lit("num") >> x3::lit("7") >> x3::attr(Key::KPNum7)) |
			(-x3::lit("num") >> x3::lit("8") >> x3::attr(Key::KPNum8)) |
			(-x3::lit("num") >> x3::lit("9") >> x3::attr(Key::KPNum9)) |
			(x3::lit(".") | x3::lit("dot")) >> x3::attr(Key::KPDot) |
			(x3::lit("/") | x3::lit("slash")) >> x3::attr(Key::KPSlash) |
			(x3::lit("*") | x3::lit("asterisk")) >> x3::attr(Key::KPAsterisk) |
			(x3::lit("-") | x3::lit("minus")) >> x3::attr(Key::KPMinus);
			(x3::lit("+") | x3::lit("plus")) >> x3::attr(Key::KPPlus) |
			(x3::lit("enter") >> x3::attr(Key::KPEnter)) |
			(x3::lit("=") | x3::lit("equal")) >> x3::attr(Key::KPEqual);

	const auto num_keys = x3::rule<class num_keys, Key>{} =
			(-x3::lit("num") >> x3::lit("0") >> x3::attr(Key::Num0)) |
			(-x3::lit("num") >> x3::lit("1") >> x3::attr(Key::Num1)) |
			(-x3::lit("num") >> x3::lit("2") >> x3::attr(Key::Num2)) |
			(-x3::lit("num") >> x3::lit("3") >> x3::attr(Key::Num3)) |
			(-x3::lit("num") >> x3::lit("4") >> x3::attr(Key::Num4)) |
			(-x3::lit("num") >> x3::lit("5") >> x3::attr(Key::Num5)) |
			(-x3::lit("num") >> x3::lit("6") >> x3::attr(Key::Num6)) |
			(-x3::lit("num") >> x3::lit("7") >> x3::attr(Key::Num7)) |
			(-x3::lit("num") >> x3::lit("8") >> x3::attr(Key::Num8)) |
			(-x3::lit("num") >> x3::lit("9") >> x3::attr(Key::Num9));

	const auto kp_keys = x3::rule<class kp_keys, Key>{} =
			// Note: lit "." cannot be optional as it would consume the "." token from KPDot
			x3::lit("kp") >> (x3::lit(".") >> kp_base_keys | kp_base_keys);

	const auto pg_keys = x3::rule<class pg_keys, Key>{} =
			(
				x3::lit("pg") >> -x3::lit(".") |
				x3::lit("page")
			) >> (
				(x3::lit("up") >> x3::attr(Key::PageUp)) |
				(x3::lit("down") >> x3::attr(Key::PageDown))
			);

	const auto sided_left_keys = x3::rule<class sided_left_keys, Key>{} =
			(x3::lit("shift") >> x3::attr(Key::ShiftLeft)) |
			(x3::lit("alt") >> x3::attr(Key::AltLeft)) |
			(x3::lit("control") >> x3::attr(Key::ControlLeft)) |
			(x3::lit("ctrl") >> x3::attr(Key::ControlLeft)) |
			(x3::lit("super") >> x3::attr(Key::SuperLeft));

	const auto sided_right_keys = x3::rule<class sided_right_keys, Key>{} =
			(x3::lit("shift") >> x3::attr(Key::ShiftRight)) |
			(x3::lit("alt") >> x3::attr(Key::AltRight)) |
			(x3::lit("control") >> x3::attr(Key::ControlRight)) |
			(x3::lit("ctrl") >> x3::attr(Key::ControlRight)) |
			(x3::lit("super") >> x3::attr(Key::SuperRight));

	const auto sided_keys = x3::rule<class sided_keys, Key>{} =
			(x3::lit("left") | x3::lit("l") >> -x3::lit(".")) >> sided_left_keys |
			(x3::lit("right") | x3::lit("r") >> -x3::lit(".")) >> sided_right_keys |
			sided_left_keys >> (x3::lit("left") | x3::lit("l") >> -x3::lit(".")) |
			sided_right_keys >> (x3::lit("right") | x3::lit("r") >> -x3::lit("."));

	const auto multi_word_keys = x3::rule<class multi_word_keys, Key>{} =
			x3::lit("bracket") >> x3::lit("open") >> x3::attr(Key::BracketOpen) |
			x3::lit("bracket") >> x3::lit("close") >> x3::attr(Key::BracketClose) |
			x3::lit("caps") >>   (x3::lit("lock") | (x3::lit("lck") | x3::lit("lk")) >> -x3::lit(".")) >> x3::attr(Key::CapsLock) |
			x3::lit("scroll") >> (x3::lit("lock") | (x3::lit("lck") | x3::lit("lk")) >> -x3::lit(".")) >> x3::attr(Key::ScrollLock) |
			x3::lit("num") >>    (x3::lit("lock") | (x3::lit("lck") | x3::lit("lk")) >> -x3::lit(".")) >> x3::attr(Key::NumLock) |
			x3::lit("print") >> x3::lit("screen") >> x3::attr(Key::PrintScreen) |
			x3::lit("alt") >> x3::lit("gr") >> -x3::lit(".") >> x3::attr(Key::AltRight);

	const auto key_rule = x3::rule<class key_rule, Key>{} =
			x3::no_case[
				multi_word_keys |
				num_keys |
				kp_keys |
				pg_keys |
				sided_keys |
				named_keys
			];

	Key result = Key::Unknown;

	auto it = str.begin();
	auto success = x3::phrase_parse(it, str.end(), key_rule, x3::space, result);
	success = success && it == str.end();

	return success ? std::optional<Key>{result} : std::nullopt;
}

// -------------------------------------------------------------------------------------------------

} // namespace input
} // namespace libv
