// File: Context.hpp, Created on 2015. január 6. 20:08, Author: Vader

#pragma once

// std
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
// pro
#include <vl/log.hpp>

#ifndef VL_DEFAULT_CONTEXT_TASK_PRIORITY
#    define VL_DEFAULT_CONTEXT_TASK_PRIORITY 300
#endif

namespace vl {

class Context {
	class Task {
		int priority;
		std::condition_variable_any* executed_cv;
		std::function<void() > function;
		std::recursive_mutex* mtx;
		std::atomic<bool>* done;
	public:
		Task() { }
		template<typename F, typename = decltype(std::declval<F>()()) >
		Task(F&& func,
				int priority,
				std::condition_variable_any* cv = nullptr,
				std::recursive_mutex* mtx = nullptr,
				std::atomic<bool>* done = nullptr) :
			priority(priority),
			executed_cv(cv),
			function(std::forward<F>(func)),
			mtx(mtx),
			done(done) { }
		void execute() {
			function();
			if (executed_cv) {
				std::lock_guard<std::recursive_mutex> lk(*mtx);
				*done = true;
				executed_cv->notify_all();
			}
		}
		bool operator<(const Task& right) const {
			return priority < right.priority;
		}
	};

	std::condition_variable_any recieved_cv;
	std::recursive_mutex que_m;
	std::priority_queue<Task> que;

	std::atomic<bool> terminateFlag{false};

	std::string name;
	std::thread thread;

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
			std::condition_variable_any executed_cv;
			std::unique_lock<std::recursive_mutex> lk(que_m);
			std::atomic<bool> done{false};

			que.emplace(std::forward<F>(func), priority, &executed_cv, &que_m, &done);
			recieved_cv.notify_all();
			executed_cv.wait(lk, [&done] {
				return done.load();
			});
		} else
			func();
	}
	void terminate() {
		std::lock_guard<std::recursive_mutex> lk(que_m);
		terminateFlag = true;
		recieved_cv.notify_all();
	}
	void join() {
		static std::mutex join_m;
		std::lock_guard<std::mutex> lk(join_m);
		if (thread.joinable())
			thread.join();
	}
	inline auto get_id() {
		return thread.get_id();
	}

private:
	void run() {
		while (!terminateFlag) {
			{
				std::unique_lock<std::recursive_mutex> lk(que_m);
				recieved_cv.wait(lk, [this] {
					return terminateFlag.load() || !que.empty();
				});
			}
			try {
				processImpl();
			} catch (const std::exception& ex) {
				VLOG_ERROR(vl::log(), "Exception occurred in [%s] context: %s", name, ex.what());
			}
		}
		try {
			processImpl();
		} catch (const std::exception& ex) {
			VLOG_ERROR(vl::log(), "Exception occurred in [%s] context: %s", name, ex.what());
		}
	}

public:
	Context(const std::string& name) : name(name), thread(&Context::run, this) { }
	Context() : Context("Unnamed") { }
	virtual ~Context() {
		terminate();
		join();
	}
};

} //namespace vl