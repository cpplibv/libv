// Project: libv.parse, File: src/libv/ui/parse/parse_orientation2.cpp

// hpp
#include <libv/ui/parse/parse_orientation2.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/char/unicode.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <algorithm>
#include <stdexcept>
// pro
#include <libv/ui/property/orientation2.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

namespace x3 = boost::spirit::x3;

struct named_orientation2s_v_ : x3::symbols<Orientation2> {
	named_orientation2s_v_() {
		add
				("down_right", Orientation2::down_right)
				("down_left", Orientation2::down_left)
				("up_right", Orientation2::up_right)
				("up_left", Orientation2::up_left)
				("left_down", Orientation2::left_down)
				("left_up", Orientation2::left_up)
				("right_down", Orientation2::right_down)
				("right_up", Orientation2::right_up)

				("down-right", Orientation2::down_right)
				("down-left", Orientation2::down_left)
				("up-right", Orientation2::up_right)
				("up-left", Orientation2::up_left)
				("left-down", Orientation2::left_down)
				("left-up", Orientation2::left_up)
				("right-down", Orientation2::right_down)
				("right-up", Orientation2::right_up)

				("down right", Orientation2::down_right)
				("down left", Orientation2::down_left)
				("up right", Orientation2::up_right)
				("up left", Orientation2::up_left)
				("left down", Orientation2::left_down)
				("left up", Orientation2::left_up)
				("right down", Orientation2::right_down)
				("right up", Orientation2::right_up)

				("downright", Orientation2::down_right)
				("downleft", Orientation2::down_left)
				("upright", Orientation2::up_right)
				("upleft", Orientation2::up_left)
				("leftdown", Orientation2::left_down)
				("leftup", Orientation2::left_up)
				("rightdown", Orientation2::right_down)
				("rightup", Orientation2::right_up)
		;
	}
} named_orientation2s_v;

} // namespace -------------------------------------------------------------------------------------

std::optional<Orientation2> parse_orientation2_optional(const std::string_view str) {
	Orientation2 result = Orientation2::right_down;

	auto it = str.begin();
	// NOTE: x3::unicode::space is required for certain invalid UTF8 inputs as x3::space skipper would assert
	auto success = x3::phrase_parse(it, str.end(), x3::no_case[named_orientation2s_v], x3::unicode::space, result);
	success = success && it == str.end();

	return success ? std::optional<Orientation2>{result} : std::nullopt;
}

Orientation2 parse_orientation2_or(const std::string_view str, const Orientation2 fallback) {
	const auto result = parse_orientation2_optional(str);
	return result ? *result : fallback;
}

Orientation2 parse_orientation2_or_throw(const std::string_view str) {
	const auto result = parse_orientation2_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat('"', str, "\" is not a valid orientation2"));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
