// Project: libv.parse, File: src/libv/ui/parse/parse_scroll_mode.cpp

// hpp
#include <libv/ui/parse/parse_scroll_mode.hpp>
// libv
#include <libv/utility/concat.hpp>
#include <libv/algo/trim.hpp>
// std
#include <stdexcept>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::optional<ScrollMode> parse_scroll_mode_optional(std::string_view str) {
	str = libv::trim(str);

	for (const auto& value : ScrollMode_state::values())
		if (to_string(value) == str)
			return value;

	return std::nullopt;
}

ScrollMode parse_scroll_mode_or(const std::string_view str, const ScrollMode fallback) {
	const auto result = parse_scroll_mode_optional(str);
	return result ? *result : fallback;
}

ScrollMode parse_scroll_mode_or_throw(const std::string_view str) {
	const auto result = parse_scroll_mode_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat('"', str, "\" is not a valid scroll_mode"));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
