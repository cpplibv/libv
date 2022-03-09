// Project: libv.math, File: src/libv/math/constants.hpp

#pragma once

// std
#include <numbers>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T> static constexpr T rad_v       = static_cast<T>(57.295779513082320876800);
template <typename T> static constexpr T inv_sqrt2_v = static_cast<T>( 0.707106781186547524401);
template <typename T> static constexpr T tau_v       = static_cast<T>( 6.283185307179586476926); /// tau = 2 PI
template <typename T> static constexpr T golden_v    = std::numbers::phi_v<T>;

template <typename T> static constexpr T e_v          = std::numbers::e_v<T>;
template <typename T> static constexpr T egamma_v     = std::numbers::egamma_v<T>;
template <typename T> static constexpr T inv_pi_v     = std::numbers::inv_pi_v<T>;
template <typename T> static constexpr T inv_sqrt3_v  = std::numbers::inv_sqrt3_v<T>;
template <typename T> static constexpr T inv_sqrtpi_v = std::numbers::inv_sqrtpi_v<T>;
template <typename T> static constexpr T ln10_v       = std::numbers::ln10_v<T>;
template <typename T> static constexpr T ln2_v        = std::numbers::ln2_v<T>;
template <typename T> static constexpr T log10e_v     = std::numbers::log10e_v<T>;
template <typename T> static constexpr T log2e_v      = std::numbers::log2e_v<T>;
template <typename T> static constexpr T phi_v        = std::numbers::phi_v<T>;
template <typename T> static constexpr T pi_v         = std::numbers::pi_v<T>;
template <typename T> static constexpr T sqrt2_v      = std::numbers::sqrt2_v<T>;
template <typename T> static constexpr T sqrt3_v      = std::numbers::sqrt3_v<T>;

static constexpr float rad_f        = rad_v<float>;
static constexpr float e_f          = e_v<float>;
static constexpr float egamma_f     = egamma_v<float>;
static constexpr float inv_pi_f     = inv_pi_v<float>;
static constexpr float inv_sqrt2_f  = inv_sqrt2_v<float>;
static constexpr float inv_sqrt3_f  = inv_sqrt3_v<float>;
static constexpr float inv_sqrtpi_f = inv_sqrtpi_v<float>;
static constexpr float ln10_f       = ln10_v<float>;
static constexpr float ln2_f        = ln2_v<float>;
static constexpr float log10e_f     = log10e_v<float>;
static constexpr float log2e_f      = log2e_v<float>;
static constexpr float phi_f        = phi_v<float>;
static constexpr float pi_f         = pi_v<float>;
static constexpr float sqrt2_f      = sqrt2_v<float>;
static constexpr float sqrt3_f      = sqrt3_v<float>;
static constexpr float tau_f        = tau_v<float>; /// tau = 2 PI
static constexpr float golden_f     = golden_v<float>;

static constexpr double rad_d        = rad_v<double>;
static constexpr double e_d          = e_v<double>;
static constexpr double egamma_d     = egamma_v<double>;
static constexpr double inv_pi_d     = inv_pi_v<double>;
static constexpr double inv_sqrt2_d  = inv_sqrt2_v<double>;
static constexpr double inv_sqrt3_d  = inv_sqrt3_v<double>;
static constexpr double inv_sqrtpi_d = inv_sqrtpi_v<double>;
static constexpr double ln10_d       = ln10_v<double>;
static constexpr double ln2_d        = ln2_v<double>;
static constexpr double log10e_d     = log10e_v<double>;
static constexpr double log2e_d      = log2e_v<double>;
static constexpr double phi_d        = phi_v<double>;
static constexpr double pi_d         = pi_v<double>;
static constexpr double sqrt2_d      = sqrt2_v<double>;
static constexpr double sqrt3_d      = sqrt3_v<double>;
static constexpr double tau_d        = tau_v<double>; /// tau = 2 PI
static constexpr double golden_d     = golden_v<double>;

using default_constants_type = float;

static constexpr default_constants_type rad        = rad_v<default_constants_type>;
static constexpr default_constants_type e          = e_v<default_constants_type>;
static constexpr default_constants_type egamma     = egamma_v<default_constants_type>;
static constexpr default_constants_type inv_pi     = inv_pi_v<default_constants_type>;
static constexpr default_constants_type inv_sqrt2  = inv_sqrt2_v<default_constants_type>;
static constexpr default_constants_type inv_sqrt3  = inv_sqrt3_v<default_constants_type>;
static constexpr default_constants_type inv_sqrtpi = inv_sqrtpi_v<default_constants_type>;
static constexpr default_constants_type ln10       = ln10_v<default_constants_type>;
static constexpr default_constants_type ln2        = ln2_v<default_constants_type>;
static constexpr default_constants_type log10e     = log10e_v<default_constants_type>;
static constexpr default_constants_type log2e      = log2e_v<default_constants_type>;
static constexpr default_constants_type phi        = phi_v<default_constants_type>;
static constexpr default_constants_type pi         = pi_v<default_constants_type>;
static constexpr default_constants_type sqrt2      = sqrt2_v<default_constants_type>;
static constexpr default_constants_type sqrt3      = sqrt3_v<default_constants_type>;
static constexpr default_constants_type tau        = tau_v<default_constants_type>; /// tau = 2 PI
static constexpr default_constants_type golden     = golden_v<default_constants_type>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
