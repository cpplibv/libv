// Project: libv.mt, File: src/libv/mt/concurrent_queue.hpp

#pragma once

// std
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <stop_token>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

template <typename T>
class concurrent_queue {
	mutable std::mutex queue_m;
	std::queue<T> queue;
	mutable std::condition_variable_any received_cv;
	mutable std::condition_variable_any consumed_cv;

	const std::size_t limit = std::numeric_limits<std::size_t>::max();

public:
	inline explicit concurrent_queue() noexcept = default;
	inline explicit concurrent_queue(std::size_t limit) noexcept : limit(limit) { }

public:
	template <typename... Args>
	void emplace(Args&&... args) {
		std::unique_lock lock(queue_m);

		while (queue.size() >= limit)
			consumed_cv.wait(lock);

		queue.emplace(std::forward<Args>(args)...);

		lock.unlock();
		received_cv.notify_one();
	}

	T pop() {
		T result; // Could be temporary std::optional if default constructability is impossible

		std::unique_lock lock(queue_m);
		while (queue.empty())
			received_cv.wait(lock);

		result = std::move(queue.front());
		queue.pop();

		lock.unlock();
		consumed_cv.notify_all(); // notify_all to ensure that wait_empty would work

		return result;
	}

	std::optional<T> pop(const std::stop_token& stop_token) {
		std::optional<T> result;

		std::unique_lock lock(queue_m);

		received_cv.wait(lock, stop_token, [&]{ return !queue.empty(); });
		if (stop_token.stop_requested())
			return result;

		result = std::move(queue.front());
		queue.pop();

		lock.unlock();
		consumed_cv.notify_all(); // notify_all to ensure that wait_empty would work

		return result;
	}

	std::optional<T> try_pop() {
		std::optional<T> result;

		std::unique_lock lock(queue_m);
		if (queue.empty())
			return result;

		result = std::move(queue.front());
		queue.pop();

		lock.unlock();
		consumed_cv.notify_all(); // notify_all to ensure that wait_empty would work

		return result;
	}

	void wait_empty() const {
		std::unique_lock lock(queue_m);
		while (!queue.empty())
			consumed_cv.wait(lock);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
