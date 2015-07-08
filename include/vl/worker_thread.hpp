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

#ifndef VL_DEFAULT_CONTEXT_TASK_PRIORITY
#    define VL_DEFAULT_CONTEXT_TASK_PRIORITY 500
#endif
#ifndef VL_DEFAULT_CONTEXT_NAME
#    define VL_DEFAULT_CONTEXT_NAME "UNNAMED"
#endif

namespace vl {

class WorkerThread {

	class Task {
		size_t priority;
		std::function<void()> function;
		vl::Semaphore* done;
	public:
		Task() { }
		Task(Task&&) = default;
		Task& operator=(Task&&) = default;
		template<typename F, typename = decltype(std::declval<F>()()) >
		Task(F&& func, size_t priority, vl::Semaphore* done = nullptr) :
			priority(priority),
			done(done) { }
		void execute() {
			function();
			if (done)
				done->raise();
		}
		bool operator<(const Task& rhs) const {
			return priority < rhs.priority;
		}
	};

	std::condition_variable_any recieved_cv;
	std::recursive_mutex que_m;
	std::priority_queue<Task> que;

	size_t defaultPriority;
	bool terminateFlag = false;

	std::thread thread;
	std::string name;

private:
	inline void process() {
		try {
			Task cmd;
			while (true) {
				{
					std::lock_guard<std::recursive_mutex> lk(que_m);
					if (que.empty())
						return;
					cmd = std::move(const_cast<Task&>(que.top()));
					que.pop();
				}
				cmd.execute();
			}
		} catch (const std::exception& ex) {
			VLOG_ERROR(vl::log(),
					"Unhandled exception occurred in WorkerThread [%s]: %s", name, ex.what());
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
			thread.join();
		} catch (std::system_error&) {
		}
	}
	inline auto get_id() {
		return thread.get_id();
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
	WorkerThread(const std::string& name) : WorkerThread(name, VL_DEFAULT_CONTEXT_TASK_PRIORITY) { }
	WorkerThread(size_t defaultPriority) : WorkerThread(VL_DEFAULT_CONTEXT_NAME, defaultPriority) { }
	WorkerThread() : WorkerThread(VL_DEFAULT_CONTEXT_NAME, VL_DEFAULT_CONTEXT_TASK_PRIORITY) { }
	virtual ~WorkerThread() {
		terminate();
		join();
	}
};

} //namespace vl