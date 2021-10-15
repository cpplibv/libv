// Project: libv.update, File: src/libv/update/version.hpp

#pragma once

// std
#include <cstdint>
#include <ostream>
// libv
#include <libv/utility/extract_bits.hpp>
#include <libv/utility/to_underlying.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

//struct version {
//	uint32_t major = 0;
//	uint32_t minor = 0;
//	uint32_t patch = 0;
//	uint32_t build = 0;
//
//	// implicit
//	version(version_number nv) noexcept :
//		major(libv::extract_high_bits<uint32_t,  0, 16>(libv::to_value(nv))),
//		minor(libv::extract_high_bits<uint32_t, 16, 16>(libv::to_value(nv))),
//		patch(libv::extract_high_bits<uint32_t, 32, 16>(libv::to_value(nv))),
//		build(libv::extract_high_bits<uint32_t, 48, 16>(libv::to_value(nv))) {}
//
//  operator<=>
//
//	friend std::ostream& operator<<(std::ostream& os, const version& var) {
//		return os << var.major << '.' << var.minor << '.' << var.patch << '.' << var.build;
//	}
//};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
