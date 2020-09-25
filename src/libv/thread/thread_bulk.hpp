// Created by Vader on 2020.09.25..

#pragma once

// std
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>
// pro
#include <libv/thread/thread_group.hpp>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

class thread_bulk {
	std::function<bool()> work;

	bool terminated = false;
	std::atomic_bool finishing = false;
	std::atomic_size_t working{0};

	std::mutex mutex;
	std::condition_variable work_cv;
	std::condition_variable done_cv; // C++20 done_cv can be folded under done atomic

	thread_group group;

private:
	void loop() {
		std::unique_lock lock(mutex);
		while (true) {
			{
				while (!terminated && (!work || finishing))
					work_cv.wait(lock);

				if (terminated)
					return;

				++working;
				lock.unlock();
			}

			while (!finishing && work());

			{
				lock.lock();
				finishing = true;

				if (--working == 0) { // Last thread is dedicated to clean up
					work = nullptr;
					finishing = false;
					done_cv.notify_all();
	//				done.notify_all();
				}
			}
		}
	}

public:
	explicit thread_bulk(size_t n) :
		group(n, [this]{ loop(); }) {
	}

	~thread_bulk() {
		std::unique_lock lock(mutex);
		terminated = true;
		work_cv.notify_all();
	}

public:
	/// Executes func from multiple thread. Stops when false was returned.
	/// @note There may be multiple calls even after the first false was returned.
	template <typename F>
	void execute_and_wait(F&& func) {
		std::unique_lock lock(mutex);

		while (work) // Wait to finish previous work from a concurrent call
			done_cv.wait(lock);
//			done.wait(lock);

		work = std::forward<F>(func);
		work_cv.notify_all();

		while (work)
			done_cv.wait(lock);
//			done.wait(lock);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
