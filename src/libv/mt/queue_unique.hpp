// Project: libv.mt, File: src/libv/mt/queue_unique.hpp

#pragma once

// libv
#include <libv/mt/mutex_noop.hpp>
// std
#include <algorithm>
#include <deque>
#include <mutex>
#include <optional>
#include <vector>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

template <typename T, typename Mutex>
class basic_queue_unique {
private:
	[[no_unique_address]] mutable Mutex mutex;
	std::deque<T> queue;

public:
	[[nodiscard]] std::optional<T> pop_front_optional() {
		std::optional<T> result;

		auto lock = std::unique_lock(mutex);

		if (!queue.empty()) {
			result.emplace(std::move(queue.front()));
			queue.pop_front();
		}

		return result;
	}

	void push_back(T&& var) {
		auto lock = std::unique_lock(mutex);
		if (std::ranges::find(queue, var) != queue.end())
			return;
		queue.push_back(std::move(var));
	}

	void push_back(const T& var) {
		auto lock = std::unique_lock(mutex);
		if (std::ranges::find(queue, var) != queue.end())
			return;
		queue.push_back(var);
	}

	[[nodiscard]] inline bool empty() const noexcept {
		auto lock = std::unique_lock(mutex);
		return queue.empty();
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename Mutex>
class basic_queue_unique_batch {
private:
	[[no_unique_address]] mutable Mutex mutex;
	std::vector<T> queue;

public:
	void push_back(T&& var) {
		auto lock = std::unique_lock(mutex);
		if (std::ranges::find(queue, var) != queue.end())
			return;
		queue.push_back(std::move(var));
	}

	void push_back(const T& var) {
		auto lock = std::unique_lock(mutex);
		if (std::ranges::find(queue, var) != queue.end())
			return;
		queue.push_back(var);
	}

	[[nodiscard]] inline bool empty() const noexcept {
		auto lock = std::unique_lock(mutex);
		return queue.empty();
	}

	/// @threadsafe Only a single thread can call drain_and_clear at the same time (but push_back can be called concurrently)
	template <typename F>
	void drain_and_clear(F&& func) {
		auto lock = std::unique_lock(mutex);

		for (std::size_t i = 0; i < queue.size(); ++i) {
			T var = std::move(queue[i]);
			lock.unlock();
			func(std::move(var));
			lock.lock();
		}

		queue.clear();
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
using queue_unique = basic_queue_unique<T, std::mutex>;

template <typename T>
using queue_unique_st = basic_queue_unique<T, libv::mt::mutex_noop>;

template <typename T>
using queue_unique_batch = basic_queue_unique_batch<T, std::mutex>;

template <typename T>
using queue_unique_batch_st = basic_queue_unique_batch<T, libv::mt::mutex_noop>;

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
