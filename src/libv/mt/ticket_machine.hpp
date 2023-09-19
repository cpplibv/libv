// Project: libv.mt, File: src/libv/mt/ticket_machine.hpp

#pragma once

// libv
#include <libv/mt/fwd.hpp>
#include <libv/utility/guard.hpp>
#include <libv/utility/memory/observer_ptr.hpp>
// std
#include <atomic>
#include <stop_token>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

class ticket;
class ticket_machine;

class ticket {
private:
	libv::observer_ptr<ticket_machine> owner_machine = nullptr;

public:
	explicit constexpr inline ticket(libv::observer_ptr<ticket_machine> owner_machine) :
		owner_machine(owner_machine) {
	}
	constexpr inline ticket(const ticket&) noexcept = delete;
	constexpr inline ticket& operator=(const ticket&) & noexcept = delete;
	constexpr inline ticket(ticket&& other) noexcept :
		owner_machine(other.owner_machine) {
		other.owner_machine = nullptr;
	}
	constexpr inline ticket& operator=(ticket&& other) & noexcept {
		std::swap(owner_machine, other.owner_machine);
		return *this;
	}
	constexpr inline ~ticket();
};

// -------------------------------------------------------------------------------------------------

class ticket_machine {
	friend ticket;

private:
	std::atomic_int32_t active_ticket_count{0};
	std::atomic<std::stop_source*> zero_stop_source = nullptr;

public:
	[[nodiscard]] inline ticket create_ticket() {
		++active_ticket_count;
		return ticket{this};
	}

	inline void start_async_wait(std::stop_source& ss) {
		zero_stop_source.store(&ss);
		if (active_ticket_count.load() == 0)
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
	inline void destroy_ticket() {
		if (--active_ticket_count == 0)
			if (auto ptr = zero_stop_source.load())
				ptr->request_stop();
	}

public:
	[[nodiscard]] inline int32_t ticket_count() const {
		return active_ticket_count.load();
	}
};

//

constexpr inline ticket::~ticket() {
	if (owner_machine != nullptr)
		owner_machine->destroy_ticket();
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
