// Project: libv.ctrl, File: test/libv/ctrl/mock_feature.hpp

// std
#include <vector>
#include <optional>


// -------------------------------------------------------------------------------------------------

struct MockContext {
	size_t access_count = 0;
};

template <typename T>
class MockFeature {
	std::vector<T> queue;
	size_t head = 0;

public:
	auto callback() {
		return [this](auto&& arg, auto& ctx) {
			ctx.access_count++;
			queue.emplace_back(std::forward<decltype(arg)>(arg));
		};
	}

	size_t queue_size() {
		return queue.size();
	}

	size_t consume_queue() {
		const auto n = queue.size();
		queue.clear();
		return n;
	}

	std::optional<T> consume_first() {
		std::optional<T> result;

		if (!queue.empty()) {
			result = queue.front();
			queue.erase(queue.begin());
		}

		return result;
	}

	bool empty() {
		return queue.empty();
	}

	const T& next() {
		return queue.at(head++);
	}
};
