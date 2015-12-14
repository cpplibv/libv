// File: Semaphore.hpp, Created on 2015. január 11. 6:05, Author: Vader

#pragma once

#include <mutex>
#include <condition_variable>

namespace libv {

class Semaphore {
	bool passed;
	std::mutex mutex;
	std::condition_variable cv;
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
	void wait() {
		std::unique_lock<std::mutex> lock(mutex);
		while(!passed)
			cv.wait(lock);
	}

	Semaphore(bool passed = false) : passed(passed) { }
};

} //namespace libv