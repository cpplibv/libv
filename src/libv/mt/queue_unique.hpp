// Project: libv.mt, File: src/libv/mt/queue_unique.hpp

#pragma once

// std
#include <algorithm>
#include <deque>
#include <mutex>
#include <optional>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

template <typename T>
class queue_unique {

private:
	std::mutex mutex;
	std::deque<T> queue;

public:
	[[nodiscard]] std::optional<T> pop_optional() {
		auto lock = std::unique_lock(mutex);

		std::optional<T> result;

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
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
