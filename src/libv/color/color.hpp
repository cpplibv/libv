// Project: libv.color, File: src/libv/color/color.hpp, Author: Császár Mátyás [Vader]

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
