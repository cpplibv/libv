// File: enable_memberwise.hpp Author: Vader Created on 2017. június 11., 19:00

#pragma once

// ext
#include <cereal/types/tuple.hpp>
// libv
#include <libv/meta/to_tuple.hpp>
// pro
#include <libv/serialization/serialization.hpp>


// -------------------------------------------------------------------------------------------------

namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

template <typename Archive, typename T, typename = typename T::libv_serialize_memberwise>
inline void LIBV_SERIALIZATION_SAVE_FUNCTION_NAME(Archive& ar, const T& object) {
	auto tuple = libv::meta::to_tuple(object);
	ar & LIBV_NVP(tuple);
}

template <typename Archive, typename T, typename = typename T::libv_serialize_memberwise>
inline void LIBV_SERIALIZATION_LOAD_FUNCTION_NAME(Archive& ar, T& object) {
	libv::meta::to_tuple_type_t<T> tuple;
	ar & LIBV_NVP(tuple);
	object = libv::meta::make_from_tuple_using_braces<T>(tuple);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE
