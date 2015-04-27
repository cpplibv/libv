// File: ThreadPolicy.hpp, Created on 2014. augusztus 12. 23:11, Author: Vader

#pragma once

#include <mutex>

namespace vl {

	struct SingleThread {
		inline void lock() { }
		inline void unlock() { }
	};

	class MultiThreadGlobal {
		static inline std::recursive_mutex& getLock() {
			static std::recursive_mutex lock_m;
			return lock_m;
		}
	public:
		inline void lock() {
			getLock().lock();
		}
		inline void unlock() {
			getLock().unlock();
		}
	};

	class MultiThreadLocal {
		std::recursive_mutex lock_m;
	public:
		inline void lock() {
			lock_m.lock();
		}
		inline void unlock() {
			lock_m.unlock();
		}
	};

} //namespace vl
