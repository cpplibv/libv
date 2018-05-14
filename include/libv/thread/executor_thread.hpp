// File: Context.hpp, Created on 2015. január 6. 20:08, Author: Vader

#pragma once

// std
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
// pro
#include <libv/log/log.hpp>
#include <libv/thread/binary_latch.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

class ExecutorThread {
	std::queue<std::function<void()>> queue;
	std::mutex queue_m;
	std::condition_variable recieved_cv;

	std::atomic_bool terminate = false;
	std::thread thread;
	std::string name;

public:
	template <typename F, typename = decltype(std::declval<F>()())>
	void executeAsync(F&& func) {
		std::unique_lock lock(queue_m);
		queue.emplace(std::forward<F>(func));
		recieved_cv.notify_all();
	}

	template <typename F, typename = decltype(std::declval<F>()())>
	void executeSync(F&& func) {
		libv::BinaryLatch done;
		executeAsync([&done, f = std::forward<F>(func)]{
			f();
			done.raise();
		});
		done.wait();
	}

	void stop() {
		std::unique_lock lock(queue_m);
		terminate = true;
		recieved_cv.notify_all();
	}

	void join() {
		try {
			if (thread.joinable())
				thread.join();
		} catch (std::system_error& ex) {
			LIBV_LOG_LIBV_ERROR("Exception during joining ExecutorThread {}: {}", name, ex.what());
		}
	}

	inline auto getID() {
		return thread.get_id();
	}

private:
	inline void process() {
		std::function<void()> task;

		while (true) {
			{
				std::unique_lock lock(queue_m);
				if (queue.empty())
					return;
				task = std::move(queue.front());
				queue.pop();
			}

			try {
				task();
			} catch (std::exception& ex) {
				LIBV_LOG_LIBV_ERROR("Exception occurred in {} ExecutorThread: {}", name, ex.what());
			}
		}
	}

	void run() {
		while (!terminate) {
			{
				std::unique_lock lock(queue_m);
				while (queue.empty() && !terminate)
					recieved_cv.wait(lock);
			}
			process();
		}
		process();
	}

public:
	ExecutorThread(const std::string& name = "UNNAMED") :
		thread(&ExecutorThread::run, this),
		name(name) { }
	virtual ~ExecutorThread() {
		stop();
		join();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
