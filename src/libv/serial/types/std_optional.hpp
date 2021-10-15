// Project: libv.serial, File: src/libv/serial/types/std_optional.hpp

#pragma once

// std
#include <optional>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

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

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE
