// File:   noop_mutex.hpp Author: Vader Created on 07 September 2019, 17:19

#pragma once

// libv
#include <libv/log/log.hpp>
// std
#include <mutex>
#include <shared_mutex>
#include <string>
// pro
#include <libv/thread/number.hpp>


namespace libv {
namespace thread {

// -------------------------------------------------------------------------------------------------

struct mutex_debug {
private:
	std::shared_mutex mutex;

public:
	std::string name = "";

	mutex_debug() {}
	mutex_debug(std::string name) : name(name) {}

public:
	inline void lock() {
		log.debug("{} lock from {}", name, libv::thread_number());
		mutex.lock();
	}
	inline bool try_lock() {
		log.debug("{} try_lock from {}", name, libv::thread_number());
		return mutex.try_lock();
	}
	inline void unlock() {
		log.debug("{} unlock from {}", name, libv::thread_number());
		mutex.unlock();
	}

	inline void lock_shared() {
		log.debug("{} lock_shared from {}", name, libv::thread_number());
		mutex.lock_shared();
	}
	inline bool try_lock_shared() {
		log.debug("{} try_lock_shared from {}", name, libv::thread_number());
		return mutex.try_lock_shared();
	}
	inline void unlock_shared() {
		log.debug("{} unlock_shared from {}", name, libv::thread_number());
		mutex.unlock_shared();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace thread
} // namespace libv
