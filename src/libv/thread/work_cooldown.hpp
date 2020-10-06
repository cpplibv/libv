// Project: libv.thread, File: src/libv/thread/work_cooldown.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <atomic>
#include <chrono>
#include <mutex>
// pro
#include <libv/thread/worker_thread.hpp>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

class work_cooldown {
private:
	std::atomic_bool in_flight = false;
	std::mutex cooldown_m;
	std::chrono::steady_clock::time_point cooldown_at = std::chrono::steady_clock::time_point::min();
	std::chrono::steady_clock::duration cooldown{std::chrono::milliseconds{100}};

public:
	explicit inline work_cooldown(std::chrono::steady_clock::duration cooldown) noexcept :
			cooldown(cooldown) {}

public:
	template <typename F>
	void execute_async(F&& func, libv::mt::worker_thread& thread) {
		const auto need_to_queue = !in_flight.exchange(true);

		if (need_to_queue) {
			const auto now = std::chrono::steady_clock::now();
			const auto guard = std::scoped_lock(cooldown_m);

			auto callback = [this, f = std::forward<F>(func)] {
				in_flight = false;
				f();
			};

			if (now >= cooldown_at) {
				// No cooldown
				thread.execute_async(std::move(callback));
				cooldown_at = now + cooldown;
			} else {
				// Has cooldown
				thread.execute_async(std::move(callback), cooldown_at);
				cooldown_at = cooldown_at + cooldown;
			}
		}
	}
};

// -------------------------------------------------------------------------------------------------

class work_heatup_cooldown {
private:
	std::atomic_bool in_flight = false;
	std::mutex cooldown_m;
	std::chrono::steady_clock::time_point cooldown_at = std::chrono::steady_clock::time_point::min();
	std::chrono::steady_clock::duration heatup{std::chrono::milliseconds{100}};
	std::chrono::steady_clock::duration cooldown{std::chrono::milliseconds{100}};

public:
	inline work_heatup_cooldown(std::chrono::steady_clock::duration heatup, std::chrono::steady_clock::duration cooldown) noexcept :
		heatup(heatup),
		cooldown(cooldown) {}

public:
	template <typename F>
	void execute_async(F&& func, libv::mt::worker_thread& thread) {
		const auto need_to_queue = !in_flight.exchange(true);

		if (need_to_queue) {
			const auto now = std::chrono::steady_clock::now();
			const auto guard = std::scoped_lock(cooldown_m);

			auto callback = [this, f = std::forward<F>(func)] {
				in_flight = false;
				f();
			};

			if (now >= cooldown_at) {
				// No cooldown
				thread.execute_async(std::move(callback), now + heatup);
				cooldown_at = now + heatup + cooldown;
			} else {
				// Has cooldown
				const auto execute_at = std::max(cooldown_at, now + heatup);
				thread.execute_async(std::move(callback), execute_at);
				cooldown_at = execute_at + cooldown;
			}
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
