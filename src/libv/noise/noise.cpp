// Created by dbobula on 2/13/2022.

#include <libv/noise/noise.hpp>
#include <libv/noise/noise_util.hpp>

#include <cmath>


//namespace libv {
//
//float noise_value(int seed, float x, float y) noexcept {
//	auto x0 = static_cast<int32_t>(std::floor(x));
//	auto y0 = static_cast<int32_t>(std::floor(y));
//
//	float xs = hermiteInterp(static_cast<float>(x - x0));
//	float ys = hermiteInterp(static_cast<float>(y - y0));
//
//	x0 *= PrimeX;
//	y0 *= PrimeY;
//	int x1 = x0 + PrimeX;
//	int y1 = y0 + PrimeY;
//
//	float xf0 = lerp(ValCoord(seed, x0, y0), ValCoord(seed, x1, y0), xs);
//	float xf1 = lerp(ValCoord(seed, x0, y1), ValCoord(seed, x1, y1), xs);
//
//	return lerp(xf0, xf1, ys);
//}
//
//} // namespace libv
