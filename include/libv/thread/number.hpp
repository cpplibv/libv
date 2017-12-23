// File: number.hpp - Created on 2017.10.21. 02:36 - Author: Vader

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
