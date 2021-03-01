// Project: libv.math, File: src/libv/math/vec_serial.hpp, Author: Császár Mátyás [Vader]

#pragma once

// hpp
#include <libv/math/vec.hpp>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

// TODO P5: Implement vec serialization based on vec 2,3,4 concepts, this solves glm too
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

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE

