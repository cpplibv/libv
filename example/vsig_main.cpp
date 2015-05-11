// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

#include <algorithm>
#include <chrono>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <typeinfo>
#include <unordered_set>

#include "vl/sig/signal.hpp"

//// <editor-fold defaultstate="collapsed" desc="DumpFunc ...">
//int DUMPFUNC_HELPER_LAYER = 0;
//
//void DUMPFUNC_HELPER_LAYER_PRINT() {
//	for (int i = 0; i < DUMPFUNC_HELPER_LAYER; i++) std::cout << '\t';
//}
//struct DUMPFUNC_HELPER {
//
//	DUMPFUNC_HELPER() {
//		++DUMPFUNC_HELPER_LAYER;
//	}
//
//	~DUMPFUNC_HELPER() {
//		--DUMPFUNC_HELPER_LAYER;
//		DUMPFUNC_HELPER_LAYER_PRINT();
//		std::cout << "}\n";
//	}
//};
//#define DUMPFUNC() DUMPFUNC_HELPER_LAYER_PRINT(); \
//std::cout << __PRETTY_FUNCTION__ << " {\n"; DUMPFUNC_HELPER dumpfunc_helper;
//// </editor-fold>
//class MyClass : public vl::Trackable<vl::MultiThreadGlobal> {
//public:
//	int a = 0;
//
//	void foo(int x) {
//		std::cout << __PRETTY_FUNCTION__ << " " << x << ' ' << a++ << std::endl;
//	}
//
//	void bar() {
//		std::cout << __PRETTY_FUNCTION__ << " " << a++ << std::endl;
//	}
//
//	static void sfoo(int x) {
//		std::cout << __PRETTY_FUNCTION__ << " " << x << std::endl;
//	}
//
//	~MyClass() {
//		disconnectAll();
//		a = 0;
//	}
//};
//
//void foo(int x) {
//	std::cout << __PRETTY_FUNCTION__ << " " << x << std::endl;
//}
//
//vl::Signal<int> vsig;
//vl::Signal<int> slot;
//
//void baz() {
//	for (int i = 0; i < 1000; i++) {
//		std::cout << "- " << std::flush;
//		vsig.fire(i);
//	}
//}

//using Clock = std::chrono::high_resolution_clock;
//using Clock = std::chrono::steady_clock;
//using TimePoint = std::chrono::time_point<Clock>;
//
//void print_ms(const TimePoint& point) {
//	using Ms = std::chrono::milliseconds;
//	const Clock::duration since_epoch = point.time_since_epoch();
//	std::cout << std::chrono::duration_cast<Ms>(since_epoch).count() << " ms\n";
//}

//using namespace std::chrono;
//using namespace std::literals::chrono_literals;

//------------

int main(int, char**) {
	vl::Signal<void(int)> sig0;
	vl::Signal<long(int)> sig1;
	
	return 0;
}