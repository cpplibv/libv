// Project: libv.mt, File: src/libv/mt/number.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/bit_cast.hpp>
// std
#include <thread>


namespace libv {

// -------------------------------------------------------------------------------------------------

inline uint64_t thread_number() {
	return libv::bit_cast<uint64_t>(std::this_thread::get_id());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
