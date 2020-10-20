// Project: libv.mt, File: src/libv/mt/worker_thread.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/mt/worker_thread.hpp>
// pro
#include <libv/log/log.hpp>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

void worker_thread::execute_async(libv::unique_function<void()> func) {
	execute_async(std::move(func), immediate);
}

void worker_thread::execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::duration after) {
	execute_async(std::move(func), std::chrono::steady_clock::now() + after);
}

void worker_thread::execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::time_point at) {
	std::unique_lock lock(queue_m);
	assert(!terminate && "Queueing task after worker thread has been stopped");

	queue.emplace(at, next_index++, std::move(func));
	work_cv.notify_all();
}

void worker_thread::stop() {
	std::unique_lock lock(queue_m);
	terminate = true;
	work_cv.notify_all();
}

// -------------------------------------------------------------------------------------------------

worker_thread::worker_thread(std::string new_name) :
	thread(&worker_thread::loop, this),
	name_(std::move(new_name)) {}

worker_thread::~worker_thread() {
	// TODO P2: What should happen with tasks that are 'canceled', for now we just dont execute them, best would be an argument to the task like netts that tells it, that it is cancelled, with maybe a default way that it would just not get called
	stop();
	// join called in jthread dtor
}

// -------------------------------------------------------------------------------------------------

inline void worker_thread::loop() {
	libv::unique_function<void()> task;

	while (true) {
		{
			std::unique_lock lock(queue_m);

			// Wait for terminate, new task, or until the time of the top task is up
			while (true) {
				if (terminate) {
					break;

				} else if (queue.empty()) {
					work_cv.wait(lock);

				} else if (queue.top().time <= std::chrono::steady_clock::now()) {
					break;

				} else {
					const auto cvs = work_cv.wait_until(lock, queue.top().time);
					if (cvs == std::cv_status::timeout)
						break;
				}
			}

			if (terminate)
				break;

			task = std::move(const_cast<worker_thread_task&>(queue.top()).func); // NOTE: const_cast is safe as the task is popped anyways but ugly
			queue.pop();
		}

		try {
			task();
		} catch (const std::exception& ex) {
			log.error("Exception occurred in {} worker_thread: {}", name_, ex.what());
		}
	}

	// Process tasks that can be executed
	std::unique_lock lock(queue_m);
	while (!queue.empty() && queue.top().time <= std::chrono::steady_clock::now()) {
		task = std::move(const_cast<worker_thread_task&>(queue.top()).func); // NOTE: const_cast is safe but ugly as the task is popped anyways
		queue.pop();

		lock.unlock();

		try {
			task();
		} catch (const std::exception& ex) {
			log.error("Exception occurred in {} worker_thread: {}", name_, ex.what());
		}

		lock.lock();
	}

	// NOTE: Currently the rest of the outlining task are just dropped, but they could be cancelled

	//	// Process tasks that has to be cancelled
	//	while (!queue.empty()) {
	//		task = std::move(const_cast<worker_thread_task&>(queue.top()).func); // NOTE: const_cast is safe but ugly as the task is popped anyways
	//		queue.pop();
	//
	//		lock.unlock();
	//
	//		try {
	//			task(cancelled);
	//		} catch (const std::exception& ex) {
	//			log.error("Exception occurred in {} worker_thread: {}", name_, ex.what());
	//		}
	//
	//		lock.lock();
	//	}
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
