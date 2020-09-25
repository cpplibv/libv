// Created by Vader on 2020.09.25..

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
	template <typename F>
	explicit thread_group(size_t n, F&& func) {
		for (size_t i = 0; i < n; ++i)
			if constexpr (std::is_invocable_v<F, size_t>)
				group.emplace_back(func, i);
			else
				group.emplace_back(func);
	}

	[[nodiscard]] inline size_t size() const noexcept {
		return group.size();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
