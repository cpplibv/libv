// Project: libv.utility, File: src/libv/utility/random/xoroshiro128_serial.hpp

#pragma once

// hpp
#include <libv/utility/random/xoroshiro128.hpp>
// libv
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive>
constexpr inline void save(Archive& ar, const ::libv::xoroshiro128& var) noexcept {
	ar & LIBV_NVP_NAMED("s0", var.state_s0());
	ar & LIBV_NVP_NAMED("s1", var.state_s1());
}

template <typename Archive>
constexpr inline void load(Archive& ar, ::libv::xoroshiro128& var) noexcept {
	uint64_t s0;
	uint64_t s1;
	ar & LIBV_NVP_NAMED("s0", s0);
	ar & LIBV_NVP_NAMED("s1", s1);
	var.state_s0(s0);
	var.state_s1(s1);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE
