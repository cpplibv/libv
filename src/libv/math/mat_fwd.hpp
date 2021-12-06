// Project: libv.math, File: src/libv/math/mat_fwd.hpp

#pragma once

// std
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <std::size_t R, std::size_t C, typename T>
struct mat_t;

// aliases -----------------------------------------------------------------------------------------

template <typename T> using mat2x2_t = mat_t<2, 2, T>;
template <typename T> using mat2x3_t = mat_t<2, 3, T>;
template <typename T> using mat2x4_t = mat_t<2, 4, T>;
template <typename T> using mat3x2_t = mat_t<3, 2, T>;
template <typename T> using mat3x3_t = mat_t<3, 3, T>;
template <typename T> using mat3x4_t = mat_t<3, 4, T>;
template <typename T> using mat4x2_t = mat_t<4, 2, T>;
template <typename T> using mat4x3_t = mat_t<4, 3, T>;
template <typename T> using mat4x4_t = mat_t<4, 4, T>;

template <typename T> using mat2_t = mat2x2_t<T>;
template <typename T> using mat3_t = mat3x3_t<T>;
template <typename T> using mat4_t = mat4x4_t<T>;

using mat2x2f = mat2x2_t<float>;
using mat2x3f = mat2x3_t<float>;
using mat2x4f = mat2x4_t<float>;
using mat2x2d = mat2x2_t<double>;
using mat2x3d = mat2x3_t<double>;
using mat2x4d = mat2x4_t<double>;
using mat3x2f = mat3x2_t<float>;
using mat3x3f = mat3x3_t<float>;
using mat3x4f = mat3x4_t<float>;
using mat3x2d = mat3x2_t<double>;
using mat3x3d = mat3x3_t<double>;
using mat3x4d = mat3x4_t<double>;
using mat4x2f = mat4x2_t<float>;
using mat4x3f = mat4x3_t<float>;
using mat4x4f = mat4x4_t<float>;
using mat4x2d = mat4x2_t<double>;
using mat4x3d = mat4x3_t<double>;
using mat4x4d = mat4x4_t<double>;

using mat2f = mat2x2_t<float>;
using mat3f = mat3x3_t<float>;
using mat4f = mat4x4_t<float>;
using mat2d = mat2x2_t<double>;
using mat3d = mat3x3_t<double>;
using mat4d = mat4x4_t<double>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
