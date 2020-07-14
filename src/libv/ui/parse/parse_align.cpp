// File:   Align.cpp Author: Vader Created on 2019. február 18., 4:42

// hpp
#include <libv/ui/parse/parse_align.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <algorithm>
#include <stdexcept>
// pro
#include <libv/ui/property/align.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

AlignVertical parse_align_vertical_or(const std::string_view str, const AlignVertical fallback) {
	const auto result = parse_align_vertical_optional(str);
	return result ? *result : fallback;
}

AlignVertical parse_align_vertical_or_throw(const std::string_view str) {
	const auto result = parse_align_vertical_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat('"', str, "\" is not a valid Vertical Alignment"));
}

std::optional<AlignVertical> parse_align_vertical_optional(const std::string_view str) {
	namespace x3 = boost::spirit::x3;

	struct named_aligns_ : x3::symbols<AlignVertical> {
		named_aligns_() {
			add
				("top", AlignVertical::top)
				("center", AlignVertical::center)
				("bottom", AlignVertical::bottom)
				("justifyall", AlignVertical::justify_all)
				("justify_all", AlignVertical::justify_all)
				("justify-all", AlignVertical::justify_all)
				("justify", AlignVertical::justify)

				("north", AlignVertical::top)
				("south", AlignVertical::bottom)
				("n", AlignVertical::top)
				("c", AlignVertical::center)
				("s", AlignVertical::bottom)
				("ja", AlignVertical::justify_all)
				("j", AlignVertical::justify)
			;
		}
	} named_aligns;

	AlignVertical result = AlignVertical::center;

	auto it = str.begin();
	auto success = x3::phrase_parse(it, str.end(), x3::no_case[named_aligns], x3::space, result);
	success = success && it == str.end();

	return success ? std::optional<AlignVertical>{result} : std::nullopt;
}

// -------------------------------------------------------------------------------------------------

AlignHorizontal parse_align_horizontal_or(const std::string_view str, const AlignHorizontal fallback) {
	const auto result = parse_align_horizontal_optional(str);
	return result ? *result : fallback;
}

AlignHorizontal parse_align_horizontal_or_throw(const std::string_view str) {
	const auto result = parse_align_horizontal_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat('"', str, "\" is not a valid Horizontal Alignment"));
}

std::optional<AlignHorizontal> parse_align_horizontal_optional(const std::string_view str) {
	namespace x3 = boost::spirit::x3;

	struct named_aligns_ : x3::symbols<AlignHorizontal> {
		named_aligns_() {
			add
				("left", AlignHorizontal::left)
				("center", AlignHorizontal::center)
				("right", AlignHorizontal::right)
				("justifyall", AlignHorizontal::justify_all)
				("justify-all", AlignHorizontal::justify_all)
				("justify_all", AlignHorizontal::justify_all)
				("justify", AlignHorizontal::justify)

				("west", AlignHorizontal::left)
				("east", AlignHorizontal::right)
				("w", AlignHorizontal::left)
				("c", AlignHorizontal::center)
				("e", AlignHorizontal::right)
				("ja", AlignHorizontal::justify_all)
				("j", AlignHorizontal::justify)
			;
		}
	} named_aligns;

	AlignHorizontal result = AlignHorizontal::center;

	auto it = str.begin();
	auto success = x3::phrase_parse(it, str.end(), x3::no_case[named_aligns], x3::space, result);
	success = success && it == str.end();

	return success ? std::optional<AlignHorizontal>{result} : std::nullopt;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
