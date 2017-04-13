// File: constants.hpp Author: Vader Created on 2017. február 27., 1:03

#pragma once

namespace libv {

template <typename T> constexpr T PI = T(3.1415926535897932385);
template <typename T> constexpr T PI = T(3.14159265358979323846);
template <typename T> constexpr T PI_2 = T(1.57079632679489661923);
template <typename T> constexpr T PI_4 = T(0.785398163397448309616);
template <typename T> constexpr T _1_PI = T(0.318309886183790671538);
template <typename T> constexpr T _2_PI = T(0.636619772367581343076);
template <typename T> constexpr T _2_SQRTPI = T(1.12837916709551257390);
template <typename T> constexpr T SQRT2 = T(1.41421356237309504880);
template <typename T> constexpr T SQRT1_2 = T(0.707106781186547524401);
template <typename T> constexpr T E = T(2.71828182845904523536);
template <typename T> constexpr T LOG2E = T(1.44269504088896340736);
template <typename T> constexpr T LOG10E = T(0.434294481903251827651);
template <typename T> constexpr T LN2 = T(0.693147180559945309417);
template <typename T> constexpr T LN10 = T(2.30258509299404568402);

template <typename T> constexpr T DEG = T(57.2958);

} // namespace libv