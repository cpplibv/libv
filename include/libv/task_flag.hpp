// File:   task_task.hpp

#pragma once

#include <mutex>
#include <vector>
#include <functional>

namespace libv {

// -------------------------------------------------------------------------------------------------

template<typename T = void>
class TaskFlag {
	bool passed;
	T result_;
	std::mutex mutex;
	std::vector<std::function<void(T)>> targets; // TODO P1: use small_vector
public:
	void complete(T result) {
		std::lock_guard<std::mutex> lock(mutex);
		passed = true;
		result_ = result;
		for (auto& target : targets)
			target(result_);
		targets.clear();
	}
	void undo() {
		std::lock_guard<std::mutex> lock(mutex);
		passed = false;
		result_ = T{};
	}
	void subsribe(std::function<void(T)>&& cb) {
		std::lock_guard<std::mutex> lock(mutex);
		if (passed)
			cb(result_);
		else
			targets.emplace_back(std::move(cb));
	}
	inline bool isComplete() {
		std::lock_guard<std::mutex> lock(mutex);
		return passed;
	}
	inline T result() {
		std::lock_guard<std::mutex> lock(mutex);
		return result_;
	}
	TaskFlag() : passed(false) { }
};

template<>
class TaskFlag<void> {
	bool passed;
	std::mutex mutex;
	std::vector<std::function<void() >> targets; // TODO P1: use small_vector
public:
	void complete() {
		std::lock_guard<std::mutex> lock(mutex);
		passed = true;
		for (auto& target : targets)
			target();
		targets.clear();
	}
	void undo() {
		std::lock_guard<std::mutex> lock(mutex);
		passed = false;
	}
	void subsribe(std::function<void()>&& cb) {
		std::lock_guard<std::mutex> lock(mutex);
		if (passed)
			cb();
		else
			targets.emplace_back(std::move(cb));
	}
	inline bool isComplete() {
		std::lock_guard<std::mutex> lock(mutex);
		return passed;
	}
	TaskFlag() : passed(false) { }
};

} // namespace libv