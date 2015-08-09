// File:   resource_manager.hpp

#pragma once

// vl
#include <vl/cache.hpp>
#include <vl/context.hpp>
#include <vl/worker_thread.hpp>
// std
#include <memory>

#include <iostream>

namespace vl {


// -------------------------------------------------------------------------------------------------

struct ContextHost {
	virtual std::shared_ptr<vl::WorkerThread> get(vl::ContextID contextID) = 0;
};

class LoadStep {
private:
	std::shared_ptr<vl::WorkerThread> worker;
	std::shared_ptr<std::function<void()>> func;
public:
	std::shared_ptr<LoadStep> nextStep;
	void execute() {
		worker->executeAsync([func = func, nextStep = nextStep]{
			(*func)();
			if (nextStep)
				nextStep->execute();
		});
	}
public:
	LoadStep(
			std::shared_ptr<vl::WorkerThread> worker,
			std::shared_ptr<std::function<void()>> func) :
		worker(worker),
		func(func) { }
};

class FunctionSequence {
private:
	std::shared_ptr<ContextHost> contextHost;
	std::shared_ptr<LoadStep> firstStep;
	std::shared_ptr<LoadStep> lastStep;
private:
	template <typename T, typename R, typename... Args, typename... FArgs>
	void addStep(vl::ContextID contextID, R(T::*mfn)(Args...), std::weak_ptr<T> obj, FArgs&&... args) {
		auto func = std::make_shared<std::function<void()>>([ = ](){
			if (auto target = obj.lock())
				(target.get()->*mfn)(std::move(args)...);
		});
		auto context = contextHost->get(contextID);

		auto step = std::make_shared<LoadStep>(context, func);

		if (!firstStep) {
			lastStep = firstStep = step;
		} else {
			lastStep->nextStep = step;
			lastStep = lastStep->nextStep;
		}
	}
public:
	template <typename T, typename R, typename... Args, typename... FArgs>
	inline void addStepIO(R(T::*mfunc)(Args...), std::weak_ptr<T> obj, FArgs&&... args) {
		addStep(vl::ContextID::IO, mfunc, obj, std::forward<FArgs>(args)...);
	}
	template <typename T, typename R, typename... Args, typename... FArgs>
	inline void addStepIO(R(T::*mfunc)(Args...), std::shared_ptr<T> obj, FArgs&&... args) {
		addStep(vl::ContextID::IO, mfunc, std::weak_ptr<T>(obj), std::forward<FArgs>(args)...);
	}
	template <typename T, typename R, typename... Args, typename... FArgs>
	inline void addStepGL(R(T::*mfunc)(Args...), std::weak_ptr<T> obj, FArgs&&... args) {
		addStep(vl::ContextID::GL, mfunc, obj, std::forward<FArgs>(args)...);
	}
	template <typename T, typename R, typename... Args, typename... FArgs>
	inline void addStepGL(R(T::*mfunc)(Args...), std::shared_ptr<T> obj, FArgs&&... args) {
		addStep(vl::ContextID::GL, mfunc, std::weak_ptr<T>(obj), std::forward<FArgs>(args)...);
	}
	template <typename T, typename R, typename... Args, typename... FArgs>
	inline void addStepAny(R(T::*mfunc)(Args...), std::weak_ptr<T> obj, FArgs&&... args) {
		addStep(vl::ContextID::ANY, mfunc, obj, std::forward<FArgs>(args)...);
	}
	template <typename T, typename R, typename... Args, typename... FArgs>
	inline void addStepAny(R(T::*mfunc)(Args...), std::shared_ptr<T> obj, FArgs&&... args) {
		addStep(vl::ContextID::ANY, mfunc, std::weak_ptr<T>(obj), std::forward<FArgs>(args)...);
	}
	template <typename T, typename R, typename... Args, typename... FArgs>
	inline void addStepNet(R(T::*mfunc)(Args...), std::weak_ptr<T> obj, FArgs&&... args) {
		addStep(vl::ContextID::NET, mfunc, obj, std::forward<FArgs>(args)...);
	}
	template <typename T, typename R, typename... Args, typename... FArgs>
	inline void addStepNet(R(T::*mfunc)(Args...), std::shared_ptr<T> obj, FArgs&&... args) {
		addStep(vl::ContextID::NET, mfunc, std::weak_ptr<T>(obj), std::forward<FArgs>(args)...);
	}
	void execute() {
		if (firstStep)
			firstStep->execute();
	}
public:
	FunctionSequence(std::shared_ptr<ContextHost> ex) : contextHost(ex) { }
};

} //namespace vl