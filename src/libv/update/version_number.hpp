// Project: libv.update, File: src/libv/update/version_number.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstdint>
//#include <ostream>
//// libv
//#include <libv/utility/enum.hpp>
//#include <libv/utility/extract_bits.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

enum class version_number : uint64_t {
//  IDEAS:
//	uninitialized = 0,
//	0123456789012345678 // 19 digit
//	-----YYYYMMDDhhmmss
//  -------------NN0000
//
//  16 + 16 + 16 + 16 bit as m.m.p.b
};

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

//struct version_info {
//	version_number number;
//	version version;
//	std::string version_name;
//};
//
//struct build_info {
//  std::string compiler;
//  std::string build_type;
//  std::chrono::system_clock::time_point build_time;
//	std::optional<libv::hash::SHA1> git_hash;
//};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
