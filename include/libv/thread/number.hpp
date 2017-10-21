// File: number.hpp - Created on 2017.10.21. 02:36 - Author: Vader

#pragma once

// std
#include <thread>


namespace libv {

// -------------------------------------------------------------------------------------------------

inline uint64_t thread_number() {
	static_assert(sizeof (std::thread::id) <= sizeof (uint64_t), "thread::id size is greater then uint64_t");
	auto id = std::this_thread::get_id();
	return reinterpret_cast<uint64_t&>(id);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
