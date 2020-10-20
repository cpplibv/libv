// Project: libv.thread, File: src/libv/thread/hardware_concurrency.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <thread>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] inline unsigned int hardware_concurrency_or(unsigned int fallback_num) noexcept {
	const auto hint = std::thread::hardware_concurrency();
	return hint == 0 ? fallback_num : hint;
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
