// Project: libv.mt, File: src/libv/mt/thread_group.hpp

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
	explicit thread_group(std::size_t n, F&& func, Args&&... args) {
		for (std::size_t i = 0; i < n; ++i)
			if constexpr (std::is_invocable_v<F, std::size_t, Args...>)
				group.emplace_back(func, i, args...);
			else
				group.emplace_back(func, args...);
	}

	[[nodiscard]] inline std::size_t size() const noexcept {
		return group.size();
	}

	[[nodiscard]] inline bool joinable() const noexcept {
		// Fake thread_group as always joinable, join will take care of the checks
		return true;
	}

	inline void join() noexcept {
		for (auto& thread : group) {
			if (thread.joinable())
				try {
					thread.join();
				} catch (const std::system_error&) {
					// It's not an error, ignore it
				}
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
