// Project: libv.mt, File: src/libv/mt/work_cooldown.hpp

#pragma once

// std
#include <atomic>
#include <chrono>
#include <mutex>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

/// Deduplicates work execution requests after an execution for a cooldown period.
/// - The first request while idle is scheduled immediately, enters cooldown.
/// - The first request during the cooldown is rescheduled after the cooldown ends and extends the cooldown.
/// - Repeated requests during the cooldown are discarded.
/// - Once the cooldown period ends it returns to idle.
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
	template <typename Executor, typename F>
	void execute_async(Executor& executor, F&& func) {
		const auto need_to_discard = in_flight.exchange(true);
		if (need_to_discard)
			return;

		const auto now = std::chrono::steady_clock::now();
		const auto guard = std::scoped_lock(cooldown_m);

		auto callback = [this, f = std::forward<F>(func)]() mutable {
			in_flight = false;
			std::forward<F>(f)();
		};

		if (now >= cooldown_at) {
			// No cooldown
			executor.execute_async(std::move(callback));
			cooldown_at = now + cooldown;
		} else {
			// Has cooldown
			executor.execute_async(std::move(callback), cooldown_at);
			cooldown_at = cooldown_at + cooldown;
		}
	}
};

// -------------------------------------------------------------------------------------------------

/// Deduplicates work execution requests before an execution for a warmup and after it for a cooldown period.
/// - The first request while idle is rescheduled after the warmup ends, enters warmup.
/// - Requests during the warmup are discarded.
/// - Once the warmup period ends it enters cooldown.
/// - The first request during the cooldown is rescheduled after the cooldown ends and extends the cooldown.
/// - Repeated requests during the cooldown are discarded.
/// - Once the cooldown period ends it returns to idle.
class work_warmup_cooldown {
private:
	std::atomic_bool in_flight = false;
	std::mutex cooldown_m;
	std::chrono::steady_clock::time_point cooldown_at = std::chrono::steady_clock::time_point::min();
	std::chrono::steady_clock::duration warmup{std::chrono::milliseconds{100}};
	std::chrono::steady_clock::duration cooldown{std::chrono::milliseconds{100}};

public:
	inline work_warmup_cooldown(std::chrono::steady_clock::duration warmup, std::chrono::steady_clock::duration cooldown) noexcept :
		warmup(warmup),
		cooldown(cooldown) {}

public:
	template <typename Executor, typename F>
	void execute_async(Executor& executor, F&& func) {
		const auto need_to_discard = in_flight.exchange(true);
		if (need_to_discard)
			return;

		const auto now = std::chrono::steady_clock::now();
		const auto guard = std::scoped_lock(cooldown_m);

		auto callback = [this, f = std::forward<F>(func)]() mutable {
			in_flight = false;
			std::forward<F>(f)();
		};

		if (now >= cooldown_at) {
			// No cooldown
			executor.execute_async(std::move(callback), now + warmup);
			cooldown_at = now + warmup + cooldown;
		} else {
			// Has cooldown
			const auto execute_at = std::max(cooldown_at, now + warmup);
			executor.execute_async(std::move(callback), execute_at);
			cooldown_at = execute_at + cooldown;
		}
	}
};

// -------------------------------------------------------------------------------------------------

/// Deduplicates work execution requests before an execution for a warmup period.
/// - The first request while idle is rescheduled after the warmup ends, enters warmup.
/// - Requests during the warmup are discarded.
/// - Once the warmup period ends it returns to idle.
class work_warmup {
private:
	std::atomic_bool in_flight = false;
	std::mutex cooldown_m;
	std::chrono::steady_clock::duration warmup{std::chrono::milliseconds{100}};

public:
	explicit inline work_warmup(std::chrono::steady_clock::duration warmup) noexcept :
		warmup(warmup) {}

public:
	template <typename Executor, typename F>
	void execute_async(Executor& executor, F&& func) {
		const auto need_to_discard = in_flight.exchange(true);
		if (need_to_discard)
			return;

		const auto now = std::chrono::steady_clock::now();
		const auto guard = std::scoped_lock(cooldown_m);

		auto callback = [this, f = std::forward<F>(func)]() mutable {
			in_flight = false;
			std::forward<F>(f)();
		};

		executor.execute_async(std::move(callback), now + warmup);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
