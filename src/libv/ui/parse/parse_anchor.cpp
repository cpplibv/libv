// Project: libv.parse, File: src/libv/ui/parse/parse_anchor.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/parse/parse_anchor.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/char/unicode.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <algorithm>
#include <stdexcept>
// pro
#include <libv/ui/property/anchor.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

namespace x3 = boost::spirit::x3;

struct named_anchors_v_ : x3::symbols<Anchor> {
	named_anchors_v_() {
		add
				("bottom_left", Anchor::bottom_left)
				("bottom-left", Anchor::bottom_left)
				("bottom left", Anchor::bottom_left)
				("bottomleft", Anchor::bottom_left)
				("bottom_center", Anchor::bottom_center)
				("bottom-center", Anchor::bottom_center)
				("bottom center", Anchor::bottom_center)
				("bottomcenter", Anchor::bottom_center)
				("bottom_right", Anchor::bottom_right)
				("bottom-right", Anchor::bottom_right)
				("bottom right", Anchor::bottom_right)
				("bottomright", Anchor::bottom_right)
				("center_left", Anchor::center_left)
				("center-left", Anchor::center_left)
				("center left", Anchor::center_left)
				("centerleft", Anchor::center_left)
				("center_center", Anchor::center_center)
				("center-center", Anchor::center_center)
				("center center", Anchor::center_center)
				("centercenter", Anchor::center_center)
				("center_right", Anchor::center_right)
				("center-right", Anchor::center_right)
				("center right", Anchor::center_right)
				("centerright", Anchor::center_right)
				("top_left", Anchor::top_left)
				("top-left", Anchor::top_left)
				("top left", Anchor::top_left)
				("topleft", Anchor::top_left)
				("top_center", Anchor::top_center)
				("top-center", Anchor::top_center)
				("top center", Anchor::top_center)
				("topcenter", Anchor::top_center)
				("top_right", Anchor::top_right)
				("top-right", Anchor::top_right)
				("top right", Anchor::top_right)
				("topright", Anchor::top_right)

				("bottom", Anchor::bottom_center)
				("down", Anchor::bottom_center)
				("left", Anchor::center_left)
				("right", Anchor::center_right)
				("up", Anchor::top_center)
				("top", Anchor::top_center)

				("bl", Anchor::bottom_left)
				("bc", Anchor::bottom_center)
				("br", Anchor::bottom_right)
				("cl", Anchor::center_left)
				("cc", Anchor::center_center)
				("cr", Anchor::center_right)
				("tl", Anchor::top_left)
				("tc", Anchor::top_center)
				("tr", Anchor::top_right)

				("c", Anchor::center_center)

				("sw", Anchor::bottom_left)
				("s", Anchor::bottom_center)
				("se", Anchor::bottom_right)
				("w", Anchor::center_left)
				// ("", Anchor::center_center)
				("e", Anchor::center_right)
				("nw", Anchor::top_left)
				("n", Anchor::top_center)
				("ne", Anchor::top_right)

				("south", Anchor::bottom_center)
				("west", Anchor::center_left)
				("east", Anchor::center_right)
				("north", Anchor::top_center)
				;
	}
} named_anchors_v;

} // namespace -------------------------------------------------------------------------------------

std::optional<Anchor> parse_anchor_optional(const std::string_view str) {
	Anchor result = Anchor::center_center;

	auto it = str.begin();
	// NOTE: x3::unicode::space is required for certain invalid UTF8 inputs as x3::space skipper would assert
	auto success = x3::phrase_parse(it, str.end(), x3::no_case[named_anchors_v], x3::unicode::space, result);
	success = success && it == str.end();

	return success ? std::optional<Anchor>{result} : std::nullopt;
}

Anchor parse_anchor_or(const std::string_view str, const Anchor fallback) {
	const auto result = parse_anchor_optional(str);
	return result ? *result : fallback;
}

Anchor parse_anchor_or_throw(const std::string_view str) {
	const auto result = parse_anchor_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat('"', str, "\" is not a valid Vertical anchorment"));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
