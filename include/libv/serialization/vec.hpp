// File: Vec.hpp, Created on 2015. február 14. 23:11, Author: Vader

#pragma once

#include <libv/serialization/nvp.hpp>
#include <libv/vec.hpp>

namespace boost {
namespace serialization {
template<typename Archive, typename T>
inline void serialize(Archive& ar, ::libv::vec2_t<T>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
}
template<typename Archive, typename T>
inline void serialize(Archive& ar, ::libv::vec3_t<T>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
	ar & LIBV_NVP_NAMED("z", var.z);
}
template<typename Archive, typename T>
inline void serialize(Archive& ar, ::libv::vec4_t<T>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
	ar & LIBV_NVP_NAMED("z", var.z);
	ar & LIBV_NVP_NAMED("w", var.w);
}
} //namespace serialization
} //namespace boost