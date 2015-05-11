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
//TODO P5: Adaptiv Signal// This is somewhat important
//				- [in/out]put (same, generic lambda...)
//TODO P5: Routing Signal
//				+ [set/get]Condition(SignalRouter)
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

template <typename RType>
class Accumulator;

template <typename RType, typename... Args>
class SignalImpl : public TrackableBase {
	using CallablePtr = std::unique_ptr<Callable<RType, Args...>>;

protected:
	std::multiset<TrackableBase*> inputs;
	std::multimap<TrackableBase*, CallablePtr> outputs;

	Accumulator<RType>* accumulator;
protected:
	virtual void connect(TrackableBase* ptr, bool reflect = true);
	virtual void disconnect(TrackableBase* ptr, bool reflect = true);
public:
	SignalImpl();
	SignalImpl(Accumulator<RType>* accumulator);
public:
	Accumulator<RType>* getAccumulator() const;
	void setAccumulator(Accumulator<RType>* accumulator);
public:
	virtual RType fire(Args... args);
protected:
	template <typename R2 = RType, typename = typename std::enable_if<std::is_void<R2>::value>::type>
	RType fireImpl(Args... args);
	template <typename R2 = RType, typename = typename std::enable_if<is_non_void<R2>::value>::type>
	RType fireImpl(Args... args, int /*ignored*/ = 0);

public:
	void clearInput();
	void clearOutput();

	size_t inputSize() const;
	size_t outputSize() const;
	void input(SignalImpl<RType, Args...>& sig);
	void input(SignalImpl<RType, Args...> * const sig);

	void operator()(const std::function<RType(Args...)>& func);
	template<typename Object>
	void operator()(RType(Object::*func)(Args...), Object& obj);
	template<typename Object>
	void operator()(RType(Object::*func)(Args...), Object* obj);
	void operator()(SignalImpl<RType, Args...>& slot);
	void operator()(SignalImpl<RType, Args...> * const slot);

	void output(const std::function<RType(Args...)>& func);
	template<typename Object>
	void output(RType(Object::*func)(Args...), Object& obj);
	template<typename Object>
	void output(RType(Object::*func)(Args...), Object* obj);
	void output(SignalImpl<RType, Args...>& slot);
	void output(SignalImpl<RType, Args...> * const slot);

	virtual ~SignalImpl();
};

template<typename... Args>
struct Signal : public SignalImpl<void, Args...> {
	using SignalImpl<void, Args...>::SignalImpl;
};

template<typename R, typename... Args>
struct Signal<R(Args...)> : public SignalImpl<R, Args...> {
	using SignalImpl<R, Args...>::SignalImpl;
};

//==============================================================================
template <typename RType, typename... Args>
SignalImpl<RType, Args...>::SignalImpl() :
	accumulator(Accumulator<RType>::get()) { }
template <typename RType, typename... Args>
SignalImpl<RType, Args...>::SignalImpl(Accumulator<RType>* accumulator) :
	accumulator(accumulator) { }

//------------------------------------------------------------------------------
template <typename RType, typename... Args>
inline Accumulator<RType>* SignalImpl<RType, Args...>::getAccumulator() const {
	return accumulator;
}
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::setAccumulator(Accumulator<RType>* accumulator) {
	this->accumulator = accumulator;
}

//------------------------------------------------------------------------------
template <typename RType, typename... Args>
void SignalImpl<RType, Args...>::connect(TrackableBase* ptr, bool reflect) {
	if (reflect)
		ptr->connect(this, false);
}
template <typename RType, typename... Args>
void SignalImpl<RType, Args...>::disconnect(TrackableBase* ptr, bool reflect) {
	outputs.erase(ptr);
	inputs.erase(ptr);

	if (reflect && ptr)
		ptr->disconnect(this, false);
}

//------------------------------------------------------------------------------
template <typename RType, typename... Args> template <typename, typename>
inline RType SignalImpl<RType, Args...>::fireImpl(Args... args) {
	for (auto& output : outputs) {
		(*output.second)(std::forward<Args>(args)...);
	}
}
template <typename RType, typename... Args> template <typename, typename>
inline RType SignalImpl<RType, Args...>::fireImpl(Args... args, int /*ignored*/) {
	RType result = accumulator->begin();
	for (auto& output : outputs) {
		if (!accumulator->add(result, (*output.second)(std::forward<Args>(args)...)))
			return result;
	}
	return result;
}

//------------------------------------------------------------------------------
template <typename RType, typename... Args>
RType SignalImpl<RType, Args...>::fire(Args... args) {
	return fireImpl(std::forward<Args>(args)...);
}

