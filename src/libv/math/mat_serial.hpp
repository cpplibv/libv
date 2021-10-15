// Project: libv.math, File: src/libv/math/mat_serial.hpp

#pragma once

// hpp
#include <libv/math/vec.hpp>
// libv
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, size_t R, size_t C, typename T>
inline void serialize(Archive& ar, ::libv::mat_t<R, C, T>& var, const unsigned int) {
	for (size_t i = 0; i < R * C; i++)
		ar(var.data()[i]);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE
