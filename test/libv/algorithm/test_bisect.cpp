// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// std
#include <cstdint>
// pro
#include <libv/algorithm/bisect.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("bisect ramp up", "[libv.algorithm.bisect]") {
	int called = 0;

	auto func = [&called](int guess) {
		called++;
		return guess - 5;
	};

	CHECK(libv::bisect_rampup_3way<int32_t>(func) == 5);
	CHECK(std::exchange(called, 0) == 2);

	CHECK(libv::bisect_rampup_3way(func, 5) == 5);
	CHECK(std::exchange(called, 0) == 1);

	CHECK(libv::bisect_rampup_3way(func, 5, 1) == 5);
	CHECK(std::exchange(called, 0) == 1);

	CHECK(libv::bisect_rampup_3way(func, 0, 5) == 5);
	CHECK(std::exchange(called, 0) == 2);

	CHECK(libv::bisect_rampup_3way(func, 5, -1) == 5);
	CHECK(std::exchange(called, 0) == 1);

	CHECK(libv::bisect_rampup_3way(func, 0, -1) == 5);
	CHECK(std::exchange(called, 0) == 5);

	CHECK(libv::bisect_rampup_3way(func, 0, -5) == 5);
	CHECK(std::exchange(called, 0) == 2);
}

TEST_CASE("bisect ramp up negative root", "[libv.algorithm.bisect]") {
	int called = 0;

	auto func = [&called](int guess) {
		called++;
		return guess + 5;
	};

	CHECK(libv::bisect_rampup_3way<int32_t>(func) == -5);
	CHECK(std::exchange(called, 0) == 2);

	CHECK(libv::bisect_rampup_3way(func, 5) == -5);
	CHECK(std::exchange(called, 0) == 2);

	CHECK(libv::bisect_rampup_3way(func, 5, 1) == -5);
	CHECK(std::exchange(called, 0) == 8);

	CHECK(libv::bisect_rampup_3way(func, 0, 5) == -5);
	CHECK(std::exchange(called, 0) == 2);

	CHECK(libv::bisect_rampup_3way(func, 5, -1) == -5);
	CHECK(std::exchange(called, 0) == 8);

	CHECK(libv::bisect_rampup_3way(func, 0, -1) == -5);
	CHECK(std::exchange(called, 0) == 5);

	CHECK(libv::bisect_rampup_3way(func, 0, -5) == -5);
	CHECK(std::exchange(called, 0) == 2);
}

TEST_CASE("bisect ramp up range scan test", "[libv.algorithm.bisect]") {
	for (int32_t i = -2000; i < 2000; ++i) {
		auto func = [i](int32_t guess) {
			return guess - i;
		};
		CHECK(libv::bisect_rampup_3way<int32_t>(func) == i);
	}
	for (int32_t i = -2000; i < 2000; ++i) {
		auto func = [i](int32_t guess) {
			return guess - 45;
		};
		CHECK(libv::bisect_rampup_3way<int32_t>(func, i) == 45);
	}
	for (int32_t i = -2000; i < 2000; ++i) {
		auto func = [i](int32_t guess) {
			return guess - 45;
		};
		CHECK(libv::bisect_rampup_3way<int32_t>(func, 0, i) == 45);
	}
}

// -------------------------------------------------------------------------------------------------
