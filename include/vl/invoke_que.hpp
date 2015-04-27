// File: InvokeQue.hpp, Created on 2014. november 22. 0:22, Author: Vader

#pragma once

// std
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

//TODO P5: Switching from functional to union magic to eliminate dynamic memory allocation

namespace vl {

class InvokeQueue {

	class Command {
		std::condition_variable_any* executed_cv;
		std::function<void()> function;
	public:
		Command() { }
		template<typename F, typename = decltype(std::declval<F>()()) >
		Command(F&& func, std::condition_variable_any* cv = nullptr) :
			executed_cv(cv), function(std::forward<F>(func)) { }
		inline void execute() {
			function();
			if (executed_cv)
				executed_cv->notify_all();
		}
	};
private:
	bool ignored = false;
	std::condition_variable_any recieved_cv;
	std::recursive_mutex data_m;
	std::queue<Command> data;
private:
	inline void processImpl() {
		Command cmd; //TODO P2: remove need for copy ctor on std::function?
		while (true) {
			{
				std::unique_lock<std::recursive_mutex> data_lk(data_m);
				if (data.empty()) {
					return;
				}
				cmd = data.front();
				data.pop();
			}
			cmd.execute();
		}
	}
public:
	void ignore() {
		std::unique_lock<std::recursive_mutex> data_lk(data_m);
		ignored = true;
	}
	void ignoreAndProcess() {
		ignore();
		processImpl();
	}
	void ignoreAndClear() {
		std::unique_lock<std::recursive_mutex> data_lk(data_m);
		ignored = true;
		while (!data.empty())
			data.pop();
	}
	void ignoreAndInterrupt() {
		interrupt();
		processImpl();
	}
	void interrupt() {
		std::unique_lock<std::recursive_mutex> data_lk(data_m);
		recieved_cv.notify_all();
	}
	template<typename F, typename = decltype(std::declval<F>()()) >
	void invokeAsync(F&& func) {
		std::unique_lock<std::recursive_mutex> data_lk(data_m);
		if (!ignored) {
			data.emplace(std::forward<F>(func));
			recieved_cv.notify_all();
		}
	}
	template<typename F, typename = decltype(std::declval<F>()()) >
	void invokeSync(F&& func) {
		std::condition_variable_any executed_cv;
		std::unique_lock<std::recursive_mutex> data_lk(data_m);
		if (!ignored) {
			data.emplace(std::forward<F>(func), &executed_cv);
			recieved_cv.notify_all();
			executed_cv.wait(data_lk);
		}
	}
	void process() {
		processImpl();
	}
	void waitAndProcess() {
		{
			std::unique_lock<std::recursive_mutex> data_lk(data_m);
			if (data.empty())
				recieved_cv.wait(data_lk);
		}
		processImpl();
	}
	void wait() {
		std::unique_lock<std::recursive_mutex> data_lk(data_m);
		if (data.empty())
			recieved_cv.wait(data_lk);
	}
};

} //namespace vl