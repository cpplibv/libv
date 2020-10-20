// Project: libv.mt, File: src/libv/mt/binary_latch.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <condition_variable>
#include <mutex>


namespace libv {

// -------------------------------------------------------------------------------------------------

class BinaryLatch {
	bool passed;

	mutable std::mutex mutex;
	mutable std::condition_variable cv;

public:
	inline explicit BinaryLatch(bool passed = false) :
		passed(passed) { }

public:
	inline void raise() {
		std::unique_lock<std::mutex> lock(mutex);
		passed = true;
		cv.notify_all();
	}

	inline void reset() {
		std::unique_lock<std::mutex> lock(mutex);
		passed = false;
	}

	inline void wait() const {
		std::unique_lock<std::mutex> lock(mutex);
		while (!passed)
			cv.wait(lock);
	}

	template<typename Rep, typename Period>
	inline bool wait_for(const std::chrono::duration<Rep, Period>& timeout) const {
		std::unique_lock<std::mutex> lock(mutex);
		return cv.wait_for(lock, timeout, [this]{ return passed; });
	}

	template<typename Clock, typename Duration>
	inline bool wait_until(const std::chrono::time_point<Clock, Duration>& attime) const {
		std::unique_lock<std::mutex> lock(mutex);
		return cv.wait_until(lock, attime, [this]{ return passed; }) == std::cv_status::no_timeout;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
