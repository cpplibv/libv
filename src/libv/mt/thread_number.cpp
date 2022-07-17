// Project: libv.mt, File: src/libv/mt/number.cpp

// hpp
#include <libv/mt/thread_number.hpp>
// std
#include <bit>
#include <thread>


namespace libv {

// -------------------------------------------------------------------------------------------------

uint64_t thread_number() noexcept {
	return std::bit_cast<uint64_t>(std::this_thread::get_id());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
