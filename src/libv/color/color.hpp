// Project: libv.color, File: src/libv/color/color.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace color {

// -------------------------------------------------------------------------------------------------

struct SpaceHCL {};
struct SpaceHCV {};
struct SpaceHCY {};
struct SpaceHSL {};
struct SpaceHSV {};
struct SpaceRGB {};

template <typename Rep = float, typename ColorSpace = SpaceRGB, bool HasAlpha = true>
struct Color {
	static constexpr bool is_integral = std::is_integral_v<Rep>;

	using rep = Rep;
	using rep_vec = std::conditional_t<HasAlpha, libv::vec4_t<rep>, libv::vec3_t<rep>>;
};

// -------------------------------------------------------------------------------------------------

template <typename Rep> using RGB_t = color::Color<Rep, color::SpaceRGB, false>;
template <typename Rep> using RGBA_t = color::Color<Rep, color::SpaceRGB, true>;

using RGBf = RGB_t<float>;
using RGBAf = RGBA_t<float>;
using RGBd = RGB_t<double>;
using RGBAd = RGBA_t<double>;
using RGB8 = RGB_t<uint8_t>;
using RGBA8 = RGBA_t<uint8_t>;
using RGB16 = RGB_t<uint16_t>;
using RGBA16 = RGBA_t<uint16_t>;
using RGB32 = RGB_t<uint32_t>;
using RGBA32 = RGBA_t<uint32_t>;
using RGB64 = RGB_t<uint64_t>;
using RGBA64 = RGBA_t<uint64_t>;

template <typename Rep> using HCL_t = color::Color<Rep, color::SpaceHCL, false>;
template <typename Rep> using HCLA_t = color::Color<Rep, color::SpaceHCL, true>;

using HCLf = HCL_t<float>;
using HCLAf = HCLA_t<float>;
using HCLd = HCL_t<double>;
using HCLAd = HCLA_t<double>;
using HCL8 = HCL_t<uint8_t>;
using HCLA8 = HCLA_t<uint8_t>;
using HCL16 = HCL_t<uint16_t>;
using HCLA16 = HCLA_t<uint16_t>;
using HCL32 = HCL_t<uint32_t>;
using HCLA32 = HCLA_t<uint32_t>;
using HCL64 = HCL_t<uint64_t>;
using HCLA64 = HCLA_t<uint64_t>;

// -------------------------------------------------------------------------------------------------

// Implementation status:
//
//     | HUE | HCL | HCV | HCY | HSL | HSV | RGB
// ----+-----+-----+-----+-----+-----+-----+-----
// HUE |  -
// HCL |        -
// HCV |              -
// HCY |                    -
// HSL |                          -
// HSV |                                -
// RGB |                                      -
//

template <typename Rep, typename TargetRep = Rep>
inline RGBf HUEtoRGB(const Rep H) noexcept {
	// TODO P1: Protect against integral overflow (8 bit)

	auto R = std::abs(H * 6 - 3) - 1;
	auto G = 2 - std::abs(H * 6 - 2);
	auto B = 2 - std::abs(H * 6 - 4);

	return libv::vec::clamp(RGBf{R, G, B}, 0, 1);
}

// -------------------------------------------------------------------------------------------------

} // namespace color

using Color = color::RGBAf;

// -------------------------------------------------------------------------------------------------

} // namespace libv

// =================================================================================================
// =================================================================================================
// =================================================================================================

