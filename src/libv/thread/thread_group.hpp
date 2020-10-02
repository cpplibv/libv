// Project: libv.thread, File: src/libv/thread/thread_group.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <thread>
#include <vector>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

class thread_group {
	std::vector<std::jthread> group;

public:
	template <typename F, typename... Args>
	explicit thread_group(size_t n, F&& func, Args&&... args) {
		for (size_t i = 0; i < n; ++i)
			if constexpr (std::is_invocable_v<F, size_t, Args...>)
				group.emplace_back(func, i, args...);
			else
				group.emplace_back(func, args...);
	}

	[[nodiscard]] inline size_t size() const noexcept {
		return group.size();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
