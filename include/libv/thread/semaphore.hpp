// File: Semaphore.hpp, Created on 2015. január 11. 6:05, Author: Vader

#pragma once

// std
#include <condition_variable>
#include <mutex>


namespace libv {

// -------------------------------------------------------------------------------------------------

class Semaphore {
	bool passed;
	mutable std::mutex mutex;
	mutable std::condition_variable cv;

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

	inline explicit Semaphore(bool passed = false) : passed(passed) { }
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