////Converting pure hue to RGB
//
//float3 HUEtoRGB(in float H) {
//	float R = abs(H * 6 - 3) - 1;
//	float G = 2 - abs(H * 6 - 2);
//	float B = 2 - abs(H * 6 - 4);
//	return saturate(float3(R, G, B));
//}
//
////Converting RGB to hue / chroma / value
//
//float Epsilon = 1e-10;
//float3 RGBtoHCV(in float3 RGB) {
//	// Based on work by Sam Hocevar and Emil Persson
//	float4 P = (RGB.g < RGB.b) ? float4(RGB.bg, -1.0, 2.0 / 3.0) : float4(RGB.gb, 0.0, -1.0 / 3.0);
//	float4 Q = (RGB.r < P.x) ? float4(P.xyw, RGB.r) : float4(RGB.r, P.yzx);
//	float C = Q.x - min(Q.w, Q.y);
//	float H = abs((Q.w - Q.y) / (6 * C + Epsilon) + Q.z);
//	return float3(H, C, Q.x);
//}
//
////Converting HSV to RGB
//
//float3 HSVtoRGB(in float3 HSV) {
//	float3 RGB = HUEtoRGB(HSV.x);
//	return ((RGB - 1) * HSV.y + 1) * HSV.z;
//}
//
////Converting HSL to RGB
//
//float3 HSLtoRGB(in float3 HSL) {
//	float3 RGB = HUEtoRGB(HSL.x);
//	float C = (1 - abs(2 * HSL.z - 1)) * HSL.y;
//	return (RGB - 0.5) * C + HSL.z;
//}
//
////Converting HCY to RGB
//
//// The weights of RGB contributions to luminance.
//// Should sum to unity.
//float3 HCYwts = float3(0.299, 0.587, 0.114);
//float3 HCYtoRGB(in float3 HCY) {
//	float3 RGB = HUEtoRGB(HCY.x);
//	float Z = dot(RGB, HCYwts);
//	if (HCY.z < Z) {
//		HCY.y *= HCY.z / Z;
//	} else if (Z < 1) {
//		HCY.y *= (1 - HCY.z) / (1 - Z);
//	}
//	return (RGB - Z) * HCY.y + HCY.z;
//}
//
////Converting HCL to RGB
//
//float HCLgamma = 3;
//float HCLy0 = 100;
//float HCLmaxL = 0.530454533953517; // == exp(HCLgamma / HCLy0) - 0.5
//float PI = 3.1415926536;
//float3 HCLtoRGB(in float3 HCL) {
//	float3 RGB = 0;
//	if (HCL.z != 0) {
//		float H = HCL.x;
//		float C = HCL.y;
//		float L = HCL.z * HCLmaxL;
//		float Q = exp((1 - C / (2 * L)) * (HCLgamma / HCLy0));
//		float U = (2 * L - C) / (2 * Q - 1);
//		float V = C / Q;
//		float A = (H + min(frac(2 * H) / 4, frac(-2 * H) / 8)) * PI * 2;
//		float T;
//		H *= 6;
//		if (H <= 0.999) {
//			T = tan(A);
//			RGB.r = 1;
//			RGB.g = T / (1 + T);
//		} else if (H <= 1.001) {
//			RGB.r = 1;
//			RGB.g = 1;
//		} else if (H <= 2) {
//			T = tan(A);
//			RGB.r = (1 + T) / T;
//			RGB.g = 1;
//		} else if (H <= 3) {
//			T = tan(A);
//			RGB.g = 1;
//			RGB.b = 1 + T;
//		} else if (H <= 3.999) {
//			T = tan(A);
//			RGB.g = 1 / (1 + T);
//			RGB.b = 1;
//		} else if (H <= 4.001) {
//			RGB.g = 0;
//			RGB.b = 1;
//		} else if (H <= 5) {
//			T = tan(A);
//			RGB.r = -1 / T;
//			RGB.b = 1;
//		} else {
//			T = tan(A);
//			RGB.r = 1;
//			RGB.b = -T;
//		}
//		RGB = RGB * V + U;
//	}
//	return RGB;
//}
//
////Converting RGB to HSV
//
//float3 RGBtoHSV(in float3 RGB) {
//	float3 HCV = RGBtoHCV(RGB);
//	float S = HCV.y / (HCV.z + Epsilon);
//	return float3(HCV.x, S, HCV.z);
//}
//
////Converting RGB to HSL
//
//float3 RGBtoHSL(in float3 RGB) {
//	float3 HCV = RGBtoHCV(RGB);
//	float L = HCV.z - HCV.y * 0.5;
//	float S = HCV.y / (1 - abs(L * 2 - 1) + Epsilon);
//	return float3(HCV.x, S, L);
//}
//
////Converting RGB to HCY
//
//float3 RGBtoHCY(in float3 RGB) {
//	// Corrected by David Schaeffer
//	float3 HCV = RGBtoHCV(RGB);
//	float Y = dot(RGB, HCYwts);
//	float Z = dot(HUEtoRGB(HCV.x), HCYwts);
//	if (Y < Z) {
//		HCV.y *= Z / (Epsilon + Y);
//	} else {
//		HCV.y *= (1 - Z) / (Epsilon + 1 - Y);
//	}
//	return float3(HCV.x, HCV.y, Y);
//}
//
////Converting RGB to HCL
//
//float3 RGBtoHCL(in float3 RGB) {
//	float3 HCL;
//	float H = 0;
//	float U = min(RGB.r, min(RGB.g, RGB.b));
//	float V = max(RGB.r, max(RGB.g, RGB.b));
//	float Q = HCLgamma / HCLy0;
//	HCL.y = V - U;
//	if (HCL.y != 0) {
//		H = atan2(RGB.g - RGB.b, RGB.r - RGB.g) / PI;
//		Q *= U / V;
//	}
//	Q = exp(Q);
//	HCL.x = frac(H / 2 - min(frac(H), frac(-H)) / 6);
//	HCL.y *= Q;
//	HCL.z = lerp(-U, V, Q) / (HCLmaxL * 2);
//	return HCL;
//}
//
// =================================================================================================
//
//
//
// Use these constants in parse_color
//
//static constexpr aliceblue = Color(0.9412f, 0.9725f, 1.0000f, 1.0000f);
//static constexpr antiquewhite = Color(0.9804f, 0.9216f, 0.8431f, 1.0000f);
//static constexpr aqua = Color(0.0000f, 1.0000f, 1.0000f, 1.0000f);
//static constexpr aquamarine = Color(0.4980f, 1.0000f, 0.8314f, 1.0000f);
//static constexpr azure = Color(0.9412f, 1.0000f, 1.0000f, 1.0000f);
//static constexpr beige = Color(0.9608f, 0.9608f, 0.8627f, 1.0000f);
//static constexpr bisque = Color(1.0000f, 0.8941f, 0.7686f, 1.0000f);
//static constexpr black = Color(0.0000f, 0.0000f, 0.0000f, 1.0000f);
//static constexpr blanchedalmond = Color(1.0000f, 0.9216f, 0.8039f, 1.0000f);
//static constexpr blue = Color(0.0000f, 0.0000f, 1.0000f, 1.0000f);
//static constexpr blueviolet = Color(0.5412f, 0.1686f, 0.8863f, 1.0000f);
//static constexpr brown = Color(0.6471f, 0.1647f, 0.1647f, 1.0000f);
//static constexpr burlywood = Color(0.8706f, 0.7216f, 0.5294f, 1.0000f);
//static constexpr cadetblue = Color(0.3725f, 0.6196f, 0.6275f, 1.0000f);
//static constexpr chartreuse = Color(0.4980f, 1.0000f, 0.0000f, 1.0000f);
//static constexpr chocolate = Color(0.8235f, 0.4118f, 0.1176f, 1.0000f);
//static constexpr coral = Color(1.0000f, 0.4980f, 0.3137f, 1.0000f);
//static constexpr cornflowerblue = Color(0.3922f, 0.5843f, 0.9294f, 1.0000f);
//static constexpr cornsilk = Color(1.0000f, 0.9725f, 0.8627f, 1.0000f);
//static constexpr crimson = Color(0.8627f, 0.0784f, 0.2353f, 1.0000f);
//static constexpr cyan = Color(0.0000f, 1.0000f, 1.0000f, 1.0000f);
//static constexpr darkblue = Color(0.0000f, 0.0000f, 0.5451f, 1.0000f);
//static constexpr darkcyan = Color(0.0000f, 0.5451f, 0.5451f, 1.0000f);
//static constexpr darkgoldenrod = Color(0.7216f, 0.5255f, 0.0431f, 1.0000f);
//static constexpr darkgray = Color(0.6627f, 0.6627f, 0.6627f, 1.0000f);
//static constexpr darkgreen = Color(0.0000f, 0.3922f, 0.0000f, 1.0000f);
//static constexpr darkgrey = Color(0.6627f, 0.6627f, 0.6627f, 1.0000f);
//static constexpr darkkhaki = Color(0.7412f, 0.7176f, 0.4196f, 1.0000f);
//static constexpr darkmagenta = Color(0.5451f, 0.0000f, 0.5451f, 1.0000f);
//static constexpr darkolivegreen = Color(0.3333f, 0.4196f, 0.1843f, 1.0000f);
//static constexpr darkorange = Color(1.0000f, 0.5490f, 0.0000f, 1.0000f);
//static constexpr darkorchid = Color(0.6000f, 0.1961f, 0.8000f, 1.0000f);
//static constexpr darkred = Color(0.5451f, 0.0000f, 0.0000f, 1.0000f);
//static constexpr darksalmon = Color(0.9137f, 0.5882f, 0.4784f, 1.0000f);
//static constexpr darkseagreen = Color(0.5608f, 0.7373f, 0.5608f, 1.0000f);
//static constexpr darkslateblue = Color(0.2824f, 0.2392f, 0.5451f, 1.0000f);
//static constexpr darkslategrey = Color(0.1843f, 0.3098f, 0.3098f, 1.0000f);
//static constexpr darkturquoise = Color(0.0000f, 0.8078f, 0.8196f, 1.0000f);
//static constexpr darkviolet = Color(0.5804f, 0.0000f, 0.8275f, 1.0000f);
//static constexpr deeppink = Color(1.0000f, 0.0784f, 0.5765f, 1.0000f);
//static constexpr deepskyblue = Color(0.0000f, 0.7490f, 1.0000f, 1.0000f);
//static constexpr dimgray = Color(0.4118f, 0.4118f, 0.4118f, 1.0000f);
//static constexpr dimgrey = Color(0.4118f, 0.4118f, 0.4118f, 1.0000f);
//static constexpr dodgerblue = Color(0.1176f, 0.5647f, 1.0000f, 1.0000f);
//static constexpr firebrick = Color(0.6980f, 0.1333f, 0.1333f, 1.0000f);
//static constexpr floralwhite = Color(1.0000f, 0.9804f, 0.9412f, 1.0000f);
//static constexpr forestgreen = Color(0.1333f, 0.5451f, 0.1333f, 1.0000f);
//static constexpr fuchsia = Color(1.0000f, 0.0000f, 1.0000f, 1.0000f);
//static constexpr gainsboro = Color(0.8627f, 0.8627f, 0.8627f, 1.0000f);
//static constexpr ghostwhite = Color(0.9725f, 0.9725f, 1.0000f, 1.0000f);
//static constexpr gold = Color(1.0000f, 0.8431f, 0.0000f, 1.0000f);
//static constexpr goldenrod = Color(0.8549f, 0.6471f, 0.1255f, 1.0000f);
//static constexpr gray = Color(0.5020f, 0.5020f, 0.5020f, 1.0000f);
//static constexpr grey = Color(0.5020f, 0.5020f, 0.5020f, 1.0000f);
//static constexpr green = Color(0.0000f, 0.5020f, 0.0000f, 1.0000f);
//static constexpr greenyellow = Color(0.6784f, 1.0000f, 0.1843f, 1.0000f);
//static constexpr honeydew = Color(0.9412f, 1.0000f, 0.9412f, 1.0000f);
//static constexpr hotpink = Color(1.0000f, 0.4118f, 0.7059f, 1.0000f);
//static constexpr indianred = Color(0.8039f, 0.3608f, 0.3608f, 1.0000f);
//static constexpr indigo = Color(0.2941f, 0.0000f, 0.5098f, 1.0000f);
//static constexpr ivory = Color(1.0000f, 1.0000f, 0.9412f, 1.0000f);
//static constexpr khaki = Color(0.9412f, 0.9020f, 0.5490f, 1.0000f);
//static constexpr lavender = Color(0.9020f, 0.9020f, 0.9804f, 1.0000f);
//static constexpr lavenderblush = Color(1.0000f, 0.9412f, 0.9608f, 1.0000f);
//static constexpr lawngreen = Color(0.4863f, 0.9882f, 0.0000f, 1.0000f);
//static constexpr lemonchiffon = Color(1.0000f, 0.9804f, 0.8039f, 1.0000f);
//static constexpr lightblue = Color(0.6784f, 0.8471f, 0.9020f, 1.0000f);
//static constexpr lightcoral = Color(0.9412f, 0.5020f, 0.5020f, 1.0000f);
//static constexpr lightcyan = Color(0.8784f, 1.0000f, 1.0000f, 1.0000f);
//static constexpr lightgoldenrodyellow = Color(0.9804f, 0.9804f, 0.8235f, 1.0000f);
//static constexpr lightgray = Color(0.8275f, 0.8275f, 0.8275f, 1.0000f);
//static constexpr lightgreen = Color(0.5647f, 0.9333f, 0.5647f, 1.0000f);
//static constexpr lightgrey = Color(0.8275f, 0.8275f, 0.8275f, 1.0000f);
//static constexpr lightpink = Color(1.0000f, 0.7137f, 0.7569f, 1.0000f);
//static constexpr lightsalmon = Color(1.0000f, 0.6275f, 0.4784f, 1.0000f);
//static constexpr lightseagreen = Color(0.1255f, 0.6980f, 0.6667f, 1.0000f);
//static constexpr lightskyblue = Color(0.5294f, 0.8078f, 0.9804f, 1.0000f);
//static constexpr lightslategray = Color(0.4667f, 0.5333f, 0.6000f, 1.0000f);
//static constexpr lightslategrey = Color(0.4667f, 0.5333f, 0.6000f, 1.0000f);
//static constexpr lightsteelblue = Color(0.6902f, 0.7686f, 0.8706f, 1.0000f);
//static constexpr lightyellow = Color(1.0000f, 1.0000f, 0.8784f, 1.0000f);
//static constexpr lime = Color(0.0000f, 1.0000f, 0.0000f, 1.0000f);
//static constexpr limegreen = Color(0.1961f, 0.8039f, 0.1961f, 1.0000f);
//static constexpr linen = Color(0.9804f, 0.9412f, 0.9020f, 1.0000f);
//static constexpr magenta = Color(1.0000f, 0.0000f, 1.0000f, 1.0000f);
//static constexpr maroon = Color(0.5020f, 0.0000f, 0.0000f, 1.0000f);
//static constexpr mediumaquamarine = Color(0.4000f, 0.8039f, 0.6667f, 1.0000f);
//static constexpr mediumblue = Color(0.0000f, 0.0000f, 0.8039f, 1.0000f);
//static constexpr mediumorchid = Color(0.7294f, 0.3333f, 0.8275f, 1.0000f);
//static constexpr mediumpurple = Color(0.5765f, 0.4392f, 0.8588f, 1.0000f);
//static constexpr mediumseagreen = Color(0.2353f, 0.7020f, 0.4431f, 1.0000f);
//static constexpr mediumslateblue = Color(0.4824f, 0.4078f, 0.9333f, 1.0000f);
//static constexpr mediumspringgreen = Color(0.0000f, 0.9804f, 0.6039f, 1.0000f);
//static constexpr mediumturquoise = Color(0.2824f, 0.8196f, 0.8000f, 1.0000f);
//static constexpr mediumvioletred = Color(0.7804f, 0.0824f, 0.5216f, 1.0000f);
//static constexpr midnightblue = Color(0.0980f, 0.0980f, 0.4392f, 1.0000f);
//static constexpr mintcream = Color(0.9608f, 1.0000f, 0.9804f, 1.0000f);
//static constexpr mistyrose = Color(1.0000f, 0.8941f, 0.8824f, 1.0000f);
//static constexpr moccasin = Color(1.0000f, 0.8941f, 0.7098f, 1.0000f);
//static constexpr navajowhite = Color(1.0000f, 0.8706f, 0.6784f, 1.0000f);
//static constexpr navy = Color(0.0000f, 0.0000f, 0.5020f, 1.0000f);
//static constexpr oldlace = Color(0.9922f, 0.9608f, 0.9020f, 1.0000f);
//static constexpr olive = Color(0.5020f, 0.5020f, 0.0000f, 1.0000f);
//static constexpr olivedrab = Color(0.4196f, 0.5569f, 0.1373f, 1.0000f);
//static constexpr orange = Color(1.0000f, 0.6471f, 0.0000f, 1.0000f);
//static constexpr orangered = Color(1.0000f, 0.2706f, 0.0000f, 1.0000f);
//static constexpr orchid = Color(0.8549f, 0.4392f, 0.8392f, 1.0000f);
//static constexpr palegoldenrod = Color(0.9333f, 0.9098f, 0.6667f, 1.0000f);
//static constexpr palegreen = Color(0.5961f, 0.9843f, 0.5961f, 1.0000f);
//static constexpr paleturquoise = Color(0.6863f, 0.9333f, 0.9333f, 1.0000f);
//static constexpr palevioletred = Color(0.8588f, 0.4392f, 0.5765f, 1.0000f);
//static constexpr papayawhip = Color(1.0000f, 0.9373f, 0.8353f, 1.0000f);
//static constexpr peachpuff = Color(1.0000f, 0.8549f, 0.7255f, 1.0000f);
//static constexpr peru = Color(0.8039f, 0.5216f, 0.2471f, 1.0000f);
//static constexpr pink = Color(1.0000f, 0.7529f, 0.7961f, 1.0000f);
//static constexpr plum = Color(0.8667f, 0.6275f, 0.8667f, 1.0000f);
//static constexpr powderblue = Color(0.6902f, 0.8784f, 0.9020f, 1.0000f);
//static constexpr purple = Color(0.5020f, 0.0000f, 0.5020f, 1.0000f);
//static constexpr red = Color(1.0000f, 0.0000f, 0.0000f, 1.0000f);
//static constexpr rosybrown = Color(0.7373f, 0.5608f, 0.5608f, 1.0000f);
//static constexpr royalblue = Color(0.2549f, 0.4118f, 0.8824f, 1.0000f);
//static constexpr saddlebrown = Color(0.5451f, 0.2706f, 0.0745f, 1.0000f);
//static constexpr salmon = Color(0.9804f, 0.5020f, 0.4471f, 1.0000f);
//static constexpr sandybrown = Color(0.9569f, 0.6431f, 0.3765f, 1.0000f);
//static constexpr seagreen = Color(0.1804f, 0.5451f, 0.3412f, 1.0000f);
//static constexpr seashell = Color(1.0000f, 0.9608f, 0.9333f, 1.0000f);
//static constexpr sienna = Color(0.6275f, 0.3216f, 0.1765f, 1.0000f);
//static constexpr silver = Color(0.7529f, 0.7529f, 0.7529f, 1.0000f);
//static constexpr skyblue = Color(0.5294f, 0.8078f, 0.9216f, 1.0000f);
//static constexpr slateblue = Color(0.4157f, 0.3529f, 0.8039f, 1.0000f);
//static constexpr slategray = Color(0.4392f, 0.5020f, 0.5647f, 1.0000f);
//static constexpr slategrey = Color(0.4392f, 0.5020f, 0.5647f, 1.0000f);
//static constexpr snow = Color(1.0000f, 0.9804f, 0.9804f, 1.0000f);
//static constexpr springgreen = Color(0.0000f, 1.0000f, 0.4980f, 1.0000f);
//static constexpr steelblue = Color(0.2745f, 0.5098f, 0.7059f, 1.0000f);
//static constexpr tan = Color(0.8235f, 0.7059f, 0.5490f, 1.0000f);
//static constexpr teal = Color(0.0000f, 0.5020f, 0.5020f, 1.0000f);
//static constexpr thistle = Color(0.8471f, 0.7490f, 0.8471f, 1.0000f);
//static constexpr tomato = Color(1.0000f, 0.3882f, 0.2784f, 1.0000f);
//static constexpr turquoise = Color(0.2510f, 0.8784f, 0.8157f, 1.0000f);
//static constexpr violet = Color(0.9333f, 0.5098f, 0.9333f, 1.0000f);
//static constexpr wheat = Color(0.9608f, 0.8706f, 0.7020f, 1.0000f);
//static constexpr white = Color(1.0000f, 1.0000f, 1.0000f, 1.0000f);
//static constexpr whitesmoke = Color(0.9608f, 0.9608f, 0.9608f, 1.0000f);
//static constexpr yellow = Color(1.0000f, 1.0000f, 0.0000f, 1.0000f);
//static constexpr yellowgreen = Color(0.6039f, 0.8039f, 0.1961f, 1.0000f);
//static constexpr transparent = Color(0.0000f, 0.0000f, 0.0000f, 0.0000f);
