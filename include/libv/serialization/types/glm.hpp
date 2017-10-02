// File: glm.hpp, Created on 2015. február 14. 23:50, Author: Vader

#pragma once

#include <libv/serialization/serialization.hpp>
#include <boost/serialization/array.hpp>
#include <glm/glm.hpp>

namespace boost {
namespace serialization {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tvec2<T, P>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
}
template <typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tvec3<T, P>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
	ar & LIBV_NVP_NAMED("z", var.z);
}
template <typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tvec4<T, P>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
	ar & LIBV_NVP_NAMED("z", var.z);
	ar & LIBV_NVP_NAMED("w", var.w);
}
template <typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tmat2x2<T, P>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("mat2x2", boost::serialization::make_array(&var[0], var.length()));
}
template <typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tmat3x3<T, P>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("mat3x3", boost::serialization::make_array(&var[0], var.length()));
}
template <typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tmat4x4<T, P>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("mat4x4", boost::serialization::make_array(&var[0], var.length()));
}

// -------------------------------------------------------------------------------------------------

} // namespace serialization
} // namespace boost

<<< this file from ^^^ that to vvv that

// hpp
#include <libv/math/vec.hpp>
// pro
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T>
inline void serialize(Archive& ar, ::libv::vec2_t<T>& var) {
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
}
template <typename Archive, typename T>
inline void serialize(Archive& ar, ::libv::vec3_t<T>& var) {
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
	ar & LIBV_NVP_NAMED("z", var.z);
}
template <typename Archive, typename T>
inline void serialize(Archive& ar, ::libv::vec4_t<T>& var) {
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
	ar & LIBV_NVP_NAMED("z", var.z);
	ar & LIBV_NVP_NAMED("w", var.w);
}
//template <typename Archive, size_t N, typename T>
//inline void serialize(Archive& ar, ::libv::mat_t<N, T>& var, const unsigned int) {
//	for (size_t i = 0; i < N * N; ++i)
//		ar(::libv::value_ptr(var)[i]);
//}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE

