// Project: libv.parse, File: src/libv/ui/parse/parse_orientation.cpp

// hpp
#include <libv/ui/parse/parse_orientation.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/char/unicode.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <algorithm>
#include <stdexcept>
// pro
#include <libv/ui/property/orientation.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

namespace x3 = boost::spirit::x3;

struct named_orientations_v_ : x3::symbols<Orientation> {
	named_orientations_v_() {
		add
				("bottom_to_top", Orientation::up)
				("left_to_right", Orientation::right)
				("right_to_left", Orientation::left)
				("top_to_bottom", Orientation::down)

				("bottom-to-top", Orientation::up)
				("left-to-right", Orientation::right)
				("right-to-left", Orientation::left)
				("top-to-bottom", Orientation::down)

				("bottom to top", Orientation::up)
				("left to right", Orientation::right)
				("right to left", Orientation::left)
				("top to bottom", Orientation::down)

				("up", Orientation::up)
				("right", Orientation::right)
				("left", Orientation::left)
				("down", Orientation::down)
				;
	}
} named_orientations_v;

} // namespace -------------------------------------------------------------------------------------

std::optional<Orientation> parse_orientation_optional(const std::string_view str) {
	Orientation result = Orientation::down;

	auto it = str.begin();
	// NOTE: x3::unicode::space is required for certain invalid UTF8 inputs as x3::space skipper would assert
	auto success = x3::phrase_parse(it, str.end(), x3::no_case[named_orientations_v], x3::unicode::space, result);
	success = success && it == str.end();

	return success ? std::optional<Orientation>{result} : std::nullopt;
}

Orientation parse_orientation_or(const std::string_view str, const Orientation fallback) {
	const auto result = parse_orientation_optional(str);
	return result ? *result : fallback;
}

Orientation parse_orientation_or_throw(const std::string_view str) {
	const auto result = parse_orientation_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat('"', str, "\" is not a valid Vertical orientationment"));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
