// Project: libv.thread, File: src/libv/thread/worker_thread.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/unique_function.hpp>
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

class worker_thread {
	static constexpr std::chrono::steady_clock::time_point immediate = std::chrono::steady_clock::time_point::min();

	struct worker_thread_task {
		std::chrono::steady_clock::time_point time;
		size_t index;
		libv::unique_function<void()> func;

		[[nodiscard]] friend constexpr inline bool operator<(const worker_thread_task& lhs, const worker_thread_task& rhs) noexcept {
			if (lhs.time == rhs.time)
				return lhs.index > rhs.index;
			return lhs.time > rhs.time;
		}
	};

private:
	std::priority_queue<worker_thread_task> queue;
	mutable std::mutex queue_m;
	std::condition_variable work_cv;

	bool terminate = false;
	std::jthread thread;
	const std::string name_;

	size_t next_index = 0;

public:
	void execute_async(libv::unique_function<void()> func);
	void execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::duration after);
	void execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::time_point at);
	void stop();

public:
	[[nodiscard]] inline const std::string& name() const noexcept {
		return name_;
	}
	[[nodiscard]] inline size_t size() const noexcept {
		std::unique_lock lock(queue_m);
		return queue.size();
	}

public:
	explicit worker_thread(std::string name = "unnamed");
	~worker_thread();

private:
	inline void loop();
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
