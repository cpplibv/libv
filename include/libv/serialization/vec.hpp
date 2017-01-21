// File: Vec.hpp, Created on 2015. február 14. 23:11, Author: Vader

#pragma once

#include <boost/serialization/array.hpp>
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
template<typename Archive, size_t N, typename T>
inline void serialize(Archive& ar, ::libv::vec_t<N, T>& vec, const unsigned int) {
	ar & boost::serialization::make_array<T>(vec.ptr, N);
}

// -------------------------------------------------------------------------------------------------

// Disable vec_t<N, T> boost serialization attributes (class_id, tracking_level, version)
template <size_t N, typename T>
struct implementation_level<::libv::vec_t<N, T>> {
	typedef mpl::integral_c_tag tag;
	typedef mpl::int_<object_serializable> type;
	BOOST_STATIC_CONSTANT(int, value = implementation_level::type::value);
};

// Disable tracking of vec_t<N, T>
template <size_t N, typename T>
struct tracking_level<::libv::vec_t<N, T>> {
	typedef mpl::integral_c_tag tag;
	typedef mpl::int_<track_never> type;
	BOOST_STATIC_CONSTANT(int, value = tracking_level::type::value);
};


} // namespace serialization
} // namespace boost
