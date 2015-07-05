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
#    define VL_DEFAULT_CONTEXT_TASK_PRIORITY 300
#endif

namespace vl {

class WorkerThread {

	class Task {
		int priority;
		std::function<void() > function;
		vl::Semaphore* done;
	public:
		Task() { }
		template<typename F, typename = decltype(std::declval<F>()()) >
		Task(F&& func, int priority, vl::Semaphore* done = nullptr) :
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

	bool terminateFlag = false;

	std::thread thread;
	std::string name;

private:
	inline void processImpl() {
		Task cmd;
		while (true) {
			{
				std::lock_guard<std::recursive_mutex> lk(que_m);
				if (que.empty())
					return;
				cmd = std::move(que.top());
				que.pop();
			}
			cmd.execute();
		}
	}

public:
	template<typename F, typename = decltype(std::declval<F>()())>
	void executeAsync(F&& func, int priority = VL_DEFAULT_CONTEXT_TASK_PRIORITY) {
		std::lock_guard<std::recursive_mutex> lk(que_m);
		que.emplace(std::forward<F>(func), priority);
		recieved_cv.notify_all();
	}
	template<typename F, typename = decltype(std::declval<F>()())>
	void executeSync(F&& func, int priority = VL_DEFAULT_CONTEXT_TASK_PRIORITY) {
		if (std::this_thread::get_id() != thread.get_id()) {
			std::lock_guard<std::recursive_mutex> lk(que_m);
			vl::Semaphore done;
			que.emplace(std::forward<F>(func), priority, &done);
			recieved_cv.notify_all();
			done.wait();
		} else
			func();
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
			try {
				processImpl();
			} catch (const std::exception& ex) {
				VLOG_ERROR(vl::log(), "Unhandled exception occurred in WorkerThread [%s]: %s", name, ex.what());
			}
		}
		try {
			processImpl();
		} catch (const std::exception& ex) {
			VLOG_ERROR(vl::log(), "Unhandled exception occurred in WorkerThread [%s]: %s", name, ex.what());
		}
	}

public:
	WorkerThread(const std::string& name) :
		thread(&WorkerThread::run, this),
		name(name) { }
	WorkerThread() : WorkerThread("Unnamed") { }
	virtual ~WorkerThread() {
		terminate();
		join();
	}
};

} //namespace vl

//TODO P4: run ből jó lenne kiszedni a redundanciát
//TODO P4: imo processImpl and run can be merged

