// File: glm.hpp, Created on 2015. február 14. 23:50, Author: Vader

#pragma once

#include <vl/serialization/nvp.hpp>
#include <boost/serialization/array.hpp>
#include <glm/glm.hpp>

namespace boost {
namespace serialization {

// -------------------------------------------------------------------------------------------------

template<typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tvec2<T, P>& var, const unsigned int) {
	ar & VL_NVP_NAMED("x", var.x);
	ar & VL_NVP_NAMED("y", var.y);
}
template<typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tvec3<T, P>& var, const unsigned int) {
	ar & VL_NVP_NAMED("x", var.x);
	ar & VL_NVP_NAMED("y", var.y);
	ar & VL_NVP_NAMED("z", var.z);
}
template<typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tvec4<T, P>& var, const unsigned int) {
	ar & VL_NVP_NAMED("x", var.x);
	ar & VL_NVP_NAMED("y", var.y);
	ar & VL_NVP_NAMED("z", var.z);
	ar & VL_NVP_NAMED("w", var.w);
}
template<typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tmat2x2<T, P>& var, const unsigned int) {
	ar & VL_NVP_NAMED("mat2x2", boost::serialization::make_array(&var[0], var.length()));
}
template<typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tmat3x3<T, P>& var, const unsigned int) {
	ar & VL_NVP_NAMED("mat3x3", boost::serialization::make_array(&var[0], var.length()));
}
template<typename Archive, typename T, ::glm::precision P>
inline void serialize(Archive& ar, ::glm::tmat4x4<T, P>& var, const unsigned int) {
	ar & VL_NVP_NAMED("mat4x4", boost::serialization::make_array(&var[0], var.length()));
}

// -------------------------------------------------------------------------------------------------

} //namespace serialization
} //namespace boost