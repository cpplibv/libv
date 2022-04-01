// Project: libv.mt, File: src/libv/mt/basic_worker_thread.lpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/unique_function.hpp>
#include <libv/log/log.hpp>
// std
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

template <typename Threads>
class basic_worker_thread {
	static constexpr std::chrono::steady_clock::time_point immediate = std::chrono::steady_clock::time_point::min();

	struct queued_task {
		std::chrono::steady_clock::time_point time;
		uint64_t index;
		libv::unique_function<void()> func;
	};

	struct queue_comp {
		[[nodiscard]] constexpr inline bool operator()(const queued_task& lhs, const queued_task& rhs) const noexcept {
			if (lhs.time == rhs.time)
				return lhs.index > rhs.index;
			return lhs.time > rhs.time;
		}
	};

private:
	std::priority_queue<queued_task, std::vector<queued_task>, queue_comp> queue;
	mutable std::mutex queue_m;
	mutable std::condition_variable work_cv;

	bool terminate = false;
	Threads context_;
	const std::string name_;

	uint64_t next_index = 0;

public:
	template <typename... Args>
	inline explicit basic_worker_thread(std::string name, Args&&... args);
	inline ~basic_worker_thread();

public:
	inline void execute_async(libv::unique_function<void()> func);
	inline void execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::duration after);
	inline void execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::time_point at);
	inline void stop();
	inline void join();
	[[nodiscard]] inline bool wait_for_empty() const;

public:
	[[nodiscard]] inline const std::string& name() const noexcept {
		return name_;
	}
	[[nodiscard]] inline std::size_t task_queue_size() const noexcept {
		std::unique_lock lock(queue_m);
		return queue.size();
	}
	[[nodiscard]] inline const Threads& context() const noexcept {
		return context_;
	}

public:
	void run();
};

// -------------------------------------------------------------------------------------------------

template <typename Threads>
template <typename... Args>
inline basic_worker_thread<Threads>::basic_worker_thread(std::string new_name, Args&&... args) :
	context_(std::forward<Args>(args)..., &basic_worker_thread::run, this),
	name_(std::move(new_name)) {}

template <typename Threads>
inline basic_worker_thread<Threads>::~basic_worker_thread() {
	// TODO P2: What should happen with tasks that are 'canceled', for now we just dont execute them, best would be an argument to the task like netts that tells it, that it is cancelled, with maybe a default way that it would just not get called
	stop();
}

// -------------------------------------------------------------------------------------------------

template <typename Threads>
inline void basic_worker_thread<Threads>::execute_async(libv::unique_function<void()> func) {
	execute_async(std::move(func), immediate);
}

template <typename Threads>
inline void basic_worker_thread<Threads>::execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::duration after) {
	execute_async(std::move(func), std::chrono::steady_clock::now() + after);
}

template <typename Threads>
inline void basic_worker_thread<Threads>::execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::time_point at) {
	std::unique_lock lock(queue_m);
	assert(!terminate && "Queueing task after worker thread has been stopped");

	queue.emplace(at, next_index++, std::move(func));
	work_cv.notify_one();
}

template <typename Threads>
inline void basic_worker_thread<Threads>::stop() {
	std::unique_lock lock(queue_m);
	terminate = true;
	work_cv.notify_all();
}

template <typename Threads>
inline void basic_worker_thread<Threads>::join() {
	context_.join();
}

template <typename Threads>
inline bool basic_worker_thread<Threads>::wait_for_empty() const {
	std::unique_lock lock(queue_m);

	bool had_job = false;
	while (true) {
		if (queue.empty())
			return had_job;

		if (queue.top().time > std::chrono::steady_clock::now())
			return had_job;

		had_job = true;
		work_cv.wait(lock, [this] {
			return queue.empty() || queue.top().time > std::chrono::steady_clock::now();
		});
	}
}

// -------------------------------------------------------------------------------------------------

template <typename Threads>
inline void basic_worker_thread<Threads>::run() {
	libv::unique_function<void()> task;
	std::unique_lock lock(queue_m);

	while (true) {
		// Wait for terminate, new task, or until the time of the top task is up
		while (true) {
			if (terminate) {
				break;

			} else if (queue.empty()) {
				work_cv.wait(lock);

			} else if (queue.top().time <= std::chrono::steady_clock::now()) {
				break;

			} else {
				// NOTE: This code will woke up every thread for the next task
				//      could be improved, but it would be extra complexity, good enough for now
				const auto cvs = work_cv.wait_until(lock, queue.top().time);
				if (cvs == std::cv_status::timeout) {
					if (queue.empty() || queue.top().time > std::chrono::steady_clock::now())
						// Someone else already took care of the job, go back to sleep
						continue;
					break;
				}
			}
		}

		if (terminate)
			break;

		task = std::move(const_cast<queued_task&>(queue.top()).func); // NOTE: const_cast is safe as the task is popped anyways
		queue.pop();

		lock.unlock();

		try {
			task();
		} catch (const std::exception& ex) {
			log.error("Exception occurred in {} basic_worker_thread: {}", name_, ex.what());
		}

		lock.lock();
	}

	// Process tasks that can be executed
	while (!queue.empty() && queue.top().time <= std::chrono::steady_clock::now()) {
		task = std::move(const_cast<queued_task&>(queue.top()).func); // NOTE: const_cast is safe but ugly as the task is popped anyways
		queue.pop();

		lock.unlock();

		try {
			task();
		} catch (const std::exception& ex) {
			log.error("Exception occurred in {} basic_worker_thread: {}", name_, ex.what());
		}

		lock.lock();
	}

	// NOTE: Currently the rest of the outlining task are just dropped, but they could be cancelled

	//	// Process tasks that has to be cancelled
	//	while (!queue.empty()) {
	//		task = std::move(const_cast<queued_task&>(queue.top()).func); // NOTE: const_cast is safe but ugly as the task is popped anyways
	//		queue.pop();
	//
	//		lock.unlock();
	//
	//		try {
	//			task(cancelled);
	//		} catch (const std::exception& ex) {
	//			log.error("Exception occurred in {} basic_worker_thread: {}", name_, ex.what());
	//		}
	//
	//		lock.lock();
	//	}
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
