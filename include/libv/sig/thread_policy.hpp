// File: thread_policy.hpp Author: Vader Created on March 13, 2016, 2:55 PM

#pragma once

// std
#include <mutex>


namespace libv {
namespace sig {

// -------------------------------------------------------------------------------------------------

struct SingleThread {
public:
	static constexpr bool ALLOW_DISCONNECT_ON_DTOR = true;

public:
	inline void lock() { }
	inline void unlock() { }
	inline void lock_shared() { }
	inline void unlock_shared() { }
};

struct MultiThread {
public:
	static constexpr bool ALLOW_DISCONNECT_ON_DTOR = false;

private:
	std::shared_mutex mutex;

public:
	inline void lock() {
		mutex.lock();
	}
	inline void unlock() {
		mutex.unlock();
	}
	inline void lock_shared() {
		mutex.lock_shared();
	}
	inline void unlock_shared() {
		mutex.unlock_shared();
	}
};

// -------------------------------------------------------------------------------------------------

template <typename Mutex>
using read_lock_guard = std::shared_lock<Mutex>;

template <typename Mutex>
using write_lock_guard = std::unique_lock<Mutex>;

// -------------------------------------------------------------------------------------------------

} // namespace sig
} //namespace libv
