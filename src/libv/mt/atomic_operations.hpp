// Project: libv.mt, File: src/libv/mt/atomic_operations.hpp

#pragma once

// std
#include <atomic>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

template <typename T>
inline void atomic_max(std::atomic<T>& max_value, const T value) noexcept {
	T prev_value = max_value;
	while (prev_value < value && !max_value.compare_exchange_weak(prev_value, value)) {}
}

template <typename T>
inline void atomic_min(std::atomic<T>& min_value, const T value) noexcept {
	T prev_value = min_value;
	while (prev_value > value && !min_value.compare_exchange_weak(prev_value, value)) {}
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
