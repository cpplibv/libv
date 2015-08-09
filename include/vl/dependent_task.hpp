// File:   dependent_task.hpp

#pragma once

#include <mutex>
#include <vector>
#include <functional>
//#include <boost/container/small_vector.hpp>

namespace vl {

// -------------------------------------------------------------------------------------------------

template<typename T>
class DependentTask {
	bool passed;
	T _result;
	std::mutex mutex;
	std::vector<std::function<void(T)>> targets; //TODO P1: use small_vector
public:
	void complete(T result) {
		std::lock_guard<std::mutex> lock(mutex);
		passed = true;
		result = result;
		for (auto& target : targets)
			target(_result);
		targets.clear();
	}
	void subsribe(std::function<void(T)>&& cb) {
		std::lock_guard<std::mutex> lock(mutex);
		if (passed)
			cb(_result);
		else
			targets.emplace_back(std::move(cb));
	}
	DependentTask() : passed(false) { }
};

template<>
class DependentTask<void> {
	bool passed;
	std::mutex mutex;
	std::vector<std::function<void()>> targets; //TODO P1: use small_vector
public:
	void complete() {
		std::lock_guard<std::mutex> lock(mutex);
		passed = true;
		for (auto& target : targets)
			target();
		targets.clear();
	}
	void subsribe(std::function<void()>&& cb) {
		std::lock_guard<std::mutex> lock(mutex);
		if (passed)
			cb();
		else
			targets.emplace_back(std::move(cb));
	}
	DependentTask() : passed(false) { }
};

} //namespace vl