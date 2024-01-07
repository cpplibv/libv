// Project: libv.parse, File: src/libv/ui/parse/parse_align.cpp

// hpp
#include <libv/ui/parse/parse_focus_mode.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/char/unicode.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <algorithm>
#include <stdexcept>
// pro
#include <libv/ui/property/focus_mode.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

namespace x3 = boost::spirit::x3;

struct named_focus_mode_ : x3::symbols<FocusMode> {
	named_focus_mode_() {
		add
				("active", FocusMode::active)
				("passive", FocusMode::passive)
				("inactive", FocusMode::inactive)
				;
	}
} named_focus_mode;

} // namespace -------------------------------------------------------------------------------------

std::optional<FocusMode> parse_focus_mode_optional(const std::string_view str) {
	FocusMode result = FocusMode::active;

	auto it = str.begin();
	// NOTE: x3::unicode::space is required for certain invalid UTF8 inputs as x3::space skipper would assert
	auto success = x3::phrase_parse(it, str.end(), x3::no_case[named_focus_mode], x3::unicode::space, result);
	success = success && it == str.end();

	return success ? std::optional<FocusMode>{result} : std::nullopt;
}

FocusMode parse_focus_mode_or(const std::string_view str, const FocusMode fallback) {
	const auto result = parse_focus_mode_optional(str);
	return result ? *result : fallback;
}

FocusMode parse_focus_mode_or_throw(const std::string_view str) {
	const auto result = parse_focus_mode_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat('"', str, "\" is not a valid Vertical Alignment"));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
