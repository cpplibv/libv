// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <catch/catch.hpp>

#include <libv/parse/color.hpp>
#include <libv/utility/approxing.hpp>


// -------------------------------------------------------------------------------------------------

using libv::parse::color;
using libv::parse::to_color;
using ApproxColor = libv::vec4_t<libv::Approxing<float, -3>>;

#define CHECK_COLOR(R, G, B, A, STR) \
	CHECK(ApproxColor(R, G, B, A) == to_color(STR))

// -------------------------------------------------------------------------------------------------

TEST_CASE("parse color: test everything") {
	CHECK_COLOR(1.000f, 0.000f, 0.000f, 1.000f, "red");
	CHECK_COLOR(0.753f, 0.753f, 0.753f, 1.000f, "silver");
	CHECK_COLOR(0.498f, 1.000f, 0.831f, 1.000f, "aquamarine");

	CHECK_COLOR(1.000f, 0.733f, 0.800f, 1.000f, "#FBC");
	CHECK_COLOR(1.000f, 0.733f, 0.800f, 0.933f, "#FBCE");
	CHECK_COLOR(0.941f, 0.945f, 0.949f, 1.000f, "#F0F1F2");
	CHECK_COLOR(0.941f, 0.945f, 0.949f, 0.953f, "#F0F1F2F3");
	CHECK_COLOR(0.941f, 0.945f, 0.949f, 0.953f, "# F0F1F2F3");
	CHECK_COLOR(0.941f, 0.945f, 0.949f, 0.953f, " # f0 F1 F2 F3");
	CHECK_COLOR(0.941f, 0.945f, 0.949f, 0.953f, "#F0 F1 F2 F3");

	CHECK_COLOR(0.055f, 0.059f, 0.063f, 1.000f, "rgb(14,15,16)");
	CHECK_COLOR(0.055f, 0.150f, 0.063f, 1.000f, "rgb(14,15%,16)");
	CHECK_COLOR(0.140f, 0.154f, 0.160f, 1.000f, "rgb(14%,15.4%,16%)");
	CHECK_COLOR(0.140f, 0.154f, 0.160f, 1.000f, "rgb(0.14,0.154,0.16)");
	CHECK_COLOR(1.000f, 1.000f, 0.000f, 1.000f, "RGB(1.,1.,0.)");
	CHECK_COLOR(0.140f, 0.154f, 0.009f, 1.000f, "rgb(.14,.154,.9%)");
	CHECK_COLOR(0.140f, 0.059f, 0.063f, 1.000f, "RGB (  14 %,   15 ,16)");

	CHECK_COLOR(0.055f, 0.059f, 0.063f, 0.170f, "rgb(14,15,16,17%)");
	CHECK_COLOR(0.055f, 0.059f, 0.063f, 0.174f, "rgb(14, 15 ,16, 17.4%)");
	CHECK_COLOR(0.140f, 0.154f, 0.160f, 0.170f, "rgb(14%,15.4%,16%,17%)");
	CHECK_COLOR(0.140f, 0.154f, 0.160f, 0.400f, "rgb(14%,15.4%,16%,0.4)");
	CHECK_COLOR(0.140f, 0.150f, 0.160f, 0.170f, "rgb ( 14%, 15%, 16 % ,17% )");

	CHECK_COLOR(0.055f, 0.059f, 0.063f, 0.170f, "rgba(14,15,16,17%)");
	CHECK_COLOR(0.055f, 0.059f, 0.063f, 0.174f, "rgba(14, 15 ,16, 17.4%)");
	CHECK_COLOR(0.140f, 0.154f, 0.160f, 0.170f, "rgba(14%,15.4%,16%,17%)");
	CHECK_COLOR(0.140f, 0.154f, 0.160f, 0.400f, "RGBA(14%,15.4%,16%,0.4)");
	CHECK_COLOR(0.140f, 0.150f, 0.160f, 0.170f, "rgba ( 14%, 15%, 16 % ,17% )");

	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "hsl(14,15%,255)");
	CHECK_COLOR(0.230f, 0.184f, 0.170f, 1.000f, "hsl(14,15%,51)");
	CHECK_COLOR(0.184f, 0.147f, 0.136f, 1.000f, "hsl(14°,15%,16%)");
	CHECK_COLOR(0.184f, 0.147f, 0.136f, 1.000f, "hsl(14,0.15,.16)");
	CHECK_COLOR(0.184f, 0.147f, 0.136f, 1.000f, "hsl(374,0.15,.16)");
	CHECK_COLOR(0.184f, 0.147f, 0.136f, 1.000f, "hsl(-346,0.15,.16)");

	CHECK_COLOR(0.066f, 0.061f, 0.059f, 0.170f, "hsl(14,15,16,17%)");
	CHECK_COLOR(0.066f, 0.061f, 0.059f, 0.174f, "hsl(14, 15 ,16, 17.4%)");
	CHECK_COLOR(0.185f, 0.147f, 0.135f, 0.170f, "hsl(14,15.4%,16%,17%)");
	CHECK_COLOR(0.185f, 0.147f, 0.135f, 0.400f, "hsl(14°,15.4%,16%,0.4)");
	CHECK_COLOR(0.184f, 0.147f, 0.136f, 0.170f, "hsl ( 14, 15%, 16 % ,17% )");

	CHECK_COLOR(0.066f, 0.061f, 0.059f, 0.170f, "hsla(14,15,16,17%)");
	CHECK_COLOR(0.066f, 0.061f, 0.059f, 0.174f, "hsla(14, 15 ,16, 17.4%)");
	CHECK_COLOR(0.185f, 0.147f, 0.135f, 0.170f, "hsla(14°,15.4%,16%,17%)");
	CHECK_COLOR(0.185f, 0.147f, 0.135f, 0.400f, "hsla(14,15.4%,16%,0.4)");
	CHECK_COLOR(0.184f, 0.147f, 0.136f, 0.170f, "hsla ( 14, 15%, 16 % ,17% )");

	CHECK_COLOR(1.000f, 0.885f, 0.850f, 1.000f, "hsv(14,15%,255)");
	CHECK_COLOR(0.200f, 0.177f, 0.170f, 1.000f, "hsv(14,15%,51)");
	CHECK_COLOR(0.160f, 0.142f, 0.136f, 1.000f, "hsv(14°,15%,16%)");
	CHECK_COLOR(0.160f, 0.142f, 0.136f, 1.000f, "hsv(14,0.15,.16)");
	CHECK_COLOR(0.160f, 0.142f, 0.136f, 1.000f, "hsv(374,0.15,.16)");
	CHECK_COLOR(0.160f, 0.142f, 0.136f, 1.000f, "hsv(-346,0.15,.16)");

	CHECK_COLOR(0.063f, 0.060f, 0.059f, 0.170f, "hsv(14,15,16,17%)");
	CHECK_COLOR(0.063f, 0.060f, 0.059f, 0.174f, "HSV(14, 15 ,16, 17.4%)");
	CHECK_COLOR(0.160f, 0.141f, 0.135f, 0.170f, "hsv(14,15.4%,16%,17%)");
	CHECK_COLOR(0.160f, 0.141f, 0.135f, 0.400f, "hsv(14°,15.4%,16%,0.4)");
	CHECK_COLOR(0.160f, 0.142f, 0.136f, 0.170f, "hsv ( 14, 15%, 16 % ,17% )");

	CHECK_COLOR(0.063f, 0.060f, 0.059f, 0.170f, "hsva(14,15,16,17%)");
	CHECK_COLOR(0.063f, 0.060f, 0.059f, 0.174f, "hsva(14, 15 ,16, 17.4%)");
	CHECK_COLOR(0.160f, 0.141f, 0.135f, 0.170f, "HSVA(14°,15.4%,16%,17%)");
	CHECK_COLOR(0.160f, 0.141f, 0.135f, 0.400f, "hsva(14,15.4%,16%,0.4)");
	CHECK_COLOR(0.160f, 0.142f, 0.136f, 0.170f, "hsva ( 14, 15%, 16 % ,17% )");

	// ---------------------------------------------------------------------------------------------

	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "rgb(-.14,.154,.9%)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "rgb(3.14,.154,.9%)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "rgba(14%,15.4%,16%,-15.4)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "hsl(14%,15.4%,16%)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "hsla(14%,15.4%,16%, .3)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "hsl (  14 %,   15 ,16)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "F0F1F2");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "F0F1F2F3");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "14,15,16");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "14,15,16,17");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "14, 15, 16");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "14, 15, 16, 17");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "(14,15,16)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "(14,15,16,17)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "(14, 15, 16)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "(14, 15, 16, 17)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "rgb(14,15,(16,17)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "rgb(14,15,)16,17)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "rgb(14)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "rgb(14,15)");
	CHECK_COLOR(1.000f, 1.000f, 1.000f, 1.000f, "rgb(14,15,433)");
}
