// Project: libv.thread, File: src/libv/thread/mutex_noop.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace thread {

// -------------------------------------------------------------------------------------------------

struct mutex_noop {
	constexpr inline void lock() noexcept {}
	constexpr inline bool try_lock() noexcept {
		return true;
	}
	constexpr inline void unlock() noexcept {}

	constexpr inline void lock_shared() noexcept {}
	constexpr inline bool try_lock_shared() noexcept {
		return true;
	}
	constexpr inline void unlock_shared() noexcept {}
};

// -------------------------------------------------------------------------------------------------

} // namespace thread
} // namespace libv
