// File: Vec.hpp, Created on 2014. január 7. 7:08, Author: Vader

#pragma once

#include <cmath>
#include <cassert>
#include <ostream>

//TODO P1: glm implicit cast for vec
//TODO P1: vec implicit cast for glm
//TODO P5: conditional noexcept based on arithmetic beetween components

namespace vl {
template <typename T>
struct vec2_t;
template <typename T>
struct vec3_t;
template <typename T>
struct vec4_t;

#undef VEC_DIM
#define VEC_DIM 2
#include "detail/vec.inl"
#undef VEC_DIM
#define VEC_DIM 3
#include "detail/vec.inl"
#undef VEC_DIM
#define VEC_DIM 4
#include "detail/vec.inl"

template <typename T>
vl::vec2_t<T> maxByComponents(const vl::vec2_t<T>& a, const vl::vec2_t<T>& b) {
	return vl::vec2_t<T>(std::max(a.x, b.x), std::max(a.y, b.y));
}

template <typename T>
vl::vec3_t<T> maxByComponents(const vl::vec3_t<T>& a, const vl::vec3_t<T>& b) {
	return vl::vec3_t<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

template <typename T>
vl::vec4_t<T> maxByComponents(const vl::vec4_t<T>& a, const vl::vec4_t<T>& b) {
	return vl::vec4_t<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
}

typedef vec2_t<float> vec2;
typedef vec3_t<float> vec3;
typedef vec4_t<float> vec4;
typedef vec2_t<double> dvec2;
typedef vec3_t<double> dvec3;
typedef vec4_t<double> dvec4;
typedef vec2_t<int32_t> ivec2;
typedef vec3_t<int32_t> ivec3;
typedef vec4_t<int32_t> ivec4;
} //namespace vl