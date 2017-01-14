// File: SignalTestUtil.hpp, Created on 2015. január 26. 17:15, Author: Vader

#pragma once

// ext
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
// libv
#include <libv/vec.hpp>
// std
#include <string>

#include <iostream>

namespace libv {
namespace parse {

// -------------------------------------------------------------------------------------------------

using color = libv::fvec4;

color to_color(const std::string& str) {

	namespace x3 = boost::spirit::x3;
	using float4 = std::tuple<float, float, float, float>;

	struct named_colors_ : x3::symbols<color> {
		named_colors_() {
			add
				("aliceblue", color(0.9412, 0.9725, 1.0000, 1.0000))
				("antiquewhite", color(0.9804, 0.9216, 0.8431, 1.0000))
				("aqua", color(0.0000, 1.0000, 1.0000, 1.0000))
				("aquamarine", color(0.4980, 1.0000, 0.8314, 1.0000))
				("azure", color(0.9412, 1.0000, 1.0000, 1.0000))
				// <editor-fold defaultstate="collapsed" desc="...">
				("beige", color(0.9608, 0.9608, 0.8627, 1.0000))
				("bisque", color(1.0000, 0.8941, 0.7686, 1.0000))
				("black", color(0.0000, 0.0000, 0.0000, 1.0000))
				("blanchedalmond", color(1.0000, 0.9216, 0.8039, 1.0000))
				("blue", color(0.0000, 0.0000, 1.0000, 1.0000))
				("blueviolet", color(0.5412, 0.1686, 0.8863, 1.0000))
				("brown", color(0.6471, 0.1647, 0.1647, 1.0000))
				("burlywood", color(0.8706, 0.7216, 0.5294, 1.0000))
				("cadetblue", color(0.3725, 0.6196, 0.6275, 1.0000))
				("chartreuse", color(0.4980, 1.0000, 0.0000, 1.0000))
				("chocolate", color(0.8235, 0.4118, 0.1176, 1.0000))
				("coral", color(1.0000, 0.4980, 0.3137, 1.0000))
				("cornflowerblue", color(0.3922, 0.5843, 0.9294, 1.0000))
				("cornsilk", color(1.0000, 0.9725, 0.8627, 1.0000))
				("crimson", color(0.8627, 0.0784, 0.2353, 1.0000))
				("cyan", color(0.0000, 1.0000, 1.0000, 1.0000))
				("darkblue", color(0.0000, 0.0000, 0.5451, 1.0000))
				("darkcyan", color(0.0000, 0.5451, 0.5451, 1.0000))
				("darkgoldenrod", color(0.7216, 0.5255, 0.0431, 1.0000))
				("darkgray", color(0.6627, 0.6627, 0.6627, 1.0000))
				("darkgreen", color(0.0000, 0.3922, 0.0000, 1.0000))
				("darkgrey", color(0.6627, 0.6627, 0.6627, 1.0000))
				("darkkhaki", color(0.7412, 0.7176, 0.4196, 1.0000))
				("darkmagenta", color(0.5451, 0.0000, 0.5451, 1.0000))
				("darkolivegreen", color(0.3333, 0.4196, 0.1843, 1.0000))
				("darkorange", color(1.0000, 0.5490, 0.0000, 1.0000))
				("darkorchid", color(0.6000, 0.1961, 0.8000, 1.0000))
				("darkred", color(0.5451, 0.0000, 0.0000, 1.0000))
				("darksalmon", color(0.9137, 0.5882, 0.4784, 1.0000))
				("darkseagreen", color(0.5608, 0.7373, 0.5608, 1.0000))
				("darkslateblue", color(0.2824, 0.2392, 0.5451, 1.0000))
				("darkslategrey", color(0.1843, 0.3098, 0.3098, 1.0000))
				("darkturquoise", color(0.0000, 0.8078, 0.8196, 1.0000))
				("darkviolet", color(0.5804, 0.0000, 0.8275, 1.0000))
				("deeppink", color(1.0000, 0.0784, 0.5765, 1.0000))
				("deepskyblue", color(0.0000, 0.7490, 1.0000, 1.0000))
				("dimgray", color(0.4118, 0.4118, 0.4118, 1.0000))
				("dimgrey", color(0.4118, 0.4118, 0.4118, 1.0000))
				("dodgerblue", color(0.1176, 0.5647, 1.0000, 1.0000))
				("firebrick", color(0.6980, 0.1333, 0.1333, 1.0000))
				("floralwhite", color(1.0000, 0.9804, 0.9412, 1.0000))
				("forestgreen", color(0.1333, 0.5451, 0.1333, 1.0000))
				("fuchsia", color(1.0000, 0.0000, 1.0000, 1.0000))
				("gainsboro", color(0.8627, 0.8627, 0.8627, 1.0000))
				("ghostwhite", color(0.9725, 0.9725, 1.0000, 1.0000))
				("gold", color(1.0000, 0.8431, 0.0000, 1.0000))
				("goldenrod", color(0.8549, 0.6471, 0.1255, 1.0000))
				("gray", color(0.5020, 0.5020, 0.5020, 1.0000))
				("grey", color(0.5020, 0.5020, 0.5020, 1.0000))
				("green", color(0.0000, 0.5020, 0.0000, 1.0000))
				("greenyellow", color(0.6784, 1.0000, 0.1843, 1.0000))
				("honeydew", color(0.9412, 1.0000, 0.9412, 1.0000))
				("hotpink", color(1.0000, 0.4118, 0.7059, 1.0000))
				("indianred", color(0.8039, 0.3608, 0.3608, 1.0000))
				("indigo", color(0.2941, 0.0000, 0.5098, 1.0000))
				("ivory", color(1.0000, 1.0000, 0.9412, 1.0000))
				("khaki", color(0.9412, 0.9020, 0.5490, 1.0000))
				("lavender", color(0.9020, 0.9020, 0.9804, 1.0000))
				("lavenderblush", color(1.0000, 0.9412, 0.9608, 1.0000))
				("lawngreen", color(0.4863, 0.9882, 0.0000, 1.0000))
				("lemonchiffon", color(1.0000, 0.9804, 0.8039, 1.0000))
				("lightblue", color(0.6784, 0.8471, 0.9020, 1.0000))
				("lightcoral", color(0.9412, 0.5020, 0.5020, 1.0000))
				("lightcyan", color(0.8784, 1.0000, 1.0000, 1.0000))
				("lightgoldenrodyellow", color(0.9804, 0.9804, 0.8235, 1.0000))
				("lightgray", color(0.8275, 0.8275, 0.8275, 1.0000))
				("lightgreen", color(0.5647, 0.9333, 0.5647, 1.0000))
				("lightgrey", color(0.8275, 0.8275, 0.8275, 1.0000))
				("lightpink", color(1.0000, 0.7137, 0.7569, 1.0000))
				("lightsalmon", color(1.0000, 0.6275, 0.4784, 1.0000))
				("lightseagreen", color(0.1255, 0.6980, 0.6667, 1.0000))
				("lightskyblue", color(0.5294, 0.8078, 0.9804, 1.0000))
				("lightslategray", color(0.4667, 0.5333, 0.6000, 1.0000))
				("lightslategrey", color(0.4667, 0.5333, 0.6000, 1.0000))
				("lightsteelblue", color(0.6902, 0.7686, 0.8706, 1.0000))
				("lightyellow", color(1.0000, 1.0000, 0.8784, 1.0000))
				("lime", color(0.0000, 1.0000, 0.0000, 1.0000))
				("limegreen", color(0.1961, 0.8039, 0.1961, 1.0000))
				("linen", color(0.9804, 0.9412, 0.9020, 1.0000))
				("magenta", color(1.0000, 0.0000, 1.0000, 1.0000))
				("maroon", color(0.5020, 0.0000, 0.0000, 1.0000))
				("mediumaquamarine", color(0.4000, 0.8039, 0.6667, 1.0000))
				("mediumblue", color(0.0000, 0.0000, 0.8039, 1.0000))
				("mediumorchid", color(0.7294, 0.3333, 0.8275, 1.0000))
				("mediumpurple", color(0.5765, 0.4392, 0.8588, 1.0000))
				("mediumseagreen", color(0.2353, 0.7020, 0.4431, 1.0000))
				("mediumslateblue", color(0.4824, 0.4078, 0.9333, 1.0000))
				("mediumspringgreen", color(0.0000, 0.9804, 0.6039, 1.0000))
				("mediumturquoise", color(0.2824, 0.8196, 0.8000, 1.0000))
				("mediumvioletred", color(0.7804, 0.0824, 0.5216, 1.0000))
				("midnightblue", color(0.0980, 0.0980, 0.4392, 1.0000))
				("mintcream", color(0.9608, 1.0000, 0.9804, 1.0000))
				("mistyrose", color(1.0000, 0.8941, 0.8824, 1.0000))
				("moccasin", color(1.0000, 0.8941, 0.7098, 1.0000))
				("navajowhite", color(1.0000, 0.8706, 0.6784, 1.0000))
				("navy", color(0.0000, 0.0000, 0.5020, 1.0000))
				("oldlace", color(0.9922, 0.9608, 0.9020, 1.0000))
				("olive", color(0.5020, 0.5020, 0.0000, 1.0000))
				("olivedrab", color(0.4196, 0.5569, 0.1373, 1.0000))
				("orange", color(1.0000, 0.6471, 0.0000, 1.0000))
				("orangered", color(1.0000, 0.2706, 0.0000, 1.0000))
				("orchid", color(0.8549, 0.4392, 0.8392, 1.0000))
				("palegoldenrod", color(0.9333, 0.9098, 0.6667, 1.0000))
				("palegreen", color(0.5961, 0.9843, 0.5961, 1.0000))
				("paleturquoise", color(0.6863, 0.9333, 0.9333, 1.0000))
				("palevioletred", color(0.8588, 0.4392, 0.5765, 1.0000))
				("papayawhip", color(1.0000, 0.9373, 0.8353, 1.0000))
				("peachpuff", color(1.0000, 0.8549, 0.7255, 1.0000))
				("peru", color(0.8039, 0.5216, 0.2471, 1.0000))
				("pink", color(1.0000, 0.7529, 0.7961, 1.0000))
				("plum", color(0.8667, 0.6275, 0.8667, 1.0000))
				("powderblue", color(0.6902, 0.8784, 0.9020, 1.0000))
				("purple", color(0.5020, 0.0000, 0.5020, 1.0000))
				("red", color(1.0000, 0.0000, 0.0000, 1.0000))
				("rosybrown", color(0.7373, 0.5608, 0.5608, 1.0000))
				("royalblue", color(0.2549, 0.4118, 0.8824, 1.0000))
				("saddlebrown", color(0.5451, 0.2706, 0.0745, 1.0000))
				("salmon", color(0.9804, 0.5020, 0.4471, 1.0000))
				("sandybrown", color(0.9569, 0.6431, 0.3765, 1.0000))
				("seagreen", color(0.1804, 0.5451, 0.3412, 1.0000))
				("seashell", color(1.0000, 0.9608, 0.9333, 1.0000))
				("sienna", color(0.6275, 0.3216, 0.1765, 1.0000))
				("silver", color(0.7529, 0.7529, 0.7529, 1.0000))
				("skyblue", color(0.5294, 0.8078, 0.9216, 1.0000))
				("slateblue", color(0.4157, 0.3529, 0.8039, 1.0000))
				("slategray", color(0.4392, 0.5020, 0.5647, 1.0000))
				("slategrey", color(0.4392, 0.5020, 0.5647, 1.0000))
				("snow", color(1.0000, 0.9804, 0.9804, 1.0000))
				("springgreen", color(0.0000, 1.0000, 0.4980, 1.0000))
				("steelblue", color(0.2745, 0.5098, 0.7059, 1.0000))
				("tan", color(0.8235, 0.7059, 0.5490, 1.0000))
				("teal", color(0.0000, 0.5020, 0.5020, 1.0000))
				("thistle", color(0.8471, 0.7490, 0.8471, 1.0000))
				("tomato", color(1.0000, 0.3882, 0.2784, 1.0000))
				("turquoise", color(0.2510, 0.8784, 0.8157, 1.0000))
				("violet", color(0.9333, 0.5098, 0.9333, 1.0000))
				("wheat", color(0.9608, 0.8706, 0.7020, 1.0000))
				("white", color(1.0000, 1.0000, 1.0000, 1.0000))
				("whitesmoke", color(0.9608, 0.9608, 0.9608, 1.0000))
				("yellow", color(1.0000, 1.0000, 0.0000, 1.0000))
				("yellowgreen", color(0.6039, 0.8039, 0.1961, 1.0000))
				("transparent", color(0.0000, 0.0000, 0.0000, 0.0000))
				// </editor-fold>
			;
		}
	} named_colors;

	// ---------------------------------------------------------------------------------------------

	const auto translate_rgb = [](auto& ctx) {
		_val(ctx).r = std::get<0>(_attr(ctx));
		_val(ctx).g = std::get<1>(_attr(ctx));
		_val(ctx).b = std::get<2>(_attr(ctx));
		_val(ctx).a = std::get<3>(_attr(ctx));
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
			_val(ctx).r = m + c;
			_val(ctx).g = m + x;
			_val(ctx).b = m;
		} else if (h < 120.f) {
			_val(ctx).r = m + x;
			_val(ctx).g = m + c;
			_val(ctx).b = m;
		} else if (h < 180.f) {
			_val(ctx).r = m;
			_val(ctx).g = m + c;
			_val(ctx).b = m + x;
		} else if (h < 240.f) {
			_val(ctx).r = m;
			_val(ctx).g = m + x;
			_val(ctx).b = m + c;
		} else if (h < 300.f) {
			_val(ctx).r = m + x;
			_val(ctx).g = m;
			_val(ctx).b = m + c;
		} else {
			_val(ctx).r = m + c;
			_val(ctx).g = m;
			_val(ctx).b = m + x;
		}
		_val(ctx).a = std::get<3>(_attr(ctx));
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
			_val(ctx).r = m + c;
			_val(ctx).g = m + x;
			_val(ctx).b = m;
		} else if (h < 120.f) {
			_val(ctx).r = m + x;
			_val(ctx).g = m + c;
			_val(ctx).b = m;
		} else if (h < 180.f) {
			_val(ctx).r = m;
			_val(ctx).g = m + c;
			_val(ctx).b = m + x;
		} else if (h < 240.f) {
			_val(ctx).r = m;
			_val(ctx).g = m + x;
			_val(ctx).b = m + c;
		} else if (h < 300.f) {
			_val(ctx).r = m + x;
			_val(ctx).g = m;
			_val(ctx).b = m + c;
		} else {
			_val(ctx).r = m + c;
			_val(ctx).g = m;
			_val(ctx).b = m + x;
		}
		_val(ctx).a = std::get<3>(_attr(ctx));
	};

