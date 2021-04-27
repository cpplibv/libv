// File: Context.hpp, Created on 2015. január 6. 20:08, Author: Vader

#pragma once

// std
//#include <thread>
#include <vector>
#include <optional>
#include <mutex>
#include <atomic>


namespace libv {

// -------------------------------------------------------------------------------------------------

class work_queue { // NOTE: INCOMPLETE!
	std::atomic<size_t> work = 0;
	std::atomic<bool> done = false; // C++20: use std::stop_token
	thread_group group;

	void loop() {
		while (done)
			work++;
	}

	thread_queue(size_t n) : group(n, [this] { loop(); }) {}
};

// https://www.youtube.com/watch?v=Zcqwb3CWqs4

template <typename T, size_t Limit>
struct concurrent_bounded_queue {
private:
	std::queue<T> items; // TODO P3: Use a ring instead of queue
	std::mutex mutex;
	std::counting_semaphore<Limit> items_produced{0};
	std::counting_semaphore<Limit> remaining_space{Limit};

public:
	constexpr concurrent_bounded_queue() = default;

	template <typename... Args>
	void emplace(Args&&... args);

	template <typename... Args>
	bool try_emplace(Args&&... args);

	T pop();

	std::optional<T> try_pop();
};

//// C++20: atomic aided spin_mutex
//struct spin_mutex {
//private:
//	std::atomic<bool> flag = {false};
//
//public:
//	void lock() {
//		while (flag.exchange(true, std::memory_order::acquire))
//			flag.wait(true, std::memory_order::relaxed);
//	}
//
//	void unlock() {
//		flag.store(false, std::memory_order::release);
//		flag.notify_one();
//	}
//};

// -------------------------------------------------------------------------------------------------

} // namespace libv