//------------------------------------------------------------------------------
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::operator()(const std::function<RType(Args...)>& func) {
	return this->output(func);
}
template <typename RType, typename... Args> template <typename Object>
inline void SignalImpl<RType, Args...>::operator()(RType(Object::*func)(Args...), Object& obj) {
	return this->output(func, obj);
}
template <typename RType, typename... Args> template <typename Object>
inline void SignalImpl<RType, Args...>::operator()(RType(Object::*func)(Args...), Object * const obj) {
	return this->output(func, obj);
}
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::operator()(SignalImpl<RType, Args...>& slot) {
	return this->output(slot);
}
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::operator()(SignalImpl<RType, Args...> * const slot) {
	return this->output(slot);
}

//------------------------------------------------------------------------------
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::output(const std::function<RType(Args...)>& func) {
	outputs.emplace(nullptr, CallablePtr(new CallableImpl<RType, Args...>(func)));
}
template <typename RType, typename... Args> template <typename Object>
inline void SignalImpl<RType, Args...>::output(RType(Object::*func)(Args...), Object& obj) {
	output(func, &obj);
}
template <typename RType, typename... Args> template <typename Object>
inline void SignalImpl<RType, Args...>::output(RType(Object::*func)(Args...), Object * const obj) {
	static_assert(std::is_base_of<TrackableBase, Object>::value,
			"Object type has to be Derived from TrackableBase (You may want to consider inheriting from Trackable).");
	outputs.emplace(obj, CallablePtr(new CallableImpl<RType, Args...>(obj, func)));
	static_cast<TrackableBase*> (obj)->connect(this, false);
}
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::output(SignalImpl<RType, Args...>& slot) {
	output(&slot);
}
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::output(SignalImpl<RType, Args...> * const slot) {
	slot->input(this);
}

//------------------------------------------------------------------------------
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::clearInput() {
	while (!inputs.empty())
		disconnect(*inputs.begin());
}
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::clearOutput() {
	while (!outputs.empty())
		disconnect(outputs.begin()->first);
}

//------------------------------------------------------------------------------
template <typename RType, typename... Args>
inline size_t SignalImpl<RType, Args...>::outputSize() const {
	return outputs.size();
}
template <typename RType, typename... Args>
inline size_t SignalImpl<RType, Args...>::inputSize() const {
	return inputs.size();
}

//------------------------------------------------------------------------------
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::input(SignalImpl<RType, Args...>& sig) {
	input(&sig);
}
template <typename RType, typename... Args>
inline void SignalImpl<RType, Args...>::input(SignalImpl<RType, Args...> * const sig) {
	sig->output(&SignalImpl<RType, Args...>::fire, this);
	inputs.emplace(sig);
}

//------------------------------------------------------------------------------
template <typename RType, typename... Args>
SignalImpl<RType, Args...>::~SignalImpl() {
	clearInput();
	clearOutput();
}

//==============================================================================
//==============================================================================

template <typename... Args>
class CapacitivSignalImpl : public SignalImpl<void, Args...> {
private:
	std::vector<std::tuple<typename std::remove_reference<Args>::type...>> argQue;
private:
	template<std::size_t... Is>
	inline void flushHelper(std::index_sequence<Is...>);
public:
	virtual void fire(Args... args) override;
	inline void flush();
};

//------------------------------------------------------------------------------

template<typename... T>
struct CapacitivSignal : public CapacitivSignalImpl<T...> {
	using CapacitivSignalImpl<T...>::CapacitivSignalImpl;
};

template<typename R, typename... T>
struct CapacitivSignal<R(T...)> : public CapacitivSignalImpl<T...> {
	static_assert(!std::is_same<R, void>::value, "Return type cannot be non void in CapacitivSignal"); //<<<
	using CapacitivSignalImpl<T...>::CapacitivSignalImpl;
};

//==============================================================================
template<typename... Args>
template<std::size_t... Is>
inline void CapacitivSignalImpl<Args...>::flushHelper(std::index_sequence<Is...>) {
	for (auto& item : argQue) {
		SignalImpl<void, Args...>::fire(std::get<Is>(item)...);
	}
	argQue.clear();
}

//------------------------------------------------------------------------------
template<typename... Args>
void CapacitivSignalImpl<Args...>::fire(Args... args) {
	argQue.emplace_back(args...);
}

//------------------------------------------------------------------------------
template<typename... Args>
inline void CapacitivSignalImpl<Args...>::flush() {
	flushHelper(std::index_sequence_for < Args...>{});
}

//------------------------------------------------------------------------------

} //namespace vl