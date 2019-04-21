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
				("top", AlignVertical::Top)
				("center", AlignVertical::Center)
				("bottom", AlignVertical::Bottom)
				("justifyall", AlignVertical::JustifyAll)
				("justify_all", AlignVertical::JustifyAll)
				("justify-all", AlignVertical::JustifyAll)
				("justify", AlignVertical::Justify)

				("north", AlignVertical::Top)
				("south", AlignVertical::Bottom)
				("n", AlignVertical::Top)
				("c", AlignVertical::Center)
				("s", AlignVertical::Bottom)
				("ja", AlignVertical::JustifyAll)
				("j", AlignVertical::Justify)
			;
		}
	} named_aligns;

	AlignVertical result = AlignVertical::Center;

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
				("left", AlignHorizontal::Left)
				("center", AlignHorizontal::Center)
				("right", AlignHorizontal::Right)
				("justifyall", AlignHorizontal::JustifyAll)
				("justify-all", AlignHorizontal::JustifyAll)
				("justify_all", AlignHorizontal::JustifyAll)
				("justify", AlignHorizontal::Justify)

				("west", AlignHorizontal::Left)
				("east", AlignHorizontal::Right)
				("w", AlignHorizontal::Left)
				("c", AlignHorizontal::Center)
				("e", AlignHorizontal::Right)
				("ja", AlignHorizontal::JustifyAll)
				("j", AlignHorizontal::Justify)
			;
		}
	} named_aligns;

	AlignHorizontal result = AlignHorizontal::Center;

	auto it = str.begin();
	auto success = x3::phrase_parse(it, str.end(), x3::no_case[named_aligns], x3::space, result);
	success = success && it == str.end();

	return success ? std::optional<AlignHorizontal>{result} : std::nullopt;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
