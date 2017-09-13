// File: enable_memberwise.hpp Author: Vader Created on 2017. június 11., 19:00

#pragma once

// ext
#include <cereal/types/tuple.hpp>
// libv
#include <libv/meta/to_tuple.hpp>
// pro
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T, typename = typename T::__libv_serialiaztion_enable_memberwise>
inline void save(Archive& ar, const T& object, std::integral_constant<size_t, 2000> = {}) {
	auto tuple = libv::meta::to_tuple(object);
	ar & LIBV_NVP(tuple);
	// TODO P2: direct serialization of tuple members
}

template <typename Archive, typename T, typename = typename T::__libv_serialiaztion_enable_memberwise>
inline void load(Archive& ar, T& object, std::integral_constant<size_t, 2000> = {}) {
	libv::meta::to_tuple_type_t<T> tuple;
	ar & LIBV_NVP(tuple);
	// TODO P2: direct serialization of tuple members
	object = libv::meta::make_from_tuple_using_braces<T>(tuple);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE
