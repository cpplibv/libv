// Project: libv.math, File: src/libv/math/ray_serial.hpp

#pragma once

// hpp
#include <libv/math/ray.hpp>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, std::size_t N, typename T>
inline void serialize(Archive& ar, ::libv::ray_t<N, T>& var) {
	ar & LIBV_NVP_NAMED("position", var.position);
	ar & LIBV_NVP_NAMED("direction", var.direction);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE

