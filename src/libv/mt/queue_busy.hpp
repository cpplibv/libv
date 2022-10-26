// Project: libv.mt, File: src/libv/mt/queue_busy.hpp

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

template <typename T>
class queue_busy {
	mutable libv::mutex_spinlock mutex;
	mutable std::condition_variable_any cv;
	std::queue<T> queue;

public:
	template <typename... Args>
	inline void emplace(Args&&... args) {
		std::unique_lock lock(mutex);
		queue.emplace(std::forward<Args>(args)...);
		lock.unlock();
		cv.notify_one();
	}

	[[nodiscard]] inline std::optional<T> try_wait_pop(std::stop_token stop_token) {
		std::unique_lock lock(mutex);
		const auto has_value = cv.wait(lock, stop_token, [&]{ return !queue.empty(); });

		if (!has_value)
			return std::nullopt;

		std::optional<T> result{std::move(queue.front())};
		queue.pop();

		return result;
	}

	[[nodiscard]] inline size_t size() const noexcept {
		std::unique_lock lock(mutex);
		return queue.size();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
