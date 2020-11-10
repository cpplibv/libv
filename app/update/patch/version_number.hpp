// Project: libv, File: app/update/patch/version_number.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstdint>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

enum class version_number : uint64_t {
//  IDEAS:
//	uninitialized = 0,
//	0123456789012345678 // 19 digit
//	-----YYYYMMDDhhmmss
//  -------------NN0000
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
