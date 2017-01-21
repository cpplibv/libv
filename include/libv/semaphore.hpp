// File: Semaphore.hpp, Created on 2015. január 11. 6:05, Author: Vader

#pragma once

#include <atomic>
#include <condition_variable>

namespace libv {

class Semaphore {
	bool passed;
	mutable std::mutex mutex;
	mutable std::condition_variable cv;
public:
	void raise() {
		std::lock_guard<std::mutex> lock(mutex);
		passed = true;
		cv.notify_all();
	}
	void reset() {
		std::lock_guard<std::mutex> lock(mutex);
		passed = false;
	}
	void wait() const {
		std::unique_lock<std::mutex> lock(mutex);
		while (!passed)
			cv.wait(lock);
	}
	Semaphore(bool passed = false) : passed(passed) { }
};

} // namespace libv