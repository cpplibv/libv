// Project: libv.ctrl, File: test/libv/ctrl/mock_queue.hpp

// std
#include <algorithm>
#include <vector>


// -------------------------------------------------------------------------------------------------

template <typename T>
class MockQueue {
	std::vector<T> queue;

public:
	auto callback() {
		return [this](auto&&... arg) {
			queue.emplace_back(std::forward<decltype(arg)>(arg)...);
		};
	}

	template <typename... Args>
	auto callback_as() {
		return [this](Args&&... arg) {
			queue.emplace_back(std::forward<decltype(arg)>(arg)...);
		};
	}

	const T& front() {
		return queue.front();
	}

	void pop_front() {
		queue.erase(queue.begin());
	}

	std::optional<T> consume_front() {
		std::optional<T> result;

		if (!queue.empty()) {
			result = queue.front();
			queue.erase(queue.begin());
		}

		return result;
	}

	bool queue_empty() {
		return queue.empty();
	}

	size_t queue_size() {
		return queue.size();
	}

	size_t clear() {
		const auto n = queue.size();
		queue.clear();
		return n;
	}

	template <typename F>
	void sort_by(F&& project) {
		std::sort(queue.begin(), queue.end(), [&](const auto& lhs, const auto& rhs) { return project(lhs) < project(rhs); });
	}
};
