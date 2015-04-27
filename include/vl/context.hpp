// File: Context.hpp, Created on 2015. január 6. 20:08, Author: Vader

#pragma once

// std
#include <atomic>
#include <cassert>
#include <functional>
#include <string>
#include <thread>
// pro
#include <vl/invoke_que.hpp>
#include <vl/semaphore.hpp>
#include <vl/log.hpp>

namespace vl {

class Context {
	std::atomic<bool> terminateFlag{false};
	vl::InvokeQueue taskQueue;
	vl::Semaphore run_s; //TODO P5: Remove / debug why i need this... (?)
	vl::Semaphore term_s; //TODO P5: Remove?

	std::string contextName;
	std::thread contextThread;

public:
	/**
	 * Execute a function by the context's thread asynchronously.
	 * @param func The function
	 */
	template<typename F, typename = decltype(std::declval<F>()())>
	inline void executeAsync(F&& func) {
		assert(!terminateFlag);
		taskQueue.invokeAsync(std::forward<F>(func));
	}
	/**
	 * Execute a function by the context's thread synchronously.
	 * @param func The function
	 */
	template<typename F, typename = decltype(std::declval<F>()())>
	inline void executeSync(F&& func) {
		assert(!terminateFlag);
		if (std::this_thread::get_id() != contextThread.get_id())
			taskQueue.invokeSync(std::forward<F>(func));
		else
			func();
	}

	void terminate() {
		terminateFlag = true;
		taskQueue.interrupt();
	}

	void join() {
		term_s.wait();
	}

	auto get_id(){
		return contextThread.get_id();
	}
	
private:
	void run() {
		run_s.raise();
		while (!terminateFlag) {
			try {
				taskQueue.waitAndProcess();
			} catch (const std::exception& ex) {
				VLOG_ERROR(vl::log(), "Exception occurred in [%s] context: %s", contextName, ex.what());
			}
		}
		try {
			taskQueue.ignoreAndProcess();
		} catch (const std::exception& ex) {
			VLOG_ERROR(vl::log(), "Exception occurred in [%s] context: %s", contextName, ex.what());
		}
		term_s.raise();
	}

public:
	Context() :
		contextName("Unnamed"),
		contextThread(&Context::run, this) {
		run_s.wait();
	}
	Context(const std::string& contextName) :
		contextName(contextName),
		contextThread(&Context::run, this) {
		run_s.wait();
	}
	virtual ~Context() {
		terminate();
		term_s.wait();
		contextThread.join();
	}
};

} //namespace vl
