// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <catch/catch.hpp>

#include <libv/parse/color.hpp>
#include <libv/utility/approxing.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

using libv::parse::to_color;
using color = libv::vec4_t<libv::Approxing<float, -3>>;

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("parse color: test everything") {
	CHECK(color(1.000f, 0.000f, 0.000f, 1.000f) == to_color("red"));
	CHECK(color(0.753f, 0.753f, 0.753f, 1.000f) == to_color("silver"));
	CHECK(color(0.498f, 1.000f, 0.831f, 1.000f) == to_color("aquamarine"));

	CHECK(color(1.000f, 0.733f, 0.800f, 1.000f) == to_color("#FBC"));
	CHECK(color(1.000f, 0.733f, 0.800f, 0.933f) == to_color("#FBCE"));
	CHECK(color(0.941f, 0.945f, 0.949f, 1.000f) == to_color("#F0F1F2"));
	CHECK(color(0.941f, 0.945f, 0.949f, 0.953f) == to_color("#F0F1F2F3"));
	CHECK(color(0.941f, 0.945f, 0.949f, 0.953f) == to_color("# F0F1F2F3"));
	CHECK(color(0.941f, 0.945f, 0.949f, 0.953f) == to_color(" # f0 F1 F2 F3"));
	CHECK(color(0.941f, 0.945f, 0.949f, 0.953f) == to_color("#F0 F1 F2 F3"));

	CHECK(color(0.055f, 0.059f, 0.063f, 1.000f) == to_color("rgb(14,15,16)"));
	CHECK(color(0.055f, 0.150f, 0.063f, 1.000f) == to_color("rgb(14,15%,16)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 1.000f) == to_color("rgb(14%,15.4%,16%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 1.000f) == to_color("rgb(0.14,0.154,0.16)"));
	CHECK(color(1.000f, 1.000f, 0.000f, 1.000f) == to_color("RGB(1.,1.,0.)"));
	CHECK(color(0.140f, 0.154f, 0.009f, 1.000f) == to_color("rgb(.14,.154,.9%)"));
	CHECK(color(0.140f, 0.059f, 0.063f, 1.000f) == to_color("RGB (  14 %,   15 ,16)"));

	CHECK(color(0.055f, 0.059f, 0.063f, 0.170f) == to_color("rgb(14,15,16,17%)"));
	CHECK(color(0.055f, 0.059f, 0.063f, 0.174f) == to_color("rgb(14, 15 ,16, 17.4%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 0.170f) == to_color("rgb(14%,15.4%,16%,17%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 0.400f) == to_color("rgb(14%,15.4%,16%,0.4)"));
	CHECK(color(0.140f, 0.150f, 0.160f, 0.170f) == to_color("rgb ( 14%, 15%, 16 % ,17% )"));

	CHECK(color(0.055f, 0.059f, 0.063f, 0.170f) == to_color("rgba(14,15,16,17%)"));
	CHECK(color(0.055f, 0.059f, 0.063f, 0.174f) == to_color("rgba(14, 15 ,16, 17.4%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 0.170f) == to_color("rgba(14%,15.4%,16%,17%)"));
	CHECK(color(0.140f, 0.154f, 0.160f, 0.400f) == to_color("RGBA(14%,15.4%,16%,0.4)"));
	CHECK(color(0.140f, 0.150f, 0.160f, 0.170f) == to_color("rgba ( 14%, 15%, 16 % ,17% )"));

	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("hsl(14,15%,255)"));
	CHECK(color(0.230f, 0.184f, 0.170f, 1.000f) == to_color("hsl(14,15%,51)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 1.000f) == to_color("hsl(14°,15%,16%)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 1.000f) == to_color("hsl(14,0.15,.16)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 1.000f) == to_color("hsl(374,0.15,.16)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 1.000f) == to_color("hsl(-346,0.15,.16)"));

	CHECK(color(0.066f, 0.061f, 0.059f, 0.170f) == to_color("hsl(14,15,16,17%)"));
	CHECK(color(0.066f, 0.061f, 0.059f, 0.174f) == to_color("hsl(14, 15 ,16, 17.4%)"));
	CHECK(color(0.185f, 0.147f, 0.135f, 0.170f) == to_color("hsl(14,15.4%,16%,17%)"));
	CHECK(color(0.185f, 0.147f, 0.135f, 0.400f) == to_color("hsl(14°,15.4%,16%,0.4)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 0.170f) == to_color("hsl ( 14, 15%, 16 % ,17% )"));

	CHECK(color(0.066f, 0.061f, 0.059f, 0.170f) == to_color("hsla(14,15,16,17%)"));
	CHECK(color(0.066f, 0.061f, 0.059f, 0.174f) == to_color("hsla(14, 15 ,16, 17.4%)"));
	CHECK(color(0.185f, 0.147f, 0.135f, 0.170f) == to_color("hsla(14°,15.4%,16%,17%)"));
	CHECK(color(0.185f, 0.147f, 0.135f, 0.400f) == to_color("hsla(14,15.4%,16%,0.4)"));
	CHECK(color(0.184f, 0.147f, 0.136f, 0.170f) == to_color("hsla ( 14, 15%, 16 % ,17% )"));

	CHECK(color(1.000f, 0.885f, 0.850f, 1.000f) == to_color("hsv(14,15%,255)"));
	CHECK(color(0.200f, 0.177f, 0.170f, 1.000f) == to_color("hsv(14,15%,51)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 1.000f) == to_color("hsv(14°,15%,16%)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 1.000f) == to_color("hsv(14,0.15,.16)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 1.000f) == to_color("hsv(374,0.15,.16)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 1.000f) == to_color("hsv(-346,0.15,.16)"));

	CHECK(color(0.063f, 0.060f, 0.059f, 0.170f) == to_color("hsv(14,15,16,17%)"));
	CHECK(color(0.063f, 0.060f, 0.059f, 0.174f) == to_color("HSV(14, 15 ,16, 17.4%)"));
	CHECK(color(0.160f, 0.141f, 0.135f, 0.170f) == to_color("hsv(14,15.4%,16%,17%)"));
	CHECK(color(0.160f, 0.141f, 0.135f, 0.400f) == to_color("hsv(14°,15.4%,16%,0.4)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 0.170f) == to_color("hsv ( 14, 15%, 16 % ,17% )"));

	CHECK(color(0.063f, 0.060f, 0.059f, 0.170f) == to_color("hsva(14,15,16,17%)"));
	CHECK(color(0.063f, 0.060f, 0.059f, 0.174f) == to_color("hsva(14, 15 ,16, 17.4%)"));
	CHECK(color(0.160f, 0.141f, 0.135f, 0.170f) == to_color("HSVA(14°,15.4%,16%,17%)"));
	CHECK(color(0.160f, 0.141f, 0.135f, 0.400f) == to_color("hsva(14,15.4%,16%,0.4)"));
	CHECK(color(0.160f, 0.142f, 0.136f, 0.170f) == to_color("hsva ( 14, 15%, 16 % ,17% )"));

	// ---------------------------------------------------------------------------------------------

	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("rgb(-.14,.154,.9%)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("rgb(3.14,.154,.9%)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("rgba(14%,15.4%,16%,-15.4)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("hsl(14%,15.4%,16%)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("hsla(14%,15.4%,16%, .3)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("hsl (  14 %,   15 ,16)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("F0F1F2"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("F0F1F2F3"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("14,15,16"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("14,15,16,17"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("14, 15, 16"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("14, 15, 16, 17"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("(14,15,16)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("(14,15,16,17)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("(14, 15, 16)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("(14, 15, 16, 17)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("rgb(14,15,(16,17)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("rgb(14,15,)16,17)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("rgb(14)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("rgb(14,15)"));
	CHECK(color(1.000f, 1.000f, 1.000f, 1.000f) == to_color("rgb(14,15,433)"));
}
