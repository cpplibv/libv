// File:   resource_manager.hpp

#pragma once

// vl
#include <vl/cache.hpp>
#include <vl/context_id.hpp>
// std
#include <functional>
// pro
#include "worker_thread.hpp"
#include "semaphore.hpp"

namespace vl {

struct TestResourceIOData {
	int n = 0;
	TestResourceIOData(int n) : n(n) { }
	bool load() {
		return true;
	}
	bool unload() {
		return true;
	}
	bool operator<(const TestResourceIOData& r) const {
		return n < r.n;
	}
	//	friend bool operator<(int t, const TestResourceIOData& r) {
	//		return t < r.n;
	//	}
	//	friend bool operator<(const TestResourceIOData& r, int t) {
	//		return r.n < t;
	//	}
	friend bool operator<(const std::tuple<int>& t, const TestResourceIOData& r) {
		return std::get<0>(t) < r.n;
	}
	friend bool operator<(const TestResourceIOData& r, const std::tuple<int>& t) {
		return r.n < std::get<0>(t);
	}
};

struct TestResourceGLData {
	int n = 0;
	vl::Semaphore loaded;
	TestResourceGLData(int n) :
		n(n) { }
	bool load() {
		loaded.raise();
		return true;
	}
	bool unload() {
		loaded.reset();
		return true;
	}
	bool operator<(const TestResourceGLData& r) const {
		return n < r.n;
	}
	//	friend bool operator<(int t, const TestResourceGLData& r) {
	//		return t < r.n;
	//	}
	//	friend bool operator<(const TestResourceGLData& r, int t) {
	//		return r.n < t;
	//	}
	friend bool operator<(const std::tuple<int>& t, const TestResourceGLData& r) {
		return std::get<0>(t) < r.n;
	}
	friend bool operator<(const TestResourceGLData& r, const std::tuple<int>& t) {
		return r.n < std::get<0>(t);
	}
};

namespace detail {

template <typename T>
inline auto applicableFunction_Load(T& p) -> decltype(p.load()) {
	return p.load();
}
//template <typename T>
//inline auto load(std::shared_ptr<T> p) -> decltype(load(*p)) {
//inline bool applicableFunctionLoad(std::shared_ptr<T> p) {
//	return load(*p);
//}
template <typename T>
inline auto applicableFunction_Unload(T& p) -> decltype(p.unload()) {
	return p.unload();
}
//template <typename T>
//inline auto applicableFunctionUnload(std::shared_ptr<T> p) -> decltype(unload(*p)) {
//	return unload(*p);
//}

} //namespace detail

template <typename T>
struct ContextLoader {

	struct Comparator : vl::less {
		template <typename... Args>
		inline bool operator()(const std::tuple<ThreadID, int, Args...>& t, const ContextLoader<T>& v) const {
			//Cutting down the front of the tuple to absorb context info, not too happy about this solution.
			return vl::less::operator()(reinterpret_cast<const std::tuple < Args...>&> (std::get<2>(t)), v.object);
		}
		template <typename... Args>
		inline bool operator()(const ContextLoader<T>& v, const std::tuple<ThreadID, int, Args...>& t) const {
			//Cutting down the front of the tuple to absorb context info, not too happy about this solution.
			return vl::less::operator()(v.object, reinterpret_cast<const std::tuple < Args...>&> (std::get<2>(t)));
		}
		template<typename Arg, 
		typename = vl::disable_if<vl::is_less_comparable<std::tuple<Arg>, T>>, 
		typename = vl::enable_if<vl::is_less_comparable<Arg, T>>>
		inline bool operator()(const std::tuple<ThreadID, int, Arg>& t, const ContextLoader<T>& v) const {
			return vl::less::operator()(std::get<2>(t), v.object);
		}
		template<typename Arg,
		typename = vl::disable_if<vl::is_less_comparable<T, std::tuple<Arg>>>, 
		typename = vl::enable_if<vl::is_less_comparable<T, Arg>>>
		inline bool operator()(const ContextLoader<T>& v, const std::tuple<ThreadID, int, Arg>& t) const {
			return vl::less::operator()(v.object, std::get<2>(t));
		}
		inline bool operator()(const ContextLoader<T>& a, const ContextLoader<T>& b) const {
			return vl::less::operator()(a, b);
		}
	};

	ThreadID contextID;
	int prio;
	T object;
	T* operator->() {
		return &object;
	}
	const T* operator->() const {
		return &object;
	}
	template <typename... Args>
	ContextLoader(const ThreadID& contextID, int prio, Args&&... args) :
		contextID(contextID),
		prio(prio),
		object(std::forward<Args>(args)...) {
		::vl::detail::applicableFunction_Load(object);
	}
	~ContextLoader() {
		::vl::detail::applicableFunction_Unload(object);
	}
	inline bool operator<(const ContextLoader<T>& v) const {
		return object < v.object;
	}
};

//struct LoadStep {
//	ThreadID contextID;
//	std::function<bool()> work;
//	int priority;
//	std::shared_ptr<LoadStep> nextStep;
//
//	LoadStep(ThreadID contextID, std::function<bool() > work, int priority) :
//		contextID(contextID), work(work), priority(priority) { }
//};
//
//class TestResourceManager;
//
//class LoadSequence {
//	friend class TestResourceManager;
//	std::shared_ptr<LoadStep> firstStep;
//	std::shared_ptr<LoadStep> lastStep;
//public:
//	LoadSequence() = default;
//	template<typename K, typename... Args>
//	LoadSequence& step(const ThreadID& contextID, int priority, std::shared_ptr<K> target, Args&&... args) {
//		//who, what, how
//		auto newStep = std::make_shared<LoadStep>(contextID, [&target] {
//			::vl::detail::applicableFunctionLoad(target);
//			return true;
//		}, priority);
//
//		if (!firstStep)
//			firstStep = lastStep = newStep;
//		else
//			lastStep = lastStep->nextStep = newStep;
//
//		return *this;
//	}
//};

using ContextedTestResourceIOData = ContextLoader<TestResourceIOData>;
using ContextedTestResourceGLData = ContextLoader<TestResourceGLData>;

class TestResourceManager {
private:
	vl::WorkerThread contextIO;
	vl::WorkerThread contextGL;
	vl::Cache<ContextedTestResourceIOData, ContextedTestResourceIOData::Comparator> cacheIO;
	vl::Cache<ContextedTestResourceGLData, ContextedTestResourceGLData::Comparator> cacheGL;

public:
	template <typename... Args>
	std::shared_ptr<ContextedTestResourceIOData> getIO(Args&&... args) {
		return cacheIO.get(std::forward<Args>(args)...);
	}
	template <typename... Args>
	std::shared_ptr<ContextedTestResourceGLData> getGL(Args&&... args) {
		return cacheGL.get(std::forward<Args>(args)...);
	}
};

struct TestResource {
	std::shared_ptr<ContextedTestResourceIOData> ioData;
	std::shared_ptr<ContextedTestResourceGLData> glData;
	TestResource(TestResourceManager& rm, int n) : n(n) {
		ioData = rm.getIO(ThreadID::IO, 50, n + 1);
		glData = rm.getGL(ThreadID::GL, 50, n - 1);
		//		glData->loaded.wait();
		glData->object.loaded.wait();
	}
private:
	int n;
};

} //namespace vl