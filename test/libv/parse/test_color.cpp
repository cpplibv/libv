// Project: libv.parse, File: test/libv/parse/test_color.cpp

#include <catch/catch.hpp>

#include <libv/parse/color.hpp>
#include <libv/utility/approx.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

using libv::parse::parse_color_or_throw;
using libv::parse::parse_color_optional;
using color = libv::vec4_t<libv::Approx<float, -3>>;

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("parse color: test everything", "[libv.parse]") {
	CHECK(color(1.000f, 0.000f, 0.000f, 1.000f) == parse_color_or_throw("red"));
	CHECK(color(0.753f, 0.753f, 0.753f, 1.000f) == parse_color_or_throw("silver"));
	CHECK(color(0.498f, 1.000f, 0.831f, 1.000f) == parse_color_or_throw("aquamarine"));

	CHECK(color(1.000f, 0.733f, 0.800f, 1.000f) == parse_color_or_throw("#FBC"));
	CHECK(color(1.000f, 0.733f, 0.800f, 0.933f) == parse_color_or_throw("#FBCE"));
	CHECK(color(0.941f, 0.945f, 0.949f, 1.000f) == parse_color_or_throw("#F0F1F2"));
	CHECK(color(0.941f, 0.945f, 0.949f, 0.953f) == parse_color_or_throw("#F0F1F2F3"));
	CHECK(color(0.941f, 0.945f, 0.949f, 0.953f) == parse_color_or_throw("# F0F1F2F3"));
	CHECK(color(0.941f, 0.945f, 0.949f, 0.953f) == parse_color_or_throw(" # f0 F1 F2 F3"));
	CHECK(color(0.941f, 0.945f, 0.949f, 0.953f) == parse_color_or_throw("#F0 F1 F2 F3"));

	CHECK(color(0.055f, 0.059f, 0.063f, 1.000f) == parse_color_or_throw("rgb(14,15,16)"));
	CHECK(color(0.055f, 0.150f, 0.063f, 1.000f) == parse_color_or_throw("rgb(14,15%,16)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 1.000f) == parse_color_or_throw("rgb(14%,15.4%,16%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 1.000f) == parse_color_or_throw("rgb(0.14,0.154,0.16)"));
	CHECK(color(1.000f, 1.000f, 0.000f, 1.000f) == parse_color_or_throw("RGB(1.,1.,0.)"));
	CHECK(color(0.140f, 0.154f, 0.009f, 1.000f) == parse_color_or_throw("rgb(.14,.154,.9%)"));
	CHECK(color(0.140f, 0.059f, 0.063f, 1.000f) == parse_color_or_throw("RGB (  14 %,   15 ,16)"));

	CHECK(color(0.055f, 0.059f, 0.063f, 0.170f) == parse_color_or_throw("rgb(14,15,16,17%)"));
	CHECK(color(0.055f, 0.059f, 0.063f, 0.174f) == parse_color_or_throw("rgb(14, 15 ,16, 17.4%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 0.170f) == parse_color_or_throw("rgb(14%,15.4%,16%,17%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 0.400f) == parse_color_or_throw("rgb(14%,15.4%,16%,0.4)"));
	CHECK(color(0.140f, 0.150f, 0.160f, 0.170f) == parse_color_or_throw("rgb ( 14%, 15%, 16 % ,17% )"));

	CHECK(color(0.055f, 0.059f, 0.063f, 0.170f) == parse_color_or_throw("rgba(14,15,16,17%)"));
	CHECK(color(0.055f, 0.059f, 0.063f, 0.174f) == parse_color_or_throw("rgba(14, 15 ,16, 17.4%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 0.170f) == parse_color_or_throw("rgba(14%,15.4%,16%,17%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 0.400f) == parse_color_or_throw("RGBA(14%,15.4%,16%,0.4)"));
	CHECK(color(0.140f, 0.150f, 0.160f, 0.170f) == parse_color_or_throw("rgba ( 14%, 15%, 16 % ,17% )"));

	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == parse_color_or_throw("hsl(14,15%,255)"));
	CHECK(color(0.230f, 0.184f, 0.170f, 1.000f) == parse_color_or_throw("hsl(14,15%,51)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 1.000f) == parse_color_or_throw("hsl(14°,15%,16%)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 1.000f) == parse_color_or_throw("hsl(14,0.15,.16)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 1.000f) == parse_color_or_throw("hsl(374,0.15,.16)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 1.000f) == parse_color_or_throw("hsl(-346,0.15,.16)"));

	CHECK(color(0.066f, 0.061f, 0.059f, 0.170f) == parse_color_or_throw("hsl(14,15,16,17%)"));
	CHECK(color(0.066f, 0.061f, 0.059f, 0.174f) == parse_color_or_throw("hsl(14, 15 ,16, 17.4%)"));
	CHECK(color(0.185f, 0.147f, 0.135f, 0.170f) == parse_color_or_throw("hsl(14,15.4%,16%,17%)"));
	CHECK(color(0.185f, 0.147f, 0.135f, 0.400f) == parse_color_or_throw("hsl(14°,15.4%,16%,0.4)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 0.170f) == parse_color_or_throw("hsl ( 14, 15%, 16 % ,17% )"));

	CHECK(color(0.066f, 0.061f, 0.059f, 0.170f) == parse_color_or_throw("hsla(14,15,16,17%)"));
	CHECK(color(0.066f, 0.061f, 0.059f, 0.174f) == parse_color_or_throw("hsla(14, 15 ,16, 17.4%)"));
	CHECK(color(0.185f, 0.147f, 0.135f, 0.170f) == parse_color_or_throw("hsla(14°,15.4%,16%,17%)"));
	CHECK(color(0.185f, 0.147f, 0.135f, 0.400f) == parse_color_or_throw("hsla(14,15.4%,16%,0.4)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 0.170f) == parse_color_or_throw("hsla ( 14, 15%, 16 % ,17% )"));

	CHECK(color(1.000f, 0.885f, 0.850f, 1.000f) == parse_color_or_throw("hsv(14,15%,255)"));
	CHECK(color(0.200f, 0.177f, 0.170f, 1.000f) == parse_color_or_throw("hsv(14,15%,51)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 1.000f) == parse_color_or_throw("hsv(14°,15%,16%)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 1.000f) == parse_color_or_throw("hsv(14,0.15,.16)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 1.000f) == parse_color_or_throw("hsv(374,0.15,.16)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 1.000f) == parse_color_or_throw("hsv(-346,0.15,.16)"));

	CHECK(color(0.063f, 0.060f, 0.059f, 0.170f) == parse_color_or_throw("hsv(14,15,16,17%)"));
	CHECK(color(0.063f, 0.060f, 0.059f, 0.174f) == parse_color_or_throw("HSV(14, 15 ,16, 17.4%)"));
	CHECK(color(0.160f, 0.141f, 0.135f, 0.170f) == parse_color_or_throw("hsv(14,15.4%,16%,17%)"));
	CHECK(color(0.160f, 0.141f, 0.135f, 0.400f) == parse_color_or_throw("hsv(14°,15.4%,16%,0.4)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 0.170f) == parse_color_or_throw("hsv ( 14, 15%, 16 % ,17% )"));

	CHECK(color(0.063f, 0.060f, 0.059f, 0.170f) == parse_color_or_throw("hsva(14,15,16,17%)"));
	CHECK(color(0.063f, 0.060f, 0.059f, 0.174f) == parse_color_or_throw("hsva(14, 15 ,16, 17.4%)"));
	CHECK(color(0.160f, 0.141f, 0.135f, 0.170f) == parse_color_or_throw("HSVA(14°,15.4%,16%,17%)"));
	CHECK(color(0.160f, 0.141f, 0.135f, 0.400f) == parse_color_or_throw("hsva(14,15.4%,16%,0.4)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 0.170f) == parse_color_or_throw("hsva ( 14, 15%, 16 % ,17% )"));

	CHECK(color(0.800f, 0.800f, 0.800f, 1.000f) == parse_color_or_throw("hsva(  0°,  0%,  80%, 100%)"));
	CHECK(color(0.800f, 0.800f, 0.800f, 1.000f) == parse_color_or_throw("hsva(0.0°,0.0%,  80%, 100%)"));

	// ---------------------------------------------------------------------------------------------

	CHECK(not parse_color_optional("rgb(-.14,.154,.9%)"));
	CHECK(not parse_color_optional("rgb(3.14,.154,.9%)"));
	CHECK(not parse_color_optional("rgba(14%,15.4%,16%,-15.4)"));
	CHECK(not parse_color_optional("hsl(14%,15.4%,16%)"));
	CHECK(not parse_color_optional("hsla(14%,15.4%,16%, .3)"));
	CHECK(not parse_color_optional("hsl (  14 %,   15 ,16)"));
	CHECK(not parse_color_optional("#F0F1F"));
	CHECK(not parse_color_optional("#F0F1F2F"));
	CHECK(not parse_color_optional("#F0F1F2F3F4"));
	CHECK(not parse_color_optional("F0F1F2"));
	CHECK(not parse_color_optional("F0F1F2F3"));
	CHECK(not parse_color_optional("14,15,16"));
	CHECK(not parse_color_optional("14,15,16,17"));
	CHECK(not parse_color_optional("14, 15, 16"));
	CHECK(not parse_color_optional("14, 15, 16, 17"));
	CHECK(not parse_color_optional("(14,15,16)"));
	CHECK(not parse_color_optional("(14,15,16,17)"));
	CHECK(not parse_color_optional("(14, 15, 16)"));
	CHECK(not parse_color_optional("(14, 15, 16, 17)"));
	CHECK(not parse_color_optional("rgb(14,15,(16,17)"));
	CHECK(not parse_color_optional("rgb(14,15,)16,17)"));
	CHECK(not parse_color_optional("rgb(14)"));
	CHECK(not parse_color_optional("rgb(14,15)"));
	CHECK(not parse_color_optional("rgb(14,15,433)"));
	CHECK(not parse_color_optional("á"));
}
