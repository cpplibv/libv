// File:   color.cpp Author: Vader Created on 2019. február 18., 4:42

// hpp
#include <libv/parse/color.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <stdexcept>


namespace libv {
namespace parse {

// -------------------------------------------------------------------------------------------------

libv::vec4f parse_color_or(const std::string_view str, const libv::vec4f fallback) {
	const auto result = parse_color_optional(str);
	return result ? *result : fallback;
}

libv::vec4f parse_color_or_throw(const std::string_view str) {
	const auto result = parse_color_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat("\"", str, "\" is not a valid color"));
}

std::optional<libv::vec4f> parse_color_optional(const std::string_view str) {
	namespace x3 = boost::spirit::x3;
	using float4 = std::tuple<float, float, float, float>;
	using color = libv::vec4f;

	struct named_colors_ : x3::symbols<color> {
		named_colors_() {
			add
				("aliceblue", color(0.9412f, 0.9725f, 1.0000f, 1.0000f))
				("antiquewhite", color(0.9804f, 0.9216f, 0.8431f, 1.0000f))
				("aqua", color(0.0000f, 1.0000f, 1.0000f, 1.0000f))
				("aquamarine", color(0.4980f, 1.0000f, 0.8314f, 1.0000f))
				("azure", color(0.9412f, 1.0000f, 1.0000f, 1.0000f))
				// <editor-fold defaultstate="collapsed" desc="...">
				("beige", color(0.9608f, 0.9608f, 0.8627f, 1.0000f))
				("bisque", color(1.0000f, 0.8941f, 0.7686f, 1.0000f))
				("black", color(0.0000f, 0.0000f, 0.0000f, 1.0000f))
				("blanchedalmond", color(1.0000f, 0.9216f, 0.8039f, 1.0000f))
				("blue", color(0.0000f, 0.0000f, 1.0000f, 1.0000f))
				("blueviolet", color(0.5412f, 0.1686f, 0.8863f, 1.0000f))
				("brown", color(0.6471f, 0.1647f, 0.1647f, 1.0000f))
				("burlywood", color(0.8706f, 0.7216f, 0.5294f, 1.0000f))
				("cadetblue", color(0.3725f, 0.6196f, 0.6275f, 1.0000f))
				("chartreuse", color(0.4980f, 1.0000f, 0.0000f, 1.0000f))
				("chocolate", color(0.8235f, 0.4118f, 0.1176f, 1.0000f))
				("coral", color(1.0000f, 0.4980f, 0.3137f, 1.0000f))
				("cornflowerblue", color(0.3922f, 0.5843f, 0.9294f, 1.0000f))
				("cornsilk", color(1.0000f, 0.9725f, 0.8627f, 1.0000f))
				("crimson", color(0.8627f, 0.0784f, 0.2353f, 1.0000f))
				("cyan", color(0.0000f, 1.0000f, 1.0000f, 1.0000f))
				("darkblue", color(0.0000f, 0.0000f, 0.5451f, 1.0000f))
				("darkcyan", color(0.0000f, 0.5451f, 0.5451f, 1.0000f))
				("darkgoldenrod", color(0.7216f, 0.5255f, 0.0431f, 1.0000f))
				("darkgray", color(0.6627f, 0.6627f, 0.6627f, 1.0000f))
				("darkgreen", color(0.0000f, 0.3922f, 0.0000f, 1.0000f))
				("darkgrey", color(0.6627f, 0.6627f, 0.6627f, 1.0000f))
				("darkkhaki", color(0.7412f, 0.7176f, 0.4196f, 1.0000f))
				("darkmagenta", color(0.5451f, 0.0000f, 0.5451f, 1.0000f))
				("darkolivegreen", color(0.3333f, 0.4196f, 0.1843f, 1.0000f))
				("darkorange", color(1.0000f, 0.5490f, 0.0000f, 1.0000f))
				("darkorchid", color(0.6000f, 0.1961f, 0.8000f, 1.0000f))
				("darkred", color(0.5451f, 0.0000f, 0.0000f, 1.0000f))
				("darksalmon", color(0.9137f, 0.5882f, 0.4784f, 1.0000f))
				("darkseagreen", color(0.5608f, 0.7373f, 0.5608f, 1.0000f))
				("darkslateblue", color(0.2824f, 0.2392f, 0.5451f, 1.0000f))
				("darkslategrey", color(0.1843f, 0.3098f, 0.3098f, 1.0000f))
				("darkturquoise", color(0.0000f, 0.8078f, 0.8196f, 1.0000f))
				("darkviolet", color(0.5804f, 0.0000f, 0.8275f, 1.0000f))
				("deeppink", color(1.0000f, 0.0784f, 0.5765f, 1.0000f))
				("deepskyblue", color(0.0000f, 0.7490f, 1.0000f, 1.0000f))
				("dimgray", color(0.4118f, 0.4118f, 0.4118f, 1.0000f))
				("dimgrey", color(0.4118f, 0.4118f, 0.4118f, 1.0000f))
				("dodgerblue", color(0.1176f, 0.5647f, 1.0000f, 1.0000f))
				("firebrick", color(0.6980f, 0.1333f, 0.1333f, 1.0000f))
				("floralwhite", color(1.0000f, 0.9804f, 0.9412f, 1.0000f))
				("forestgreen", color(0.1333f, 0.5451f, 0.1333f, 1.0000f))
				("fuchsia", color(1.0000f, 0.0000f, 1.0000f, 1.0000f))
				("gainsboro", color(0.8627f, 0.8627f, 0.8627f, 1.0000f))
				("ghostwhite", color(0.9725f, 0.9725f, 1.0000f, 1.0000f))
				("gold", color(1.0000f, 0.8431f, 0.0000f, 1.0000f))
				("goldenrod", color(0.8549f, 0.6471f, 0.1255f, 1.0000f))
				("gray", color(0.5020f, 0.5020f, 0.5020f, 1.0000f))
				("grey", color(0.5020f, 0.5020f, 0.5020f, 1.0000f))
				("green", color(0.0000f, 0.5020f, 0.0000f, 1.0000f))
				("greenyellow", color(0.6784f, 1.0000f, 0.1843f, 1.0000f))
				("honeydew", color(0.9412f, 1.0000f, 0.9412f, 1.0000f))
				("hotpink", color(1.0000f, 0.4118f, 0.7059f, 1.0000f))
				("indianred", color(0.8039f, 0.3608f, 0.3608f, 1.0000f))
				("indigo", color(0.2941f, 0.0000f, 0.5098f, 1.0000f))
				("ivory", color(1.0000f, 1.0000f, 0.9412f, 1.0000f))
				("khaki", color(0.9412f, 0.9020f, 0.5490f, 1.0000f))
				("lavender", color(0.9020f, 0.9020f, 0.9804f, 1.0000f))
				("lavenderblush", color(1.0000f, 0.9412f, 0.9608f, 1.0000f))
				("lawngreen", color(0.4863f, 0.9882f, 0.0000f, 1.0000f))
				("lemonchiffon", color(1.0000f, 0.9804f, 0.8039f, 1.0000f))
				("lightblue", color(0.6784f, 0.8471f, 0.9020f, 1.0000f))
				("lightcoral", color(0.9412f, 0.5020f, 0.5020f, 1.0000f))
				("lightcyan", color(0.8784f, 1.0000f, 1.0000f, 1.0000f))
				("lightgoldenrodyellow", color(0.9804f, 0.9804f, 0.8235f, 1.0000f))
				("lightgray", color(0.8275f, 0.8275f, 0.8275f, 1.0000f))
				("lightgreen", color(0.5647f, 0.9333f, 0.5647f, 1.0000f))
				("lightgrey", color(0.8275f, 0.8275f, 0.8275f, 1.0000f))
				("lightpink", color(1.0000f, 0.7137f, 0.7569f, 1.0000f))
				("lightsalmon", color(1.0000f, 0.6275f, 0.4784f, 1.0000f))
				("lightseagreen", color(0.1255f, 0.6980f, 0.6667f, 1.0000f))
				("lightskyblue", color(0.5294f, 0.8078f, 0.9804f, 1.0000f))
				("lightslategray", color(0.4667f, 0.5333f, 0.6000f, 1.0000f))
				("lightslategrey", color(0.4667f, 0.5333f, 0.6000f, 1.0000f))
				("lightsteelblue", color(0.6902f, 0.7686f, 0.8706f, 1.0000f))
				("lightyellow", color(1.0000f, 1.0000f, 0.8784f, 1.0000f))
				("lime", color(0.0000f, 1.0000f, 0.0000f, 1.0000f))
				("limegreen", color(0.1961f, 0.8039f, 0.1961f, 1.0000f))
				("linen", color(0.9804f, 0.9412f, 0.9020f, 1.0000f))
				("magenta", color(1.0000f, 0.0000f, 1.0000f, 1.0000f))
				("maroon", color(0.5020f, 0.0000f, 0.0000f, 1.0000f))
				("mediumaquamarine", color(0.4000f, 0.8039f, 0.6667f, 1.0000f))
				("mediumblue", color(0.0000f, 0.0000f, 0.8039f, 1.0000f))
				("mediumorchid", color(0.7294f, 0.3333f, 0.8275f, 1.0000f))
				("mediumpurple", color(0.5765f, 0.4392f, 0.8588f, 1.0000f))
				("mediumseagreen", color(0.2353f, 0.7020f, 0.4431f, 1.0000f))
				("mediumslateblue", color(0.4824f, 0.4078f, 0.9333f, 1.0000f))
				("mediumspringgreen", color(0.0000f, 0.9804f, 0.6039f, 1.0000f))
				("mediumturquoise", color(0.2824f, 0.8196f, 0.8000f, 1.0000f))
				("mediumvioletred", color(0.7804f, 0.0824f, 0.5216f, 1.0000f))
				("midnightblue", color(0.0980f, 0.0980f, 0.4392f, 1.0000f))
				("mintcream", color(0.9608f, 1.0000f, 0.9804f, 1.0000f))
				("mistyrose", color(1.0000f, 0.8941f, 0.8824f, 1.0000f))
				("moccasin", color(1.0000f, 0.8941f, 0.7098f, 1.0000f))
				("navajowhite", color(1.0000f, 0.8706f, 0.6784f, 1.0000f))
				("navy", color(0.0000f, 0.0000f, 0.5020f, 1.0000f))
				("oldlace", color(0.9922f, 0.9608f, 0.9020f, 1.0000f))
				("olive", color(0.5020f, 0.5020f, 0.0000f, 1.0000f))
				("olivedrab", color(0.4196f, 0.5569f, 0.1373f, 1.0000f))
				("orange", color(1.0000f, 0.6471f, 0.0000f, 1.0000f))
				("orangered", color(1.0000f, 0.2706f, 0.0000f, 1.0000f))
				("orchid", color(0.8549f, 0.4392f, 0.8392f, 1.0000f))
				("palegoldenrod", color(0.9333f, 0.9098f, 0.6667f, 1.0000f))
				("palegreen", color(0.5961f, 0.9843f, 0.5961f, 1.0000f))
				("paleturquoise", color(0.6863f, 0.9333f, 0.9333f, 1.0000f))
				("palevioletred", color(0.8588f, 0.4392f, 0.5765f, 1.0000f))
				("papayawhip", color(1.0000f, 0.9373f, 0.8353f, 1.0000f))
				("peachpuff", color(1.0000f, 0.8549f, 0.7255f, 1.0000f))
				("peru", color(0.8039f, 0.5216f, 0.2471f, 1.0000f))
				("pink", color(1.0000f, 0.7529f, 0.7961f, 1.0000f))
				("plum", color(0.8667f, 0.6275f, 0.8667f, 1.0000f))
				("powderblue", color(0.6902f, 0.8784f, 0.9020f, 1.0000f))
				("purple", color(0.5020f, 0.0000f, 0.5020f, 1.0000f))
				("red", color(1.0000f, 0.0000f, 0.0000f, 1.0000f))
				("rosybrown", color(0.7373f, 0.5608f, 0.5608f, 1.0000f))
				("royalblue", color(0.2549f, 0.4118f, 0.8824f, 1.0000f))
				("saddlebrown", color(0.5451f, 0.2706f, 0.0745f, 1.0000f))
				("salmon", color(0.9804f, 0.5020f, 0.4471f, 1.0000f))
				("sandybrown", color(0.9569f, 0.6431f, 0.3765f, 1.0000f))
				("seagreen", color(0.1804f, 0.5451f, 0.3412f, 1.0000f))
				("seashell", color(1.0000f, 0.9608f, 0.9333f, 1.0000f))
				("sienna", color(0.6275f, 0.3216f, 0.1765f, 1.0000f))
				("silver", color(0.7529f, 0.7529f, 0.7529f, 1.0000f))
				("skyblue", color(0.5294f, 0.8078f, 0.9216f, 1.0000f))
				("slateblue", color(0.4157f, 0.3529f, 0.8039f, 1.0000f))
				("slategray", color(0.4392f, 0.5020f, 0.5647f, 1.0000f))
				("slategrey", color(0.4392f, 0.5020f, 0.5647f, 1.0000f))
				("snow", color(1.0000f, 0.9804f, 0.9804f, 1.0000f))
				("springgreen", color(0.0000f, 1.0000f, 0.4980f, 1.0000f))
				("steelblue", color(0.2745f, 0.5098f, 0.7059f, 1.0000f))
				("tan", color(0.8235f, 0.7059f, 0.5490f, 1.0000f))
				("teal", color(0.0000f, 0.5020f, 0.5020f, 1.0000f))
				("thistle", color(0.8471f, 0.7490f, 0.8471f, 1.0000f))
				("tomato", color(1.0000f, 0.3882f, 0.2784f, 1.0000f))
				("turquoise", color(0.2510f, 0.8784f, 0.8157f, 1.0000f))
				("violet", color(0.9333f, 0.5098f, 0.9333f, 1.0000f))
				("wheat", color(0.9608f, 0.8706f, 0.7020f, 1.0000f))
				("white", color(1.0000f, 1.0000f, 1.0000f, 1.0000f))
				("whitesmoke", color(0.9608f, 0.9608f, 0.9608f, 1.0000f))
				("yellow", color(1.0000f, 1.0000f, 0.0000f, 1.0000f))
				("yellowgreen", color(0.6039f, 0.8039f, 0.1961f, 1.0000f))
				("transparent", color(0.0000f, 0.0000f, 0.0000f, 0.0000f))
				// </editor-fold>
			;
		}
	} named_colors;

	// ---------------------------------------------------------------------------------------------

	const auto translate_rgb = [](auto& ctx) {
		_val(ctx).x = std::get<0>(_attr(ctx));
		_val(ctx).y = std::get<1>(_attr(ctx));
		_val(ctx).z = std::get<2>(_attr(ctx));
		_val(ctx).w = std::get<3>(_attr(ctx));
	};

	const auto translate_hsl = [](auto& ctx) {
		auto h = std::fmod(std::get<0>(_attr(ctx)), 360.f);
		if (h < 0)
			h += 360.f;
		auto s = std::get<1>(_attr(ctx));
		auto l = std::get<2>(_attr(ctx));

		auto c = (1 - std::fabs(2 * l - 1)) * s;
		auto x = c * (1 - std::fabs(std::fmod(h / 60.f, 2.f) - 1));
		auto m = l - c * .5f;

		if (h < 60.f) {
			_val(ctx).x = m + c;
			_val(ctx).y = m + x;
			_val(ctx).z = m;
		} else if (h < 120.f) {
			_val(ctx).x = m + x;
			_val(ctx).y = m + c;
			_val(ctx).z = m;
		} else if (h < 180.f) {
			_val(ctx).x = m;
			_val(ctx).y = m + c;
			_val(ctx).z = m + x;
		} else if (h < 240.f) {
			_val(ctx).x = m;
			_val(ctx).y = m + x;
			_val(ctx).z = m + c;
		} else if (h < 300.f) {
			_val(ctx).x = m + x;
			_val(ctx).y = m;
			_val(ctx).z = m + c;
		} else {
			_val(ctx).x = m + c;
			_val(ctx).y = m;
			_val(ctx).z = m + x;
		}
		_val(ctx).w = std::get<3>(_attr(ctx));
	};

	const auto translate_hsv = [](auto& ctx) {
		auto h = std::fmod(std::get<0>(_attr(ctx)), 360.f);
		if (h < 0)
			h += 360.f;
		auto s = std::get<1>(_attr(ctx));
		auto v = std::get<2>(_attr(ctx));

		auto c = v * s;
		auto x = c * (1 - std::fabs(std::fmod(h / 60.f, 2.f) - 1));
		auto m = v - c;

		if (h < 60.f) {
			_val(ctx).x = m + c;
			_val(ctx).y = m + x;
			_val(ctx).z = m;
		} else if (h < 120.f) {
			_val(ctx).x = m + x;
			_val(ctx).y = m + c;
			_val(ctx).z = m;
		} else if (h < 180.f) {
			_val(ctx).x = m;
			_val(ctx).y = m + c;
			_val(ctx).z = m + x;
		} else if (h < 240.f) {
			_val(ctx).x = m;
			_val(ctx).y = m + x;
			_val(ctx).z = m + c;
		} else if (h < 300.f) {
			_val(ctx).x = m + x;
			_val(ctx).y = m;
			_val(ctx).z = m + c;
		} else {
			_val(ctx).x = m + c;
			_val(ctx).y = m;
			_val(ctx).z = m + x;
		}
		_val(ctx).w = std::get<3>(_attr(ctx));
	};

	// ---------------------------------------------------------------------------------------------

	const auto identity = [](auto& ctx) {
		_val(ctx) = _attr(ctx);
	};

	const auto scale = [](float value) {
		return [value](auto& ctx) {
			if (_attr(ctx) > value)
				_pass(ctx) = false;
			else
				_val(ctx) = _attr(ctx) * (1.0f / value);
		};
	};

	// ---------------------------------------------------------------------------------------------

	const auto hex1_channel = x3::rule<class hex1_channel_, float>{} =
			x3::uint_parser<uint8_t, 16, 1, 1>{}[scale(15)];

	const auto hex2_channel = x3::rule<class hex2_channel_, float>{} =
			x3::uint_parser<uint8_t, 16, 2, 2>{}[scale(255)];

	const auto rgb_channel = x3::rule<class rgb_channel_, float>{} =
			(x3::real_parser<float, x3::ureal_policies<float>>{} >> x3::lit('%'))[scale(100)] |
			(x3::real_parser<float, x3::strict_ureal_policies<float>>{})[scale(1)] |
			(x3::uint_parser<uint8_t, 10, 1, 3>{})[scale(255)];

	const auto hue_channel = x3::rule<class hue_channel_, float>{} =
			x3::float_ >> -x3::lit("°");

	// ---------------------------------------------------------------------------------------------

	const auto hex1_color = x3::rule<class hex1_color_, float4>{} = x3::lit('#') >>
			hex1_channel >>
			hex1_channel >>
			hex1_channel >>
			(hex1_channel | x3::attr(1.0f));

	const auto hex2_color = x3::rule<class hex2_color_, float4>{} = x3::lit('#') >>
			hex2_channel >>
			hex2_channel >>
			hex2_channel >>
			(hex2_channel | x3::attr(1.0f));

	const auto rgb_color = x3::rule<class rgb_color_, float4>{} =
			x3::no_case[x3::lit("rgb")] >>
			x3::lit('(') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			(x3::lit(',') >> rgb_channel | x3::attr(1.0f)) >> x3::lit(')');

	const auto rgba_color = x3::rule<class rgba_color_, float4>{} =
			x3::no_case[x3::lit("rgba")] >>
			x3::lit('(') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >> x3::lit(')');

	const auto hsl_color = x3::rule<class hsl_color_, float4>{} =
			x3::no_case[x3::lit("hsl")] >>
			x3::lit('(') >> hue_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			(x3::lit(',') >> rgb_channel | x3::attr(1.0f)) >> x3::lit(')');

	const auto hsla_color = x3::rule<class hsla_color_, float4>{} =
			x3::no_case[x3::lit("hsla")] >>
			x3::lit('(') >> hue_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >> x3::lit(')');

	const auto hsv_color = x3::rule<class hsv_color_, float4>{} =
			x3::no_case[x3::lit("hsv")] >>
			x3::lit('(') >> hue_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			(x3::lit(',') >> rgb_channel | x3::attr(1.0f)) >> x3::lit(')');

	const auto hsva_color = x3::rule<class hsva_color_, float4>{} =
			x3::no_case[x3::lit("hsva")] >>
			x3::lit('(') >> hue_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >> x3::lit(')');

	const auto color_rule = x3::rule<class color_rule_, color>{} =
			x3::no_case[named_colors][identity] |
			hex2_color[translate_rgb] |
			hex1_color[translate_rgb] |
			rgb_color[translate_rgb] |
			rgba_color[translate_rgb] |
			hsl_color[translate_hsl] |
			hsla_color[translate_hsl] |
			hsv_color[translate_hsv] |
			hsva_color[translate_hsv];

	color result(1.f, 1.f, 1.f, 1.f);

	auto it = str.begin();
	auto success = x3::phrase_parse(it, str.end(), color_rule, x3::space, result);
	success = success && it == str.end();

	return success ? std::optional<color>{result} : std::nullopt;
}

// -------------------------------------------------------------------------------------------------

} // namespace parse
} // namespace libv
