// File: constants.hpp Author: Vader Created on 2017. február 27., 1:03

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T> static constexpr T PI        = static_cast<T>( 3.14159265358979323846 );
template <typename T> static constexpr T PI_2      = static_cast<T>( 1.57079632679489661923 );
template <typename T> static constexpr T PI_4      = static_cast<T>( 0.785398163397448309616);
template <typename T> static constexpr T _1_PI     = static_cast<T>( 0.318309886183790671538);
template <typename T> static constexpr T _2_PI     = static_cast<T>( 0.636619772367581343076);
template <typename T> static constexpr T _2_SQRTPI = static_cast<T>( 1.12837916709551257390 );
template <typename T> static constexpr T SQRT2     = static_cast<T>( 1.41421356237309504880 );
template <typename T> static constexpr T SQRT1_2   = static_cast<T>( 0.707106781186547524401);
template <typename T> static constexpr T E         = static_cast<T>( 2.71828182845904523536 );
template <typename T> static constexpr T LOG2E     = static_cast<T>( 1.44269504088896340736 );
template <typename T> static constexpr T LOG10E    = static_cast<T>( 0.434294481903251827651);
template <typename T> static constexpr T LN2       = static_cast<T>( 0.693147180559945309417);
template <typename T> static constexpr T LN10      = static_cast<T>( 2.30258509299404568402 );
template <typename T> static constexpr T DEG       = static_cast<T>(57.29577951308232087680 );

static constexpr float PI_f       = PI        <float>;
static constexpr float PI_2_f     = PI_2      <float>;
static constexpr float PI_4_f     = PI_4      <float>;
static constexpr float _1_PI_f    = _1_PI     <float>;
static constexpr float _2_PI_f    = _2_PI     <float>;
static constexpr float _2_SQRTPI_f= _2_SQRTPI <float>;
static constexpr float SQRT2_f    = SQRT2     <float>;
static constexpr float SQRT1_2_f  = SQRT1_2   <float>;
static constexpr float E_f        = E         <float>;
static constexpr float LOG2E_f    = LOG2E     <float>;
static constexpr float LOG10E_f   = LOG10E    <float>;
static constexpr float LN2_f      = LN2       <float>;
static constexpr float LN10_f     = LN10      <float>;
static constexpr float DEG_f      = DEG       <float>;

static constexpr double PI_d       = PI        <double>;
static constexpr double PI_2_d     = PI_2      <double>;
static constexpr double PI_4_d     = PI_4      <double>;
static constexpr double _1_PI_d    = _1_PI     <double>;
static constexpr double _2_PI_d    = _2_PI     <double>;
static constexpr double _2_SQRTPI_d= _2_SQRTPI <double>;
static constexpr double SQRT2_d    = SQRT2     <double>;
static constexpr double SQRT1_2_d  = SQRT1_2   <double>;
static constexpr double E_d        = E         <double>;
static constexpr double LOG2E_d    = LOG2E     <double>;
static constexpr double LOG10E_d   = LOG10E    <double>;
static constexpr double LN2_d      = LN2       <double>;
static constexpr double LN10_d     = LN10      <double>;
static constexpr double DEG_d      = DEG       <double>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
