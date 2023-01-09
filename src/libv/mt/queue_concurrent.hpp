// Project: libv.mt, File: src/libv/mt/queue_concurrent.hpp

#pragma once

// std
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <stop_token>
// pro
#include <libv/mt/mutex_spinlock.hpp>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

// Improve naming with explicit "wait_" on any possibly blocking operation
// void queue::push(const Element&);
// void queue::push(Element&&);
// queue_op_status queue::wait_push(const Element&);
// queue_op_status queue::wait_push(Element&&);
// queue_op_status queue::try_push(const Element&);
// queue_op_status queue::try_push(Element&&);
// Element queue::value_pop();
// queue_op_status queue::try_pop(Element&);
// void queue::close() noexcept;
// void queue::open();
// bool queue::is_closed() const noexcept;
// bool queue::is_empty() const noexcept;
// bool queue::is_full() const noexcept;

template <typename T>
class queue_concurrent {
	mutable libv::mutex_spinlock queue_m;
	std::queue<T> queue;
	mutable std::condition_variable_any received_cv;
	mutable std::condition_variable_any consumed_cv;

	const std::size_t limit = std::numeric_limits<std::size_t>::max();

public:
	inline explicit queue_concurrent() noexcept = default;
	inline explicit queue_concurrent(std::size_t limit) noexcept : limit(limit) { }

public:
	void push(const T& value) {
		std::unique_lock lock(queue_m);

		while (queue.size() >= limit)
			consumed_cv.wait(lock);

		queue.push(value);

		lock.unlock();
		received_cv.notify_one();
	}

	void push(T&& value) {
		std::unique_lock lock(queue_m);

		while (queue.size() >= limit)
			consumed_cv.wait(lock);

		queue.push(std::move(value));

		lock.unlock();
		received_cv.notify_one();
	}

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
		std::optional<T> result;

		std::unique_lock lock(queue_m);
		while (queue.empty())
			received_cv.wait(lock);

		result.emplace(std::move(queue.front()));
		queue.pop();

		lock.unlock();
		consumed_cv.notify_one();

		return std::move(result.value());
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
		consumed_cv.notify_one();

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
		consumed_cv.notify_one();

		return result;
	}

	/// A not-necessarily-accurate count of the total number of elements
	std::size_t size_approx() const noexcept {
		std::unique_lock lock(queue_m);
		return queue.size();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