	// ---------------------------------------------------------------------------------------------

	const auto identity = [](auto& ctx) {
		_val(ctx) = _attr(ctx);
	};

	const auto scale = [](float value) {
		return [value](auto& ctx) {
			if(_attr(ctx) > value)
				_pass(ctx) = false;
			_val(ctx) = _attr(ctx) * (1.0f / value);
		};
	};

	// ---------------------------------------------------------------------------------------------

	const auto hex1_channel = x3::rule<class hex1_channel, float>{} =
			x3::uint_parser<uint8_t, 16, 1, 1>{}[scale(15)];

	const auto hex2_channel = x3::rule<class hex2_channel, float>{} =
			x3::uint_parser<uint8_t, 16, 2, 2>{}[scale(255)];

	const auto rgb_channel = x3::rule<class rgb_channel, float>{} =
			(x3::real_parser<float, x3::ureal_policies<float>>{} >> x3::lit('%'))[scale(100)] |
			(x3::real_parser<float, x3::strict_ureal_policies<float>>{})[scale(1)] |
			(x3::uint_parser<uint8_t, 10, 1, 3>{})[scale(255)];

	const auto hue_channel = x3::rule<class hue_channel, float>{} =
			x3::float_ >> -x3::lit("°");

	// ---------------------------------------------------------------------------------------------

