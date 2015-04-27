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
	std::cout << "VSIG is header only." << std::endl;
	
	vl::Signal<void(int)> sig0;
	vl::Signal<long(int)> sig1;
	
	//------------

	//	auto ns = 1h + 2m + 3s + +4ms + 5us + 6ns;
	//	std::cout << ns.count() << std::endl;

	//------------
	//	const TimePoint default_value = TimePoint(); // (1)
	//	print_ms(default_value); // 0 ms
	//
	//	const Clock::duration duration_4_seconds = std::chrono::seconds(4);
	//	const TimePoint time_point_4_seconds(duration_4_seconds); // (2)
	//	// 4 seconds from start of epoch
	//	print_ms(time_point_4_seconds); // 4000 ms
	//
	//	const TimePoint time_point_now = Clock::now(); // (3)
	//	print_ms(time_point_now); // 43098276 ms
	//------------

	//	std::chrono::system_clock::time_point x = std::chrono::system_clock::now();
	//	std::chrono::steady_clock::time_point y = std::chrono::steady_clock::now();
	//	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> x2 = std::chrono::system_clock::now();
	//	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> y2 = std::chrono::steady_clock::now();
	//	
	//	std::time_t cx = std::chrono::system_clock::to_time_t(x);
	//	std::time_t cy = std::chrono::steady_clock::to_time_t(y);
	//	std::cout << std::put_time(std::localtime(&cx), "%F %T") << std::endl;
	//	std::cout << cy << std::endl;

	//------------

	//		vsig.output(slot);
	//		for (int i = 0; i < 1000; i++) {
	//			std::thread d(baz);
	//			{
	//				MyClass mc;
	//				slot.output(mc, &MyClass::foo);
	//				std::this_thread::sleep_for(std::chrono::milliseconds(10));
	//			}
	//			d.join();
	//		}

	//------------

	//	vl::Signal<int> signal;
	//	vl::Slot<int> slot;
	//
	//	signal.output(slot);
	//
	//	slot.output(std::bind([](int x, int y) {
	//		std::cout << "Yup! " << x << ' ' << y << std::endl;
	//	}, std::placeholders::_1, 30));
	//	//		slot(1);
	//	signal(1);

	//------------

	//	vl::Signal<const int&> signal;
	//	vl::CapacitivSignal<const int&> relay;
	//	vl::Signal<const int&> slot;
	//
	//	signal.output(relay);
	//	relay.output(slot);
	//
	//	slot.output([](const int& x) {
	//		std::cout << x << std::endl;
	//	});
	//
	//	std::cout << "0" << std::endl;
	//	signal.fire(-1);
	//	std::cout << "1" << std::endl;
	//	signal.fire(-2);
	//	relay.flush();
	//	std::cout << "2" << std::endl;
	//	signal.fire(-3);
	//	std::cout << "3" << std::endl;
	//	relay.flush();
	//	std::cout << "4" << std::endl;

	//------------

	//	MyClass mc;
	//
	//	vl::Signal<> vsignal;
	//	vl::Slot<> vslot;
	//	vsignal.output(vslot);
	//	vslot.output([] {
	//		std::cout << "void" << std::endl;
	//	});
	//	vslot.output(mc, &MyClass::bar);
	//	vsignal();

	//	vl::Signal<int> signal;
	//	vl::Slot<int> slot;
	//	  
	//	signal.output(slot);
	//
	//	slot.output([] {
	//		return 2;
	//	});
	//		slot.output([] {
	//			return 1;
	//		});
	//
	//	std::cout << signal() << std::endl;
	//	slot.setAccumulator(Accumulator2<int>::get());
	//	std::cout << signal() << std::endl;

	/*	//	bar([&] {std::cout << __PRETTY_FUNCTION__ << temp << std::endl; });
		//	bar([] {std::cout << __PRETTY_FUNCTION__ << std::endl; });
		//	bar(fff);
			{
				vl::Signal<void, int> signal;
				vl::Signal<void, int> relay;
				vl::Slot<void, int> slot;

		//		signal.output(relay);
				relay.input(signal);
				relay.output(slot);

				//		signal.output(slot);
				//		signal.output(&slot);
		//		slot.input(relay);

				slot.output(foo);
				slot.output(&foo);
				slot.output(MyClass::sfoo);
				slot.output(&MyClass::sfoo);
				slot.output([](int x) {
					std::cout << __PRETTY_FUNCTION__ << " " << x << std::endl;
				});
				slot.output([temp](int x) {
					std::cout << __PRETTY_FUNCTION__ << " " << x << " + [temp] " << temp << std::endl;
				});
				slot.output([ = ](int x){
					std::cout << __PRETTY_FUNCTION__ << " " << x << " + [=] " << temp << std::endl;
				});
				slot.output([&](int x) {
					std::cout << __PRETTY_FUNCTION__ << " " << x << " + [&] " << temp << std::endl;
				});

				{
					MyClass mc;
					slot.output(&mc, &MyClass::foo);
					std::cout << "===========================================" << std::endl;
					signal(1);
				}
				temp++;
				std::cout << "===========================================" << std::endl;
				signal(2);


				std::cout << "===========================================" << std::endl;
				std::cout << "vl::Signal input count: " << signal.inputSize() << std::endl;
				std::cout << "vl::Signal output count: " << signal.outputSize() << std::endl;
				std::cout << "vl::Slot input count: " << slot.inputSize() << std::endl;
				std::cout << "vl::Slot output count: " << slot.outputSize() << std::endl;

			}
	 */
	return 0;
}