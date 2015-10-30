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
#include <vl/log.hpp>
#include <vl/semaphore.hpp>

#ifndef VL_DEFAULT_WORKERTHREAD_TASK_PRIORITY
#    define VL_DEFAULT_WORKERTHREAD_TASK_PRIORITY 5000
#endif
#ifndef VL_DEFAULT_WORKERTHREAD_NAME
#    define VL_DEFAULT_WORKERTHREAD_NAME "UNNAMED"
#endif

namespace vl {

// -------------------------------------------------------------------------------------------------

class WorkerThread {

	class Task {
		size_t priority;
		std::function<void() > function;
		vl::Semaphore* done;
	public:
		Task() { }
		Task(const Task&) = default;
		Task& operator=(const Task&) = default;
		template<typename F, typename = decltype(std::declval<F>()()) >
		explicit Task(F&& func, size_t priority, vl::Semaphore* done = nullptr) :
			priority(priority),
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
			return priority > rhs.priority;
		}
	};

	std::queue<std::exception_ptr> exceptionHistory;
	std::mutex exceptionHistory_m;
	std::condition_variable_any recieved_cv;
	std::priority_queue<Task, std::vector<Task>,
	std::greater<typename std::vector<Task>::value_type>> que;
	std::recursive_mutex que_m;

	size_t defaultPriority;
	bool terminateFlag = false;

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
				VLOG_DEBUG(vl::log(), "Exception occurred in [%s] WorkerThread. "
						"[%d] unhandled exception in queue. "
						"Last exception: [%s].", name, ex.what(), exceptionHistory.size());
			} catch (...) {
				std::lock_guard<std::mutex> lk(exceptionHistory_m);
				exceptionHistory.emplace(std::current_exception());
				VLOG_DEBUG(vl::log(), "Exception occurred in [%s] WorkerThread. "
						"[%d] unhandled exception in queue. ", name, exceptionHistory.size());
			}
		}
	}

public:
	template<typename F, typename = decltype(std::declval<F>()())>
	void executeAsync(F&& func, size_t priority) {
		std::lock_guard<std::recursive_mutex> lk(que_m);
		que.emplace(std::forward<F>(func), priority);
		recieved_cv.notify_all();
	}
	template<typename F, typename = decltype(std::declval<F>()())>
	inline void executeAsync(F&& func) {
		executeAsync(std::forward<F>(func), defaultPriority);
	}
	template<typename F, typename = decltype(std::declval<F>()())>
	void executeSync(F&& func, size_t priority) {
		if (std::this_thread::get_id() != thread.get_id()) {
			vl::Semaphore done;
			{
				std::lock_guard<std::recursive_mutex> lk(que_m);
				que.emplace(std::forward<F>(func), priority, &done);
				recieved_cv.notify_all();
			}
			done.wait();
		} else
			func();
	}
	template<typename F, typename = decltype(std::declval<F>()())>
	inline void executeSync(F&& func) {
		executeSync(std::forward<F>(func), defaultPriority);
	}
	void terminate() {
		std::lock_guard<std::recursive_mutex> lk(que_m);
		terminateFlag = true;
		recieved_cv.notify_all();
	}
	void join() {
		try {
			if (thread.joinable())
				thread.join();
		} catch (std::system_error& ex) {
			VLOG_DEBUG(vl::log(), "Exception during joining WorkerThread [%s]: %s", name, ex.what());
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
	WorkerThread(const std::string& name, size_t defaultPriority) :
		defaultPriority(defaultPriority),
		thread(&WorkerThread::run, this),
		name(name) { }
	WorkerThread(const std::string& name) : WorkerThread(name, VL_DEFAULT_WORKERTHREAD_TASK_PRIORITY) { }
	WorkerThread(size_t defaultPriority) : WorkerThread(VL_DEFAULT_WORKERTHREAD_NAME, defaultPriority) { }
	WorkerThread() : WorkerThread(VL_DEFAULT_WORKERTHREAD_NAME, VL_DEFAULT_WORKERTHREAD_TASK_PRIORITY) { }
	virtual ~WorkerThread() {
		terminate();
		join();
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace vl