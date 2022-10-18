// Project: libv.mt, File: src/libv/mt/binary_latch.hpp

#pragma once

// std
#include <condition_variable>
#include <mutex>
// pro
#include <libv/mt/mutex_spinlock.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

class BinaryLatch {
	bool passed;

	mutable libv::mutex_spinlock mutex;
	mutable std::condition_variable_any cv;

public:
	inline explicit BinaryLatch(bool passed = false) :
		passed(passed) { }

public:
	inline void raise() {
		auto lock = std::unique_lock(mutex);
		passed = true;
		cv.notify_all();
	}

	inline void reset() {
		auto lock = std::unique_lock(mutex);
		passed = false;
	}

	inline void wait() const {
		auto lock = std::unique_lock(mutex);
		while (!passed)
			cv.wait(lock);
	}

	template<typename Rep, typename Period>
	inline bool wait_for(const std::chrono::duration<Rep, Period>& timeout) const {
		auto lock = std::unique_lock(mutex);
		return cv.wait_for(lock, timeout, [this]{ return passed; });
	}

	template<typename Clock, typename Duration>
	inline bool wait_until(const std::chrono::time_point<Clock, Duration>& attime) const {
		auto lock = std::unique_lock(mutex);
		return cv.wait_until(lock, attime, [this]{ return passed; }) == std::cv_status::no_timeout;
	}
};

using binary_latch = BinaryLatch;

namespace mt { // ----------------------------------------------------------------------------------

// TODO P5: Move everything to this alias
using binary_latch = ::libv::BinaryLatch;

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
