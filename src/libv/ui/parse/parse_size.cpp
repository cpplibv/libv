// File: parser_size.cpp - Created on 2017.10.22. 06:09 - Author: Vader

// hpp
#include <libv/ui/parse/parse_size.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
// std
#include <stdexcept>
// pro
#include <libv/ui/property/size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

Size parse_size_or(const std::string_view str, const Size fallback) {
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

std::optional<Size> parse_size_optional(const std::string_view str) {
	namespace x3 = boost::spirit::x3;
	using SizeDim3 = std::tuple<SizeDim, SizeDim, SizeDim>;

	const auto map_pixel = [](auto& ctx) {
		if (std::not_equal_to<float>()(_val(ctx).pixel, 0.0f))
			_pass(ctx) = false;
		else
			_val(ctx).pixel = _attr(ctx);
	};
	const auto map_percent = [](auto& ctx) {
		if (std::not_equal_to<float>()(_val(ctx).percent, 0.0f))
			_pass(ctx) = false;
		else
			_val(ctx).percent = _attr(ctx);
	};
	const auto map_ratio = [](auto& ctx) {
		if (std::not_equal_to<float>()(_val(ctx).ratio, 0.0f))
			_pass(ctx) = false;
		else
			_val(ctx).ratio = _attr(ctx);
	};
	const auto map_dynamic = [](auto& ctx) {
		if (_val(ctx).dynamic)
			_pass(ctx) = false;
		else
			_val(ctx).dynamic = true;
	};

	const auto dimension = x3::rule<class dimension, SizeDim>{} =
			+(
				(x3::float_ >> x3::no_case[x3::lit("px")])[map_pixel] |
				(x3::float_ >> x3::no_case[x3::lit("%")])[map_percent] |
				(x3::float_ >> x3::no_case[x3::lit("r")])[map_ratio] |
				x3::no_case[x3::lit("dynamic")][map_dynamic] |
				x3::no_case[x3::lit("d")][map_dynamic] |
				x3::lit("0")
			);

	const auto size_rule = x3::rule<class size_rule, SizeDim3>{} =
			dimension >> x3::lit(',') >
			dimension >
			(x3::lit(',') >> dimension | x3::attr(ratio(1)));

	SizeDim3 result;
	std::get<0>(result).ratio = 0.0f;
	std::get<1>(result).ratio = 0.0f;
	std::get<2>(result).ratio = 0.0f;

	auto it = str.begin();
	auto success = x3::phrase_parse(it, str.end(), size_rule, x3::space, result);
	success = success && it == str.end();

	return success ? std::optional<Size>{
			Size{std::get<0>(result), std::get<1>(result), std::get<2>(result)}} : std::nullopt;

	// TODO P5: Would be nice to implement paring error diagnostics, x3 should support it
	//	https://www.boost.org/doc/libs/1_69_0/libs/spirit/doc/x3/html/spirit_x3/tutorials/annotation.html
	//	https://www.boost.org/doc/libs/1_69_0/libs/spirit/doc/x3/html/spirit_x3/tutorials/error_handling.html
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
