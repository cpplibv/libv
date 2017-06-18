// File: enable_memberwise.hpp Author: Vader Created on 2017. június 11., 19:00

#pragma once

// libv
#include <libv/meta/member_access.hpp>
// pro
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T, typename = typename T::__libv_serialiaztion_enable_member>
inline void save(Archive& ar, const T& object, std::integral_constant<size_t, 2001> = {}) {
	libv::meta::foreach_member_nvp(object, [&](const auto& name, const auto& value) {
		ar & LIBV_NVP_NAMED(name, value);
	});
}

template <typename Archive, typename T, typename = typename T::__libv_serialiaztion_enable_member>
inline void load(Archive& ar, T& object, std::integral_constant<size_t, 2001> = {}) {
	libv::meta::foreach_member_nvp(object, [&](const auto& name, auto& value) {
		ar & LIBV_NVP_NAMED(name, value);
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE
