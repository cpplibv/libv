// Project: libv.serialization, File: src/libv/serialization/reflection.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/meta/reflection.hpp>
// pro
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T, typename = typename T::__libv_serialiaztion_enable_reflection_save>
inline void save(Archive& ar, const T& object, std::integral_constant<size_t, 2001> = {}) {
	libv::meta::foreach_member_nrp(object, [&](const auto& name, const auto& value) {
		ar & LIBV_NVP_NAMED(name, value);
	});
}

template <typename Archive, typename T, typename = typename T::__libv_serialiaztion_enable_reflection_load>
inline void load(Archive& ar, T& object, std::integral_constant<size_t, 2001> = {}) {
	libv::meta::foreach_member_nrp(object, [&](const auto& name, auto& value) {
		ar & LIBV_NVP_NAMED(name, value);
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE
