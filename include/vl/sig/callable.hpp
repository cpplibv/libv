// File: Callable.hpp, Created on 2014. május 14. 20:05, Author: Vader

#pragma once

#include <functional>
#include <memory>

namespace vl {
	//=== std::function
	
	template <typename RType, typename... Args>
	struct Callable {
		virtual RType operator()(Args... args) = 0;
		virtual ~Callable() = 0;
	};
	template <typename RType, typename... Args>
	Callable<RType, Args...>::~Callable() { }

	//------------------------------------------------------------------------------

	template <typename RType, typename... Args>
	class CallableImpl : public Callable<RType, Args...> {
	private:
		typedef Callable<RType, Args...> Callable_t;
		std::unique_ptr<Callable_t> target;
	public:
		CallableImpl(const std::function<RType(Args...)>& target) :
			target(new SingleFunctionCaller(target)) { }
		template<typename Object>
		CallableImpl(Object* obj, RType(Object::*func)(Args...)) :
			target(new MemberFunctionCaller<Object>(obj, func)) { }
		RType operator()(Args... args) {
			return (*target)(std::forward<Args>(args)...);
		}
	private:

		class SingleFunctionCaller : public Callable_t {
			std::function<RType(Args...) > targetFunction;
		public:
			SingleFunctionCaller(const std::function<RType(Args...)>& target) : targetFunction(target) { }
			RType operator()(Args... args) {
				return targetFunction(std::forward<Args>(args)...);
			}
		};

		template <typename Object>
		class MemberFunctionCaller : public Callable_t {
			Object* targetObject;
			RType(Object::*targetFunction)(Args...);
		public:
			MemberFunctionCaller(Object* targetObject, RType(Object::*targetFunction)(Args...))
				: targetObject(targetObject),
				targetFunction(targetFunction) { }
			RType operator()(Args... args) {
				return (targetObject->*targetFunction)(std::forward<Args>(args)...);
			}
		};
	};
	
} //namespace vl