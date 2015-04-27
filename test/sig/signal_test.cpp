// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <UnitTest++/UnitTest++.h>

#include "signal_test_util.hpp"
#include "vl/sig/signal.hpp"

void foo() { }

using namespace vl;

SUITE(SignalTests) {
	TEST(SignalConstruct) {
		Signal<> testObj;

		CHECK(testObj.inputSize() == 0);
		CHECK(testObj.outputSize() == 0);
	}

	// --- Input / Output ------------------------------------------------------------------------------

	TEST(SignalInputSignal) {
		Signal<> source, target;

		target.input(source);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 1);
		CHECK(target.inputSize() == 1);
		CHECK(target.outputSize() == 0);

		target.input(&source);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 2);
		CHECK(target.inputSize() == 2);
		CHECK(target.outputSize() == 0);
	}

	TEST(SignalOutputSignal) {
		Signal<> source, target;

		source.output(target);

		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 1);
		CHECK(target.inputSize() == 1);
		CHECK(target.outputSize() == 0);

		source.output(&target);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 2);
		CHECK(target.inputSize() == 2);
		CHECK(target.outputSize() == 0);
	}

	TEST(SignalRelaySignal) {
		Signal<> source, relay, target;

		relay.input(source);
		relay.output(target);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 1);
		CHECK(relay.inputSize() == 1);
		CHECK(relay.outputSize() == 1);
		CHECK(target.inputSize() == 1);
		CHECK(target.outputSize() == 0);
	}

	TEST(SignalOutputLamda) {
		Signal<> source;

		source.output([] {
		});
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 1);
	}

	TEST(SignalOutputStdFunction) {
		Signal<> source;

		source.output(std::function<void()>(dummyGlobalFunction<void>));
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 1);
	}

	TEST(SignalOutputStdBind) {
		Signal<> source;

		source.output(std::bind(dummyGlobalFunction<void, int>, 1));
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 1);
	}

	TEST(SignalOutputGloablFunction) {
		Signal<> source;

		source.output(dummyGlobalFunction<>);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 1);

		source.output(&dummyGlobalFunction<>);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 2);
	}

	TEST(SignalOutputStaticFunction) {
		Signal<> source;

		source.output(dummyType<>::staticFunction);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 1);

		source.output(&dummyType<>::staticFunction);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 2);
	}

	TEST(SignalOutputMemberFunction) {
		Signal<> source;
		auto target = dummyType<>();

		source.output(&dummyType<>::memberFunction, target);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 1);

		source.output(&dummyType<>::memberFunction, &target);
		CHECK(source.inputSize() == 0);
		CHECK(source.outputSize() == 2);
	}

	// --- Connection Lifetime -------------------------------------------------------------------------

	TEST(SignalConnectionSignalLifeTime) {
		Signal<> source;
		{
			Signal<> target;
			source.output(target);
			CHECK(source.outputSize() == 1);
		}
		CHECK(source.outputSize() == 0);
	}

	TEST(SignalConnectionTrackableLifeTime) {
		Signal<> source;
		{
			dummyType<> dummyTarget;
			source.output(&dummyType<>::memberFunction, dummyTarget);
			CHECK(source.outputSize() == 1);
		}
		CHECK(source.outputSize() == 0);
	}

	TEST(SignalConnectionGlobalLifeTime) {
		Signal<> source;
		{
			source.output(dummyGlobalFunction<>);
			CHECK(source.outputSize() == 1);
		}
		CHECK(source.outputSize() == 1);
	}

	// --- Fire ----------------------------------------------------------------------------------------

	TEST(SignalFire) {
		Signal<> source;
		SpyResultTypeFor(source) result;
		source.output(spyInto(result));

		CHECK(result.size() == 0);
		source.fire();
		CHECK(result.size() == 1);
		source.fire();
		source.fire();
		CHECK(result.size() == 3);
	}

	TEST(SignalRelayFire) {
		Signal<> source, relay;
		SpyResultTypeFor(source) result;
		source.output(relay);
		relay.output(spyInto(result));

		CHECK(result.size() == 0);
		source.fire();
		CHECK(result.size() == 1);
		source.fire();
		source.fire();
		CHECK(result.size() == 3);
	}

	TEST(SignalFireArgs) {
		Signal<int, int> source;
		SpyResultTypeFor(source) result;
		source.output(spyInto(result));

		source.fire(0, 1);
		source.fire(1, 2);
		CHECK(result.size() == 2);
		CHECK(result[0] == std::make_tuple(0, 1));
		CHECK(result[1] == std::make_tuple(1, 2));
	}

	TEST(SignalFireConstArgs) {
		Signal<const int, const int> source;
		SpyResultTypeFor(source) result;
		source.output(spyInto(result));

		source.fire(0, 1);
		source.fire(1, 2);
		CHECK(result.size() == 2);
		CHECK(result[0] == std::make_tuple(0, 1));
		CHECK(result[1] == std::make_tuple(1, 2));
	}

	TEST(SignalFireStringArgs) {
		Signal<const char*, const std::string&> source;
		SpyResultTypeFor(source) result;
		source.output(spyInto(result));

		const char* hello = "Hello";
		std::string world = std::string("World!");

		source.fire(hello, "World!");
		source.fire(hello, world);
		CHECK(result.size() == 2);
		CHECK(std::get<0>(result[0]) == hello);
		CHECK(std::get<1>(result[0]) == "World!");
		CHECK(std::get<1>(result[0]) == world);
		CHECK(std::get<0>(result[1]) == hello);
		CHECK(std::get<1>(result[1]) == "World!");
		CHECK(std::get<1>(result[1]) == world);
	}

	TEST(SignalFireArgsLValueRef) {
		bool recieved = false;
		int x = 2;
		std::string y = "Test string";
		Signal<int&, std::string&> source;
		source.output([&x, &y, &recieved](int& a, std::string & b) {
			recieved = true;
			CHECK(a == 2);
					CHECK(b == std::string("Test string"));
					CHECK(&a == &x);
					CHECK(&b == &y);
		});
		source.fire(x, y);
		CHECK(recieved);
	}

	TEST(SignalFireArgsConstLValueRef) {
		bool recieved = false;
		const int x = 2;
		const std::string y = "Test string";
		Signal<const int&, const std::string&> source;
		source.output([&x, &y, &recieved](const int& a, const std::string & b) {
			recieved = true;
			CHECK(a == 2);
					CHECK(b == std::string("Test string"));
					CHECK(&a == &x);
					CHECK(&b == &y);
		});
		source.fire(x, y);
		CHECK(recieved);
	}

	TEST(SignalFireArgsRValueRef) {
		bool recieved = false;
		int x = 2;
		std::string y = "Test string";
		Signal<int&&, std::string&&> source;
		source.output([&x, &y, &recieved](int&& a, std::string && b) {
			recieved = true;
			CHECK(a == 2);
					CHECK(b == std::string("Test string"));
					CHECK(&a == &x);
					CHECK(b.data() == y.data());
		});
		source.fire(std::move(x), std::move(y));
		CHECK(recieved);
	}

	// --- Return --------------------------------------------------------------------------------------

	TEST(SignalReturnTypeVoidFromDefault) {
		Signal<> source0;
		Signal<int> source1;

		static_assert(std::is_same < decltype(source0.fire()), void>::value, "");
		static_assert(std::is_same < decltype(source1.fire(0)), void>::value, "");
	}

	TEST(SignalReturnTypeVoidFromSignature) {
		Signal<void() > source0;
		Signal<void(void) > source1;
		Signal<void(int) > source2;

		static_assert(std::is_same < decltype(source0.fire()), void>::value, "");
		static_assert(std::is_same < decltype(source1.fire()), void>::value, "");
		static_assert(std::is_same < decltype(source2.fire(0)), void>::value, "");
	}

	TEST(SignalReturnTypeRTypeFromSignature) {
		Signal<int() > source0;
		Signal<int(void) > source1;
		Signal<int(int) > source2;

		static_assert(std::is_same < decltype(source0.fire()), int>::value, "");
		static_assert(std::is_same < decltype(source1.fire()), int>::value, "");
		static_assert(std::is_same < decltype(source2.fire(0)), int>::value, "");
	}

	TEST(SignalReturnValue) {
		Signal<int() > source;
		source.output([] {
			return 42;
		});
		CHECK(source.fire() == 42);
	}

	// --- Operator() ----------------------------------------------------------------------------------
	// --- Accumulator ---------------------------------------------------------------------------------

	TEST(SignalReturnDefaultAccumulation) {
		Signal<int() > source;
		source.output([] {
			return 12;
		});
		source.output([] {
			return 30;
		});
		CHECK(source.fire() == 42);
	}

	// --- Disconnect ----------------------------------------------------------------------------------
	// --- Clear ---------------------------------------------------------------------------------------
	// --- DisconnectAllSignal -------------------------------------------------------------------------
}

