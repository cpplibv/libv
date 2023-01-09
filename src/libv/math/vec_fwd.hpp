// Project: libv.math, File: src/libv/math/vec_fwd.hpp

#pragma once

// std
#include <cstdint>


namespace libv {

// vec_t forward -----------------------------------------------------------------------------------

template <std::size_t N, typename T>
struct vec_t;

// aliases -----------------------------------------------------------------------------------------

template <typename T> using vec2_t = vec_t<2, T>;
template <typename T> using vec3_t = vec_t<3, T>;
template <typename T> using vec4_t = vec_t<4, T>;

using vec2b = vec2_t<bool>;
using vec3b = vec3_t<bool>;
using vec4b = vec4_t<bool>;
// using vec2h = vec2_t<short float>;
// using vec3h = vec3_t<short float>;
// using vec4h = vec4_t<short float>;
using vec2f = vec2_t<float>;
using vec3f = vec3_t<float>;
using vec4f = vec4_t<float>;
using vec2d = vec2_t<double>;
using vec3d = vec3_t<double>;
using vec4d = vec4_t<double>;
using vec2c = vec2_t<int8_t>;
using vec3c = vec3_t<int8_t>;
using vec4c = vec4_t<int8_t>;
using vec2uc = vec2_t<uint8_t>;
using vec3uc = vec3_t<uint8_t>;
using vec4uc = vec4_t<uint8_t>;
using vec2s = vec2_t<int16_t>;
using vec3s = vec3_t<int16_t>;
using vec4s = vec4_t<int16_t>;
using vec2us = vec2_t<uint16_t>;
using vec3us = vec3_t<uint16_t>;
using vec4us = vec4_t<uint16_t>;
using vec2i = vec2_t<int32_t>;
using vec3i = vec3_t<int32_t>;
using vec4i = vec4_t<int32_t>;
using vec2ui = vec2_t<uint32_t>;
using vec3ui = vec3_t<uint32_t>;
using vec4ui = vec4_t<uint32_t>;
using vec2l = vec2_t<int64_t>;
using vec3l = vec3_t<int64_t>;
using vec4l = vec4_t<int64_t>;
using vec2ul = vec2_t<uint64_t>;
using vec3ul = vec3_t<uint64_t>;
using vec4ul = vec4_t<uint64_t>;
using vec2z = vec2_t<std::size_t>;
using vec3z = vec3_t<std::size_t>;
using vec4z = vec4_t<std::size_t>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
