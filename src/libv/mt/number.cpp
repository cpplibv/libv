// Project: libv.mt, File: src/libv/mt/number.hpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/mt/number.hpp>
// libv
#include <libv/utility/bit_cast.hpp>
// std
#include <thread>


namespace libv {

// -------------------------------------------------------------------------------------------------

uint64_t thread_number() noexcept {
	return libv::bit_cast<uint64_t>(std::this_thread::get_id());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
