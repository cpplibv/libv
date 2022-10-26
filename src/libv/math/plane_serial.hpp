// Project: libv.math, File: src/libv/math/plane_serial.hpp

#pragma once

// hpp
#include <libv/math/plane.hpp>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename T>
inline void serialize(Archive& ar, ::libv::plane_t<T>& var) {
	ar & LIBV_NVP_NAMED("nx", var.normal.x);
	ar & LIBV_NVP_NAMED("ny", var.normal.y);
	ar & LIBV_NVP_NAMED("nz", var.normal.z);
	ar & LIBV_NVP_NAMED("d", var.dist);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE

