// File: SignalTestUtil.hpp, Created on 2015. január 26. 17:15, Author: Vader

#pragma once

#include <vl/sig/signal.hpp>

#include <functional>
#include <vector>

#define SpyResultTypeFor(SIGNAL) typename SpyResultFor<decltype(SIGNAL)>::type

template<typename...> struct SpyResultFor;

template<typename... Args>
struct SpyResultFor<vl::Signal<Args...>> {
	using type = std::vector<std::tuple<typename std::remove_reference<Args>::type...>>;
};
template<typename R, typename... Args>
struct SpyResultFor<vl::Signal<R(Args...)>> {
	using type = std::vector<std::tuple<typename std::remove_reference<Args>::type...>>;
};
template<typename... Args>
struct SpyResultFor<vl::CapacitivSignal<Args...>> {
	using type = std::vector<std::tuple<typename std::remove_reference<Args>::type...>>;
};
template<typename R, typename... Args>
struct SpyResultFor<vl::CapacitivSignal<R(Args...)>> {
	using type = std::vector<std::tuple<typename std::remove_reference<Args>::type...>>;
};
template<typename... Args>
struct SpyResultFor<vl::SwitchSignal<Args...>> {
	using type = std::vector<std::tuple<typename std::remove_reference<Args>::type...>>;
};
template<typename R, typename... Args>
struct SpyResultFor<vl::SwitchSignal<R(Args...)>> {
	using type = std::vector<std::tuple<typename std::remove_reference<Args>::type...>>;
};
template<typename... Args>
struct SpyResultFor<vl::HistorySignal<Args...>> {
	using type = std::vector<std::tuple<typename std::remove_reference<Args>::type...>>;
};
template<typename R, typename... Args>
struct SpyResultFor<vl::HistorySignal<R(Args...)>> {
	using type = std::vector<std::tuple<typename std::remove_reference<Args>::type...>>;
};

template<typename R, typename... Args, typename T>
std::function<R(Args...)> spyInto(T& result) {
	return [&result](Args... args) {
		result.emplace_back(args...);
	};
}

// --- Dummy targets -------------------------------------------------------------------------------
template<typename R = void, typename... Args>
R dummyGlobalFunction(Args...) { }

template<typename R = void, typename... Args>
struct dummyType : public vl::Trackable {
	R memberFunction(Args...) { }
	static R staticFunction(Args...) { }
};