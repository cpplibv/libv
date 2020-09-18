// File: std_variant.hpp Author: Vader Created on 2017. június 11., 5:15

#pragma once

// std
#include <optional>
// pro
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T>
inline void save(Archive& ar, const std::optional<T>& optional) {
	const bool has_value = optional.has_value();
	ar & LIBV_NVP_NAMED("has_value", has_value);
	if (has_value)
		ar & LIBV_NVP_NAMED("value", optional.value());
}

template <typename Archive, typename T>
inline void load(Archive& ar, std::optional<T>& optional) {
	bool has_value;
	ar & LIBV_NVP_NAMED("has_value", has_value);
	if (!has_value) {
		optional.reset();
	} else {
		T value;
		ar & LIBV_NVP_NAMED("value", value);
		optional.emplace(std::move(value));
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE
