// Project: libv.math, File: src/libv/math/quat_serial.hpp

#pragma once

// hpp
#include <libv/math/quat.hpp>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T>
inline void serialize(Archive& ar, ::libv::quat_t<T>& var) {
	ar & LIBV_NVP_NAMED("w", var.w); // w is the first for quaternions
	ar & LIBV_NVP_NAMED("x", var.x);
	ar & LIBV_NVP_NAMED("y", var.y);
	ar & LIBV_NVP_NAMED("z", var.z);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE

