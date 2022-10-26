// Project: libv.utility, File: src/libv/utility/polling_cooldown.hpp

#pragma once

// libv
//#include <libv/mt/mutex_noop.hpp>
// std
#include <chrono>

namespace libv {

// -------------------------------------------------------------------------------------------------

//template <typename Mutex = libv::mt::mutex_noop>
class polling_warmup_cooldown {
private:
	enum class State {
		idle,
		warmup,
		cooldown,
	};

private:
//	[[no_unique_address]] Mutex mutex;
	State state = State::idle;
	std::chrono::steady_clock::time_point action_at;

public:
	std::chrono::steady_clock::duration warmup{std::chrono::seconds{5}};
	std::chrono::steady_clock::duration cooldown{std::chrono::seconds{30}};

public:
	inline polling_warmup_cooldown(std::chrono::steady_clock::duration warmup, std::chrono::steady_clock::duration cooldown) :
			warmup(warmup),
			cooldown(cooldown) {}

	[[nodiscard]] bool fire_and_test() {
		const auto now = std::chrono::steady_clock::now();

		switch (state) {
		case State::idle:
			state = State::warmup;
			action_at = now + warmup;
			return false;

		case State::warmup:
			if (now > action_at) {
				state = State::cooldown;
				action_at = now + cooldown;
				return true;
			} else {
				return false;
			}

		case State::cooldown:
			if (now > action_at) {
				state = State::warmup;
				action_at = now + warmup;
				return false;
			} else {
				state = State::warmup;
				action_at = std::max(now + warmup, action_at);
				return false;
			}
		}

		return false; // Unreachable
	}

//	void fire();

	[[nodiscard]] bool test() {
		const auto now = std::chrono::steady_clock::now();

		switch (state) {
		case State::idle:
			return false;

		case State::warmup:
			if (now > action_at) {
				state = State::cooldown;
				action_at = now + cooldown;
				return true;
			} else {
				return false;
			}

		case State::cooldown:
			if (now > action_at)
				state = State::idle;

			return false;
		}

		return false; // Unreachable
	}

	void force_ready() {
		state = State::warmup;
		action_at = std::chrono::steady_clock::time_point{};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
