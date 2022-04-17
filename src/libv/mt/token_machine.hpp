// Project: libv.mt, File: src/libv/mt/token_machine.hpp

#pragma once

// libv
#include <libv/utility/guard.hpp>
#include <libv/utility/memory/observer_ptr.hpp>
// std
#include <atomic>
#include <stop_token>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

class token;
class token_machine;

// -------------------------------------------------------------------------------------------------

class token {
private:
	libv::observer_ptr<token_machine> owner_machine = nullptr;

public:
	explicit constexpr inline token(libv::observer_ptr<token_machine> owner_machine) :
		owner_machine(owner_machine) {
	}
	constexpr inline token(const token&) noexcept = delete;
	constexpr inline token& operator=(const token&) & noexcept = delete;
	constexpr inline token(token&& other) noexcept :
		owner_machine(other.owner_machine) {
		other.owner_machine = nullptr;
	}
	constexpr inline token& operator=(token&& other) & noexcept {
		std::swap(owner_machine, other.owner_machine);
		return *this;
	}
	constexpr inline ~token();
};

// -------------------------------------------------------------------------------------------------

class token_machine {
	friend token;

private:
	std::atomic_int32_t active_token_count{0};
	std::atomic<std::stop_source*> zero_stop_source = nullptr;

public:
	[[nodiscard]] inline token create_token() {
		++active_token_count;
		return token{this};
	}

	inline void start_async_wait(std::stop_source& ss) {
		zero_stop_source.store(&ss);
		if (active_token_count.load() == 0)
			ss.request_stop();
	}

	inline void stop_async_wait() {
		zero_stop_source.store(nullptr);
	}

	[[nodiscard]] inline auto async_wait_guard(std::stop_source& ss) {
		start_async_wait(ss);
		return libv::guard([this] {
			stop_async_wait();
		});
	}

private:
	inline void destroy_token() {
		if (--active_token_count == 0)
			if (auto ptr = zero_stop_source.load())
				ptr->request_stop();
	}

public:
	[[nodiscard]] inline int32_t token_count() const {
		return active_token_count.load();
	}
};

//

constexpr inline token::~token() {
	if (owner_machine != nullptr)
		owner_machine->destroy_token();
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
