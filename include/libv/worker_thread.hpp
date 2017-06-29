// File: Context.hpp, Created on 2015. január 6. 20:08, Author: Vader

#pragma once

// std
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
// pro
#include <libv/log/log.hpp>
#include <libv/semaphore.hpp>

#ifndef LIBV_DEFAULT_WORKERTHREAD_TASK_PRIORITY
#    define LIBV_DEFAULT_WORKERTHREAD_TASK_PRIORITY 5000
#endif
#ifndef LIBV_DEFAULT_WORKERTHREAD_NAME
#    define LIBV_DEFAULT_WORKERTHREAD_NAME "UNNAMED"
#endif

namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: Documentation
// TODO P5: Handle WorkerThread::counter overflow in Task::operator>

// -------------------------------------------------------------------------------------------------

class WorkerThread {

	class Task {
		uint32_t priority;
		uint32_t number;
		std::function<void() > function;
		libv::Semaphore* done;
	public:
		Task() { }
		Task(const Task&) = default;
		Task& operator=(const Task&) = default;
		template <typename F, typename = decltype(std::declval<F>()()) >
		Task(F&& func, uint32_t priority, uint32_t number, libv::Semaphore* done = nullptr) :
			priority(priority),
			number(number),
			function(func),
			done(done) { }
		void execute() {
			try {
				function();
				if (done)
					done->raise();
			} catch (...) {
				if (done)
					done->raise();
				throw;
			}
		}
		bool operator>(const Task& rhs) const {
			return priority > rhs.priority
					|| (priority == rhs.priority && number > rhs.number);
		}
	};

	std::queue<std::exception_ptr> exceptionHistory;
	std::mutex exceptionHistory_m;
	std::condition_variable_any recieved_cv;
	std::priority_queue<Task, std::vector<Task>,
			std::greater<typename std::vector<Task>::value_type>> que;
	std::recursive_mutex que_m;

	uint32_t counter = 0;
	uint32_t defaultPriority;
	volatile bool terminateFlag = false;

	std::thread thread;
	std::string name;

private:
	inline void process() {
		Task cmd;
		while (true) {
			{
				std::lock_guard<std::recursive_mutex> lk(que_m);
				if (que.empty())
					return;
				cmd = que.top();
				que.pop();
			}
			try {
				cmd.execute();
			} catch (std::exception& ex) {
				std::lock_guard<std::mutex> lk(exceptionHistory_m);
				exceptionHistory.emplace(std::current_exception());
				LIBV_LOG_LIBV_ERROR("Exception occurred in {} WorkerThread. "
						"{} unhandled exception in queue. "
						"Exception message: {}.", name, ex.what(), exceptionHistory.size());
			}
		}
	}

public:
	template <typename F, typename = decltype(std::declval<F>()())>
	void executeAsync(F&& func, uint32_t priority) {
		std::lock_guard<std::recursive_mutex> lk(que_m);
		que.emplace(std::forward<F>(func), counter++, priority);
		recieved_cv.notify_all();
	}
	template <typename F, typename = decltype(std::declval<F>()())>
	inline void executeAsync(F&& func) {
		executeAsync(std::forward<F>(func), defaultPriority);
	}
	template <typename F, typename = decltype(std::declval<F>()())>
	void executeSync(F&& func, uint32_t priority) {
		if (std::this_thread::get_id() != thread.get_id()) {
			libv::Semaphore done;
			{
				std::lock_guard<std::recursive_mutex> lk(que_m);
				que.emplace(std::forward<F>(func), counter++, priority, &done);
				recieved_cv.notify_all();
			}
			done.wait();
		} else
			func();
	}
	template <typename F, typename = decltype(std::declval<F>()())>
	inline void executeSync(F&& func) {
		executeSync(std::forward<F>(func), defaultPriority);
	}
	void stop() {
		std::lock_guard<std::recursive_mutex> lk(que_m);
		terminateFlag = true;
		recieved_cv.notify_all();
	}
	void join() {
		try {
			if (thread.joinable())
				thread.join();
		} catch (std::system_error& ex) {
			LIBV_LOG_LIBV_DEBUG("Exception during joining WorkerThread {}: {}", name, ex.what());
		}
	}
	inline auto getID() {
		return thread.get_id();
	}
	std::exception_ptr exception() {
		std::lock_guard<std::mutex> lk(exceptionHistory_m);
		if (exceptionHistory.empty())
			return nullptr;

		auto result = exceptionHistory.front();
		exceptionHistory.pop();
		return result;
	}
private:
	void run() {
		while (!terminateFlag) {
			{
				std::unique_lock<std::recursive_mutex> lk(que_m);
				while (!terminateFlag && que.empty())
					recieved_cv.wait(lk);
			}
			process();
		}
		process();
	}

public:
	WorkerThread(const std::string& name, uint32_t defaultPriority) :
		defaultPriority(defaultPriority),
		thread(&WorkerThread::run, this),
		name(name) { }
	WorkerThread(const std::string& name) : WorkerThread(name, LIBV_DEFAULT_WORKERTHREAD_TASK_PRIORITY) { }
	WorkerThread(uint32_t defaultPriority) : WorkerThread(LIBV_DEFAULT_WORKERTHREAD_NAME, defaultPriority) { }
	WorkerThread() : WorkerThread(LIBV_DEFAULT_WORKERTHREAD_NAME, LIBV_DEFAULT_WORKERTHREAD_TASK_PRIORITY) { }
	virtual ~WorkerThread() {
		stop();
		join();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv