// Project: libv.mt, File: src/libv/mt/mutex_spinlock.hpp

#pragma once

// std
#include <atomic>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------
// TODO P4: Find a better spinlock implementation

struct mutex_spinlock {
	std::atomic<bool> lock_ = {false};

public:
	inline mutex_spinlock() noexcept = default;
	mutex_spinlock(const mutex_spinlock&) = delete;
	mutex_spinlock& operator=(const mutex_spinlock&) & = delete;
	mutex_spinlock(mutex_spinlock&&) = delete;
	mutex_spinlock& operator=(mutex_spinlock&&) & = delete;

public:
	inline void lock() noexcept {
		while (true) {
			// Optimistically assume the lock is free on the first try
			if (!lock_.exchange(true, std::memory_order_acquire))
				return;

			// Wait for lock to be released without generating cache misses
			while (lock_.load(std::memory_order_relaxed))
				// Issue X86 PAUSE or ARM YIELD instruction to reduce contention between hyper-threads
				__builtin_ia32_pause();
		}
	}

	[[nodiscard]] inline bool try_lock() noexcept {
		// First do a relaxed load to check if lock is free in order to prevent
		// unnecessary cache misses if someone does while(!try_lock())
		return !lock_.load(std::memory_order_relaxed) &&
				!lock_.exchange(true, std::memory_order_acquire);
	}

	inline void unlock() noexcept {
		lock_.store(false, std::memory_order_release);
	}
};

} // namespace mt ----------------------------------------------------------------------------------

using mutex_spinlock = libv::mt::mutex_spinlock;

// -------------------------------------------------------------------------------------------------

} // namespace libv