	const auto hex1_color = x3::rule<class hex1_color, float4>{} = x3::lit('#') >>
			hex1_channel >>
			hex1_channel >>
			hex1_channel >>
			(hex1_channel | x3::attr(1.0f));

	const auto hex2_color = x3::rule<class hex2_color, float4>{} = x3::lit('#') >>
			hex2_channel >>
			hex2_channel >>
			hex2_channel >>
			(hex2_channel | x3::attr(1.0f));

	const auto rgb_color = x3::rule<class rgb_color, float4>{} =
			x3::no_case[x3::lit("rgb")] >>
			x3::lit('(') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			(x3::lit(',') >> rgb_channel | x3::attr(1.0f)) >> x3::lit(')');

	const auto rgba_color = x3::rule<class rgba_color, float4>{} =
			x3::no_case[x3::lit("rgba")] >>
			x3::lit('(') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >> x3::lit(')');

	const auto hsl_color = x3::rule<class hsl_color, float4>{} =
			x3::no_case[x3::lit("hsl")] >>
			x3::lit('(') >> hue_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			(x3::lit(',') >> rgb_channel | x3::attr(1.0f)) >> x3::lit(')');

	const auto hsla_color = x3::rule<class hsla_color, float4>{} =
			x3::no_case[x3::lit("hsla")] >>
			x3::lit('(') >> hue_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >> x3::lit(')');

	const auto hsv_color = x3::rule<class hsv_color, float4>{} =
			x3::no_case[x3::lit("hsv")] >>
			x3::lit('(') >> hue_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			(x3::lit(',') >> rgb_channel | x3::attr(1.0f)) >> x3::lit(')');

	const auto hsva_color = x3::rule<class hsva_color, float4>{} =
			x3::no_case[x3::lit("hsva")] >>
			x3::lit('(') >> hue_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >>
			x3::lit(',') >> rgb_channel >> x3::lit(')');

	const auto color_rule = x3::rule<class color_rule, color>{} =
			x3::no_case[named_colors][identity] |
			hex2_color[translate_rgb] |
			hex1_color[translate_rgb] |
			rgb_color[translate_rgb] |
			rgba_color[translate_rgb] |
			hsl_color[translate_hsl] |
			hsla_color[translate_hsl] |
			hsv_color[translate_hsv] |
			hsva_color[translate_hsv];

	color color(1, 1, 1, 1);

	phrase_parse(str.begin(), str.end(), color_rule, x3::space, color);

	return color;
}

// -------------------------------------------------------------------------------------------------

} //namespace parse
} //namespace libv
