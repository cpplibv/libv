// File: Vec.hpp, Created on 2015. február 14. 23:11, Author: Vader

#pragma once

#include "vl/serialization/nvp.hpp"
#include "vl/vec.hpp"

namespace boost {
namespace serialization {
template<typename Archive, typename T>
inline void serialize(Archive& ar, ::vl::vec2_t<T>& var, const unsigned int) {
	ar & VL_NVP_NAMED("x", var.x);
	ar & VL_NVP_NAMED("y", var.y);
}
template<typename Archive, typename T>
inline void serialize(Archive& ar, ::vl::vec3_t<T>& var, const unsigned int) {
	ar & VL_NVP_NAMED("x", var.x);
	ar & VL_NVP_NAMED("y", var.y);
	ar & VL_NVP_NAMED("z", var.z);
}
template<typename Archive, typename T>
inline void serialize(Archive& ar, ::vl::vec4_t<T>& var, const unsigned int) {
	ar & VL_NVP_NAMED("x", var.x);
	ar & VL_NVP_NAMED("y", var.y);
	ar & VL_NVP_NAMED("z", var.z);
	ar & VL_NVP_NAMED("w", var.w);
}
} //namespace serialization
} //namespace boost