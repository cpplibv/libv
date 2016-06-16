// File: Signal.hpp, Created on 2014. május 14. 20:05, Author: Vader

#pragma once

#define LIBV_SIGNAL_VERSION_MAJOR 1
#define LIBV_SIGNAL_VERSION_MINOR 0

// TODO P1: Use ordered small vector for Signal-Slot-Trackable
// TODO P1: Rework Signal-Slot multithreading with shared_reqursive_mutex
// 			http://en.cppreference.com/w/cpp/thread/shared_mutex
//			Or with lock free way, or both...
// TODO P1: Rework connection and trackable objects
// TODO P2: Branch signals into two: sync and async signals, this will ease my pain about them...
// TODO P2: Forbid &&
// TODO P2: Change from trackable to shared or weak ptr
// 			signal target variable ami egy shared_ptr és a signal kap róla
// 			egy weak_ptr-t így amikor külden rá infot csak kidobja magából, ígykönnyen
// 			lehet "lokális változót" asnyc módosítani
// TODO P2: Add "Signal Hub" approach where signals are light weight and a main hub stores
//			everything and member signal "markers" (or what should i call them...) are just an
//			accessors to the hub wrapping the necessary minimal information.
//			SignalHub is a trackable
//			Both method should be kept
// TODO P3: Remove reference, remove constness from results type?
// TODO P3: In Signal Hub-Proxy a connection can be represented with only 1 ptr where the last last
//			bit of the address can represent the direction!

// TODO P5: AdaptivSignal [in/out]put (same, generic lambda...)
// TODO P5: RoutingSignal [set/get]Condition(SignalRouter)
// TODO P5: PrioritySignal - Modified capacitiv where the storage is a priority que
// 			May consider a "predicate" function for generating priority
// TODO P5: UniqueSignal - Modified capacitiv where the storage is unique
// 			May consider a "compare" function for determining uniqueness
// TODO P5: ConditionalSignal - Forward the call only if the predicate function allows it
// TODO P5: HistorySignal - Stores and forward calls but also distibute them to late subsribers.
//			Also a template size_t N for limit max call memory number
// TODO P5: TransformSignal - Manipulating the arguments flowing through it using a
// 			manipulator function. Similar to std::transform.
// TODO P5: AsnycSignal - Put the fire method and the arguments into a worker
// 			thread que to call this signal in async mode. (template executor?)
// TODO P5: SnycSignal - Put the fire method and the arguments into a worker
// 			thread que to call this signal in sync mode. (template executor?)

// TODO P5: Another raw idea for light weight signals: SignalPtr which is only a pointer to the real
//			signal. It could also store some additional information such as FilterSignalPtr could
//			store the filter information that will be applied to the output. (Component::onClick
//			could be a FilterSignalPtr that points to the UI signal and stores the boundaries)
// TODO PD: Documentation of Signal-Slot. The boost version may be relevant
// 			http://www.boost.org/doc/libs/1_59_0/doc/html/signals2/reference.html
// TODO PD: Turotial of Signal-Slot. The boost version may be relevant
// 			http://www.boost.org/doc/libs/1_59_0/doc/html/signals2/tutorial.html

// TODO P?: Accumulator refactor into object?
// TODO P?: Make a connection object to trackables and move thread policy into it (?)
//Fact: Signal ctor needs to be able to handle an adaptor?
//Fact: It is possible to do adaptiv slots! it is very possible generic lambda?

#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <type_traits>
#include <vector>

#include "accumulator.hpp"
#include "trackable.hpp"

namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename RType>
class Accumulator;

// === Signal ======================================================================================

template <typename RType, typename... Args>
class SignalImpl : public TrackableBase {
protected:
	mutable std::recursive_mutex mutex;
	std::multiset<TrackableBase*> inputs;
	std::multimap<TrackableBase*, std::function<RType(Args...) >> outputs;

	Accumulator<RType>* accumulator;
protected:
	virtual void connect(TrackableBase* ptr, bool reflect = true) override {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		if (reflect)
			ptr->connect(this, false);
	}
	virtual void disconnect(TrackableBase* ptr, bool reflect = true) override {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		outputs.erase(ptr);
		inputs.erase(ptr);

		if (reflect && ptr)
			ptr->disconnect(this, false);
	}
	// ---------------------------------------------------------------------------------------------
public:
	SignalImpl() :
		accumulator(Accumulator<RType>::get()) { }
	SignalImpl(Accumulator<RType>* accumulator) :
		accumulator(accumulator) { }
	SignalImpl(const SignalImpl<RType, Args...>& other) = delete;

	// ---------------------------------------------------------------------------------------------
public:
	inline Accumulator<RType>* getAccumulator() const {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		return accumulator;
	}
	inline void setAccumulator(Accumulator<RType>* accumulator) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		this->accumulator = accumulator;
	}
	// ---------------------------------------------------------------------------------------------
public:
	virtual RType fire(Args... args) {
		return fireImpl(std::forward<Args>(args)...);
	}
	inline RType operator()(Args... args) {
		return fire(std::forward<Args>(args)...);
	}

protected:
	template <typename R2 = RType, typename =
	typename std::enable_if<std::is_void<R2>::value>::type>
	RType fireImpl(Args... args) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		for (auto& output : outputs) {
			output.second(std::forward<Args>(args)...);
		}
	}
	template <typename R2 = RType, typename =
	typename std::enable_if<!std::is_void<R2>::value>::type>
	RType fireImpl(Args... args, int /*ignored*/ = 0) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		RType result = accumulator->begin();
		for (auto& output : outputs) {
			if (!accumulator->add(result, output.second(std::forward<Args>(args)...)))
				return result;
		}
		return result;
	}

	// ---------------------------------------------------------------------------------------------
public:
	void clearInput() {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		while (!inputs.empty())
			disconnect(*inputs.begin());
	}
	void clearOutput() {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		while (!outputs.empty())
			disconnect(outputs.begin()->first);
	}
	inline size_t inputSize() const {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		return inputs.size();
	}
	inline size_t outputSize() const {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		return outputs.size();
	}

	// ---------------------------------------------------------------------------------------------
	inline void input(SignalImpl<RType, Args...>& sig) {
		sig.output(this);
	}
	void input(SignalImpl<RType, Args...> * const sig) {
		sig->output(this);
	}

	// ---------------------------------------------------------------------------------------------
	void output(const std::function<RType(Args...)>& func) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		outputs.emplace(nullptr, func);
	}
	template<typename Object> inline void output(RType(Object::*func)(Args...), Object& obj) {
		output(func, &obj);
	}
	template<typename Object> void output(RType(Object::*func)(Args...), Object* obj) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		static_assert(std::is_base_of<TrackableBase, Object>::value,
				"Object type has to be Derived from TrackableBase "
				"(You may want to consider inheriting from libv::Trackable).");
		outputs.emplace(obj, [obj, func](Args... args) {
			(obj->*func)(std::forward<Args>(args)...);
		});
		static_cast<TrackableBase*>(obj)->connect(this, true);
	}
	inline void output(SignalImpl<RType, Args...>& slot) {
		output(&slot);
	}
	inline void output(SignalImpl<RType, Args...>* const slot) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		output(&SignalImpl<RType, Args...>::fire, slot);
		slot->inputs.emplace(this);
	}

	// ---------------------------------------------------------------------------------------------
	virtual ~SignalImpl() {
		clearInput();
		clearOutput();
	}
};

// === CapacitivSignal =============================================================================

template <typename... Args>
class CapacitivSignalImpl : public SignalImpl<void, Args...> {
private:
	std::vector<std::tuple<typename std::remove_reference<Args>::type...>> argQue;
private:
	template<std::size_t... Is>
	inline void flushHelper(std::index_sequence<Is...>) {
		for (auto& item : argQue) {
			SignalImpl<void, Args...>::fire(std::get<Is>(item)...);
		}
		argQue.clear();
	}
public:
	virtual void fire(Args... args) override {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		argQue.emplace_back(args...);
	}
	inline void flush() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		flushHelper(std::index_sequence_for < Args...>{});
	}
};

// === SwitchSignal ===========================================================================

template <typename R, typename... Args>
struct SwitchSignalImpl : public SignalImpl<R, Args...> {
	bool enabled = true;
public:
	inline void enable() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		enabled = true;
	}
	inline void disable() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		enabled = false;
	}
	virtual R fire(Args... args) override {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		if (enabled)
			return SignalImpl < R, Args...>::fire(std::forward<Args>(args)...);
	}
};

// === HistorySignal ===============================================================================

template <typename... Args>
class HistorySignalImpl : public SignalImpl<void, Args...> {
private:
	std::vector<std::tuple<typename std::remove_reference<Args>::type...>> history;
private:
	template<typename F, std::size_t... Is>
	inline void flushHelper(F&& func, std::index_sequence<Is...>) {
		for (auto& item : history) {
			func(std::get<Is>(item)...);
		}
	}
public:
	// TODO P5: History signal output auto-flush
	//	template<typename... Args>
	//	void output(Args... args) {
	//		flushHelper(std::index_sequence_for<Args...>{});
	//	}
	inline size_t historySize() const {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		return history.size();
	}
	virtual void fire(Args... args) override {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		history.emplace_back(args...);
		SignalImpl<void, Args...>::fire(args...);
	}
	inline void clearHistory() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		history.clear();
	}
};

// === Aliases =====================================================================================

template<typename... Args>
struct Signal : public SignalImpl<void, Args...> {
	using SignalImpl<void, Args...>::SignalImpl;
};

template<typename R, typename... Args>
struct Signal<R(Args...)> : public SignalImpl<R, Args...> {
	using SignalImpl<R, Args...>::SignalImpl;
};

template<typename... T>
struct CapacitivSignal : public CapacitivSignalImpl<T...> {
	using CapacitivSignalImpl<T...>::CapacitivSignalImpl;
};

template<typename R, typename... T>
struct CapacitivSignal<R(T...)> : public CapacitivSignalImpl<T...> {
	static_assert(!std::is_same<R, void>::value, "Return type must be void in CapacitivSignal");
	using CapacitivSignalImpl<T...>::CapacitivSignalImpl;
};

template<typename... Args>
struct SwitchSignal : public SwitchSignalImpl<void, Args...> {
	using SwitchSignalImpl<void, Args...>::SwitchSignalImpl;
};

template<typename R, typename... Args>
struct SwitchSignal<R(Args...)> : public SwitchSignalImpl<R, Args...> {
	using SwitchSignalImpl<R, Args...>::SwitchSignalImpl;
};

template<typename... T>
struct HistorySignal : public HistorySignalImpl<T...> {
	using HistorySignalImpl<T...>::HistorySignalImpl;
};

template<typename R, typename... T>
struct HistorySignal<R(T...)> : public HistorySignalImpl<T...> {
	static_assert(!std::is_same<R, void>::value, "Return type must be void in HistorySignal");
	using HistorySignalImpl<T...>::HistorySignalImpl;
};

} //namespace libv