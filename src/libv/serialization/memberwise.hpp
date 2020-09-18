// Project: libv.serialization, File: src/libv/serialization/memberwise.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <cereal/types/tuple.hpp>
// libv
#include <libv/meta/to_tuple.hpp>
// pro
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T, typename = typename T::__libv_serialiaztion_enable_memberwise_save>
inline void save(Archive& ar, const T& object, std::integral_constant<size_t, 2000> = {}) {
	auto tuple = libv::meta::to_tuple(object);
	cereal::serialize(ar, tuple);
}

template <typename Archive, typename T, typename = typename T::__libv_serialiaztion_enable_memberwise_load>
inline void load(Archive& ar, T& object, std::integral_constant<size_t, 2000> = {}) {
	libv::meta::to_tuple_type_t<T> tuple;
	cereal::serialize(ar, tuple);
	object = libv::meta::make_from_tuple_using_braces<T>(tuple);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE
