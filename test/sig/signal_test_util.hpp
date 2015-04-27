// File: SignalTestUtil.hpp, Created on 2015. január 26. 17:15, Author: Vader

#pragma once

#include "vl/sig/signal.hpp"

#include <functional>
#include <vector>

#define SpyResultTypeFor(SIGNAL) SpyResultFor<decltype(SIGNAL)>::type

template<typename...> struct SpyResultFor;

template<typename... Args>
struct SpyResultFor<vl::Signal<Args...>> {
	using type = std::vector<std::tuple<Args...>>;
};
template<typename... Args>
std::function<void(Args...)> spyInto(std::vector<std::tuple<Args...>>& result) {
	return [&result](Args... args) {
		result.push_back(std::make_tuple(args...));
	};
}

// --- Dummy targets -------------------------------------------------------------------------------

template<typename R = void, typename... Args>
R dummyGlobalFunction(Args...) { }

template<typename R = void, typename... Args>
struct dummyType : public vl::Trackable<> {
	R memberFunction(Args...) { }
	static R staticFunction(Args...) { }
};