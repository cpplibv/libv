// Project: libv.parse, File: src/libv/ui/parse/parse_orientation.cpp, Author: Császár Mátyás [Vader]

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
				("bottom_to_top", Orientation::BOTTOM_TO_TOP)
				("left_to_right", Orientation::LEFT_TO_RIGHT)
				("right_to_left", Orientation::RIGHT_TO_LEFT)
				("top_to_bottom", Orientation::TOP_TO_BOTTOM)

				("bottom-to-top", Orientation::BOTTOM_TO_TOP)
				("left-to-right", Orientation::LEFT_TO_RIGHT)
				("right-to-left", Orientation::RIGHT_TO_LEFT)
				("top-to-bottom", Orientation::TOP_TO_BOTTOM)

				("bottom to top", Orientation::BOTTOM_TO_TOP)
				("left to right", Orientation::LEFT_TO_RIGHT)
				("right to left", Orientation::RIGHT_TO_LEFT)
				("top to bottom", Orientation::TOP_TO_BOTTOM)

				("up", Orientation::BOTTOM_TO_TOP)
				("right", Orientation::LEFT_TO_RIGHT)
				("left", Orientation::RIGHT_TO_LEFT)
				("down", Orientation::TOP_TO_BOTTOM)
				;
	}
} named_orientations_v;

} // namespace -------------------------------------------------------------------------------------

std::optional<Orientation> parse_orientation_optional(const std::string_view str) {
	Orientation result = Orientation::TOP_TO_BOTTOM;

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
