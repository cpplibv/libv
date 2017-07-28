// File: parser_size.cpp - Created on 2017.10.22. 06:09 - Author: Vader

// hpp
#include <libv/ui/parse/parse_size.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
// pro
#include <libv/ui/size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::optional<Size> parse_size(const std::string_view str) {
	namespace x3 = boost::spirit::x3;
//	using float4 = std::tuple<float, float, float>;

	(void) str;
//	const auto translate_rgb = [](auto& ctx) {
//		_val(ctx).x = std::get<0>(_attr(ctx));
//		_val(ctx).y = std::get<1>(_attr(ctx));
//		_val(ctx).z = std::get<2>(_attr(ctx));
//	};
//
//	const auto identity = [](auto& ctx) {
//		_val(ctx) = _attr(ctx);
//	};
//
//	const auto dimension = x3::rule<class dimension, float>{} =
//			x3::float_;
////			x3::no_case[x3::lit("hsva")];
////			x3::uint_parser<uint8_t, 16, 1, 1>{}[scale(15)];
//
//	const auto size_rule = x3::rule<class size_rule, float4>{} =
//			dimension >> x3::lit(',') >>
//			dimension >>
//			-(x3::lit(',') >> dimension);
//
////	color color(1.f, 1.f, 1.f, 1.f);
//
//	phrase_parse(str.begin(), str.end(), size_rule, x3::space, color);

//	return ui::Size{};
	return {};
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
