// File: tuple.hpp, Created on 2013. december 6. 23:01, Author: Vader

#pragma once

// TODO P4: Rename to DelayedFunction or AsyncFunction(!)
// TODO P5: Use std::function

namespace libv {

template <typename... ArgList>
struct DelayedDispatcher {
	typedef void(&funcReference)(ArgList...);
private:
	funcReference func;
	std::tuple<typename std::remove_reference<ArgList>::type...> params;
public:
	DelayedDispatcher(funcReference func, std::tuple<ArgList...>&& params) : func(func), params(params) { }
	inline void dispatch() {
		callFunc(typename gens<sizeof...(ArgList)>::type());
	}
private:
	//C++14: use std integer sequnce
	template<int ...S>
	inline void callFunc(seq<S...>) {
		func(std::get<S>(params)...);
	}
};
template <typename... ArgList>
inline DelayedDispatcher<ArgList...> makeDelayedDispatch(void (&func)(ArgList...), ArgList... args) {
	return DelayedDispatcher < ArgList...>(func, std::move(std::make_tuple(args...)));
}
template<typename Object, typename FuncType, typename... ArgsList >
void member_helper(Object* o, FuncType func, ArgsList... args) {
	(o->*func)(args...);
}

} //namespace libv