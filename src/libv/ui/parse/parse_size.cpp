// Project: libv.parse, File: src/libv/ui/parse/parse_size.cpp

// hpp
#include <libv/ui/parse/parse_size.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/char/unicode.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
// std
#include <stdexcept>
// pro
#include <libv/ui/property/size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

namespace x3 = boost::spirit::x3;

using SizeDim2 = std::tuple<SizeDim, SizeDim>;

const auto fa_pixel = [](auto& ctx) {
	if (std::not_equal_to<float>()(_val(ctx).pixel, 0.0f))
		_pass(ctx) = false;
	else
		_val(ctx).pixel = _attr(ctx);
};
const auto fa_percent = [](auto& ctx) {
	if (std::not_equal_to<float>()(_val(ctx).percent, 0.0f))
		_pass(ctx) = false;
	else
		_val(ctx).percent = _attr(ctx);
};
const auto fa_ratio = [](auto& ctx) {
	if (std::not_equal_to<float>()(_val(ctx).ratio, 0.0f))
		_pass(ctx) = false;
	else
		_val(ctx).ratio = _attr(ctx);
};
const auto fa_dynamic = [](auto& ctx) {
	if (_val(ctx).dynamic)
		_pass(ctx) = false;
	else
		_val(ctx).dynamic = true;
};
const auto fa_min = [](auto& ctx) {
	if (_val(ctx).mode == SizeDim::Mode::min)
		_pass(ctx) = false;
	else
		_val(ctx).mode = SizeDim::Mode::min;
};

const auto dimension = x3::rule<class dimension_, SizeDim>{} =
		+(
			(x3::float_ >> x3::no_case[x3::lit("px")])[fa_pixel] |
			(x3::float_ >> x3::no_case[x3::lit("%")])[fa_percent] |
			(x3::float_ >> x3::no_case[x3::lit("r")])[fa_ratio] |
			x3::no_case[x3::lit("dynamic")][fa_dynamic] |
			x3::no_case[x3::lit("d")][fa_dynamic] |
			x3::no_case[x3::lit("min")][fa_min] |
			x3::lit("0")
		);

const auto size_rule = x3::rule<class size_rule_, SizeDim2>{} =
		dimension >> x3::lit(',') >> dimension;

} // namespace -------------------------------------------------------------------------------------

std::optional<Size> parse_size_optional(const std::string_view str) {
	SizeDim2 result;
	std::get<0>(result).ratio = 0.0f;
	std::get<1>(result).ratio = 0.0f;

	auto it = str.begin();
	// NOTE: x3::unicode::space is required for certain invalid UTF8 inputs as x3::space skipper would assert
	auto success = x3::phrase_parse(it, str.end(), size_rule, x3::unicode::space, result);
	success = success && it == str.end();

	return success ? std::optional<Size>{Size{std::get<0>(result), std::get<1>(result)}} : std::nullopt;

	// TODO P5: Would be nice to implement paring error diagnostics, x3 should support it
	//	https://www.boost.org/doc/libs/1_69_0/libs/spirit/doc/x3/html/spirit_x3/tutorials/annotation.html
	//	https://www.boost.org/doc/libs/1_69_0/libs/spirit/doc/x3/html/spirit_x3/tutorials/error_handling.html
}

Size parse_size_or(const std::string_view str, const Size& fallback) {
	const auto result = parse_size_optional(str);
	return result ? *result : fallback;
}

Size parse_size_or_throw(const std::string_view str) {
	const auto result = parse_size_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(std::string(str) + " is not a valid size");
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
