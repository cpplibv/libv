// Project: libv.serial, File: src/libv/serial/types/std_optional.hpp

#pragma once

// std
#include <optional>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T>
inline void LIBV_FUNCTION_NAME_SAVE(Archive& ar, const std::optional<T>& optional) {
	const bool has_value = optional.has_value();
	ar.nvp("has_value", has_value);
	if (has_value)
		ar.nvp("value", optional.value());
}

template <typename Archive, typename T>
inline void LIBV_FUNCTION_NAME_LOAD(Archive& ar, std::optional<T>& optional) {
	bool has_value;
	ar.nvp("has_value", has_value);
	if (!has_value) {
		optional.reset();
	} else {
		optional.emplace();
		ar.nvp("value", *optional);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE
