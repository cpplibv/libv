// File: Signal.hpp, Created on 2014. május 14. 20:05, Author: Vader

#pragma once

#define VL_SIGNAL_VERSION_MAJOR 1
#define VL_SIGNAL_VERSION_MINOR 0

//TODO P1: copy constructor ?? ??
//TODO P1: Connection object
//TODO P2: Accumulator refactor into object?
//TODO P2: Make a connection object to trackables and move thread policy into it (?)
//TODO P2: Callable is removable with generic lambda? or some kind of lambda
//TODO P2: is it solved to properly forwarding && references?
//TODO P2: Move CapacitivSignal into separate file
//TODO P3: Remove reference, remove constness from results type?
//TODO P4: multithread (idea template spec (enum... for:
//			- Single-threaded
//			- Multithreaded Global
//			- Multithreaded Local))
//
//
//TODO P5: AdaptivSignal [in/out]put (same, generic lambda...)
//TODO P5: RoutingSignal [set/get]Condition(SignalRouter)
//TODO P5: UniqueSignal
//TODO P5: PrioritySignal
//
//
//TODO P5: c++14 workaround for index_sequence if c++14 is not present
//TODO P5: Switching from functional to union magic to eliminate dynamic memory allocation
//
//Fact: Signal ctor needs to be able to handle an adaptor
//Fact: It is possible to do adaptiv slots! it is very possible
//			std::generic lambda and the merge of every callable will do the trick
//
//??: kurva jó lenne ha Signal<> onClose; -nál
//			Signal<void, VFrame*> onClose; e helyett azt tudnám mondani, hogy
//			Signal<> onClose(generic lambda::_0, this);
//			igy kibaszott zsirúl lehetne hivni onClose();-al onClose(this); helyett
//			(na jól van, talán ez nem szükséges)
//
//https://www.youtube.com/watch?v=C2LcIp56i-8

#include <map>
#include <memory>
#include <set>
#include <type_traits>
#include <vector>

#include "accumulator.hpp"
#include "callable.hpp"
#include "trackable.hpp"

template <typename T>
struct is_non_void : std::is_same<typename std::is_void<T>::type, std::false_type::type> {
};

namespace vl {

// -------------------------------------------------------------------------------------------------

template <typename RType>
class Accumulator;

// === Signal ======================================================================================

template <typename RType, typename... Args>
class SignalImpl : public TrackableBase {
	using CallablePtr = std::unique_ptr<Callable<RType, Args...>>;

protected:
	std::multiset<TrackableBase*> inputs;
	std::multimap<TrackableBase*, CallablePtr> outputs;

	Accumulator<RType>* accumulator;
protected:
	virtual void connect(TrackableBase* ptr, bool reflect = true) {
		if (reflect)
			ptr->connect(this, false);
	}
	virtual void disconnect(TrackableBase* ptr, bool reflect = true) {
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
	// ---------------------------------------------------------------------------------------------
public:
	inline Accumulator<RType>* getAccumulator() const {
		return accumulator;
	}
	inline void setAccumulator(Accumulator<RType>* accumulator) {
		this->accumulator = accumulator;
	}
	// ---------------------------------------------------------------------------------------------
public:
	virtual RType fire(Args... args) {
		return fireImpl(std::forward<Args>(args)...);
	}

protected:
	template <typename R2 = RType, typename = typename std::enable_if<std::is_void<R2>::value>::type>
	RType fireImpl(Args... args) {
		for (auto& output : outputs) {
			(*output.second)(std::forward<Args>(args)...);
		}
	}
	template <typename R2 = RType, typename = typename std::enable_if<is_non_void<R2>::value>::type>
	RType fireImpl(Args... args, int /*ignored*/ = 0) {
		RType result = accumulator->begin();
		for (auto& output : outputs) {
			if (!accumulator->add(result, (*output.second)(std::forward<Args>(args)...)))
				return result;
		}
		return result;
	}

	// ---------------------------------------------------------------------------------------------
public:
	void clearInput() {
		while (!inputs.empty())
			disconnect(*inputs.begin());
	}
	void clearOutput() {
		while (!outputs.empty())
			disconnect(outputs.begin()->first);
	}
	inline size_t inputSize() const {
		return inputs.size();
	}
	inline size_t outputSize() const {
		return outputs.size();
	}

	// ---------------------------------------------------------------------------------------------
	inline void input(SignalImpl<RType, Args...>& sig) {
		input(&sig);
	}
	void input(SignalImpl<RType, Args...> * const sig) {
		sig->output(&SignalImpl<RType, Args...>::fire, this);
		inputs.emplace(sig);
	}

	// ---------------------------------------------------------------------------------------------
	inline void operator()(const std::function<RType(Args...)>& func) {
		return this->output(func);
	}
	template<typename Object> inline void operator()(RType(Object::*func)(Args...), Object& obj) {
		return this->output(func, obj);
	}
	template<typename Object> inline void operator()(RType(Object::*func)(Args...), Object* obj) {
		return this->output(func, obj);
	}
	inline void operator()(SignalImpl<RType, Args...>& slot) {
		return this->output(slot);
	}
	inline void operator()(SignalImpl<RType, Args...> * const slot) {
		return this->output(slot);
	}

	// ---------------------------------------------------------------------------------------------
	void output(const std::function<RType(Args...)>& func) {
		outputs.emplace(nullptr, CallablePtr(new CallableImpl<RType, Args...>(func)));
	}
	template<typename Object> inline void output(RType(Object::*func)(Args...), Object& obj) {
		output(func, &obj);
	}
	template<typename Object> void output(RType(Object::*func)(Args...), Object* obj) {
		static_assert(std::is_base_of<TrackableBase, Object>::value,
				"Object type has to be Derived from TrackableBase (You may want to consider inheriting from vl::Trackable).");
		outputs.emplace(obj, CallablePtr(new CallableImpl<RType, Args...>(obj, func)));
		static_cast<TrackableBase*> (obj)->connect(this, false);
	}
	inline void output(SignalImpl<RType, Args...>& slot) {
		output(&slot);
	}
	inline void output(SignalImpl<RType, Args...> * const slot) {
		slot->input(this);
	}

	// ---------------------------------------------------------------------------------------------
	virtual ~SignalImpl() {
		clearInput();
		clearOutput();
	}
};

template<typename... Args>
struct Signal : public SignalImpl<void, Args...> {
	using SignalImpl<void, Args...>::SignalImpl;
};

template<typename R, typename... Args>
struct Signal<R(Args...)> : public SignalImpl<R, Args...> {
	using SignalImpl<R, Args...>::SignalImpl;
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
		argQue.emplace_back(args...);
	}
	inline void flush() {
		flushHelper(std::index_sequence_for < Args...>{});
	}
};

// -------------------------------------------------------------------------------------------------

template<typename... T>
struct CapacitivSignal : public CapacitivSignalImpl<T...> {
	using CapacitivSignalImpl<T...>::CapacitivSignalImpl;
};

template<typename R, typename... T>
struct CapacitivSignal<R(T...)> : public CapacitivSignalImpl<T...> {
	static_assert(!std::is_same<R, void>::value, "Return type cannot be non void in CapacitivSignal"); //<<<
	using CapacitivSignalImpl<T...>::CapacitivSignalImpl;
};

// === ConditionalSignal ===========================================================================

template <typename R, typename... Args>
struct ConditionalSignalImpl : public SignalImpl<R, Args...> {
	bool enabled = true;
public:
	inline void enable() {
		enabled = true;
	}
	inline void disable() {
		enabled = false;
	}
	virtual R fire(Args... args) override {
		if (enabled)
			return SignalImpl < R, Args...>::fire(std::forward<Args>(args)...);
	}
};

// -------------------------------------------------------------------------------------------------

template<typename... Args>
struct ConditionalSignal : public ConditionalSignalImpl<void, Args...> {
	using ConditionalSignalImpl<void, Args...>::ConditionalSignalImpl;
};

template<typename R, typename... Args>
struct ConditionalSignal<R(Args...)> : public ConditionalSignalImpl<R, Args...> {
	using ConditionalSignalImpl<R, Args...>::ConditionalSignalImpl;
};

} //namespace vl




//template<typename CallSignature, typename... Moduls> struct SignalBase;
//template<typename CallSignature, typename Modul, typename... Moduls> struct Adaptiv;
//template<typename CallSignature, typename Modul, typename... Moduls> struct Capacitiv;
//template<typename CallSignature, typename Modul, typename... Moduls> struct Conditional;
//template<typename CallSignature, typename Modul, typename... Moduls> struct Priority;
//template<typename CallSignature, typename Modul, typename... Moduls> struct Routing;
//template<typename CallSignature, typename Modul, typename... Moduls> struct Unique;


//template<typename Base, typename R, typename... Args> struct A;
//template<typename Base, typename R, typename... Args> struct B;
//template<typename Base, typename R, typename... Args> struct C;
//
//template<template<typename...> class Base, typename R, typename... BaseArgs, typename... Args>
//struct A<R(Args...), Base<BaseArgs...>> : Base<R(Args...), BaseArgs...> {
//	using FireReturn = typename Base<R(Args...), BaseArgs...>::FireReturn;
//
//	int a;
//};
//
//template<template<typename...> class Base, typename R, typename... BaseArgs, typename... Args>
//struct B<R(Args...), Base<BaseArgs...>> : Base<R(Args...), BaseArgs...> {
//	using FireReturn = typename Base<R(Args...), BaseArgs...>::FireReturn;
//
//	int b;
//};
//
//template<template<template<template<typename...> class> class> class Base, typename R, typename... BaseArgs, typename... Args>
//struct C<R(Args...), Base<BaseArgs...>> : Base<R(Args...), BaseArgs...> {
////	using FireReturn = typename Base<R(Args...), BaseArgs...>::FireReturn;
//	using FireReturn = R;
//
//	int c;
//};
//
//
