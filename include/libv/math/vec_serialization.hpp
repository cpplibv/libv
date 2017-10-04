// File: Vec.hpp, Created on 2015. február 14. 23:11, Author: Vader

#pragma once

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

// TODO P5: Implement vec serialization with N dim
//template <typename Archive, size_t N, typename T>
//inline void serialize(Archive& ar, ::libv::vec_t<N, T>& vec, const unsigned int) {
//	ar & boost::serialization::make_array<T>(vec.ptr, N);
//}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE

