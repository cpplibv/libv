// File: test_slice.cpp - Created on 2018.09.04. 09:37 - Author: Vader

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/utility/approx.hpp>
// std
#include <cmath>
// pro
#include <libv/math/remap.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("remap on zero", "[libv.math.remap]") {
	CHECK(std::isnan(libv::remap<float>(0, 0, 0, 0, 0)));
	CHECK(libv::remap<int32_t>(0, 0, 1, 0, 1) == 0);
	CHECK(libv::remap<float>(0, 0, 1, 0, 1) == libv::Approx<float>(0));
}

TEST_CASE("remap from zero", "[libv.math.remap]") {
	CHECK(libv::remap<float>(0, 0, 10, 0, 100) == libv::Approx<float>(0));
	CHECK(libv::remap<float>(1, 0, 10, 0, 100) == libv::Approx<float>(10));
	CHECK(libv::remap<float>(10, 0, 10, 0, 100) == libv::Approx<float>(100));

	CHECK(libv::remap<float>(0, 0, 10, 0, -100) == libv::Approx<float>(-0));
	CHECK(libv::remap<float>(1, 0, 10, 0, -100) == libv::Approx<float>(-10));
	CHECK(libv::remap<float>(10, 0, 10, 0, -100) == libv::Approx<float>(-100));

	CHECK(libv::remap<float>(-0, 0, -10, 0, 100) == libv::Approx<float>(0));
	CHECK(libv::remap<float>(-1, 0, -10, 0, 100) == libv::Approx<float>(10));
	CHECK(libv::remap<float>(-10, 0, -10, 0, 100) == libv::Approx<float>(100));

	CHECK(libv::remap<float>(-0, 0, -10, 0, -100) == libv::Approx<float>(-0));
	CHECK(libv::remap<float>(-1, 0, -10, 0, -100) == libv::Approx<float>(-10));
	CHECK(libv::remap<float>(-10, 0, -10, 0, -100) == libv::Approx<float>(-100));
}

TEST_CASE("remap interval", "[libv.math.remap]") {
	CHECK(libv::remap<float>(5, 5, 10, 50, 100) == libv::Approx<float>(50));
	CHECK(libv::remap<float>(6, 5, 10, 50, 100) == libv::Approx<float>(60));
	CHECK(libv::remap<float>(10, 5, 10, 50, 100) == libv::Approx<float>(100));

	CHECK(libv::remap<float>(5, 5, 10, -50, -100) == libv::Approx<float>(-50));
	CHECK(libv::remap<float>(6, 5, 10, -50, -100) == libv::Approx<float>(-60));
	CHECK(libv::remap<float>(10, 5, 10, -50, -100) == libv::Approx<float>(-100));

	CHECK(libv::remap<float>(-5, -5, -10, 50, 100) == libv::Approx<float>(50));
	CHECK(libv::remap<float>(-6, -5, -10, 50, 100) == libv::Approx<float>(60));
	CHECK(libv::remap<float>(-10, -5, -10, 50, 100) == libv::Approx<float>(100));

	CHECK(libv::remap<float>(-5, -5, -10, -50, -100) == libv::Approx<float>(-50));
	CHECK(libv::remap<float>(-6, -5, -10, -50, -100) == libv::Approx<float>(-60));
	CHECK(libv::remap<float>(-10, -5, -10, -50, -100) == libv::Approx<float>(-100));
}

TEST_CASE("remap extrapolates values outside", "[libv.math.remap]") {
	CHECK(libv::remap<float>(-10, 0, 10, 0, 100) == libv::Approx<float>(-100));
	CHECK(libv::remap<float>(20, 0, 10, 0, 100) == libv::Approx<float>(200));
	CHECK(libv::remap<float>(-10, 0, 10, 0, -100) == libv::Approx<float>(100));
	CHECK(libv::remap<float>(20, 0, 10, 0, -100) == libv::Approx<float>(-200));

	CHECK(libv::remap<float>(-10, 0, 10, -100, 100) == libv::Approx<float>(-300));
	CHECK(libv::remap<float>(20, 0, 10, -100, 100) == libv::Approx<float>(300));
	CHECK(libv::remap<float>(-10, 0, 10, 100, -100) == libv::Approx<float>(300));
	CHECK(libv::remap<float>(20, 0, 10, 100, -100) == libv::Approx<float>(-300));
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("remap_clamp on zero", "[libv.math.remap_clamp]") {
	CHECK(std::isnan(libv::remap_clamp<float>(0, 0, 0, 0, 0)));
	CHECK(libv::remap_clamp<int32_t>(0, 0, 1, 0, 1) == 0);
	CHECK(libv::remap_clamp<float>(0, 0, 1, 0, 1) == libv::Approx<float>(0));
}

TEST_CASE("remap_clamp from zero", "[libv.math.remap_clamp]") {
	CHECK(libv::remap_clamp<float>(0, 0, 10, 0, 100) == libv::Approx<float>(0));
	CHECK(libv::remap_clamp<float>(1, 0, 10, 0, 100) == libv::Approx<float>(10));
	CHECK(libv::remap_clamp<float>(10, 0, 10, 0, 100) == libv::Approx<float>(100));

	CHECK(libv::remap_clamp<float>(0, 0, 10, 0, -100) == libv::Approx<float>(-0));
	CHECK(libv::remap_clamp<float>(1, 0, 10, 0, -100) == libv::Approx<float>(-10));
	CHECK(libv::remap_clamp<float>(10, 0, 10, 0, -100) == libv::Approx<float>(-100));

	CHECK(libv::remap_clamp<float>(-0, 0, -10, 0, 100) == libv::Approx<float>(0));
	CHECK(libv::remap_clamp<float>(-1, 0, -10, 0, 100) == libv::Approx<float>(10));
	CHECK(libv::remap_clamp<float>(-10, 0, -10, 0, 100) == libv::Approx<float>(100));

	CHECK(libv::remap_clamp<float>(-0, 0, -10, 0, -100) == libv::Approx<float>(-0));
	CHECK(libv::remap_clamp<float>(-1, 0, -10, 0, -100) == libv::Approx<float>(-10));
	CHECK(libv::remap_clamp<float>(-10, 0, -10, 0, -100) == libv::Approx<float>(-100));
}

TEST_CASE("remap_clamp interval", "[libv.math.remap_clamp]") {
	CHECK(libv::remap_clamp<float>(5, 5, 10, 50, 100) == libv::Approx<float>(50));
	CHECK(libv::remap_clamp<float>(6, 5, 10, 50, 100) == libv::Approx<float>(60));
	CHECK(libv::remap_clamp<float>(10, 5, 10, 50, 100) == libv::Approx<float>(100));

	CHECK(libv::remap_clamp<float>(5, 5, 10, -50, -100) == libv::Approx<float>(-50));
	CHECK(libv::remap_clamp<float>(6, 5, 10, -50, -100) == libv::Approx<float>(-60));
	CHECK(libv::remap_clamp<float>(10, 5, 10, -50, -100) == libv::Approx<float>(-100));

	CHECK(libv::remap_clamp<float>(-5, -5, -10, 50, 100) == libv::Approx<float>(50));
	CHECK(libv::remap_clamp<float>(-6, -5, -10, 50, 100) == libv::Approx<float>(60));
	CHECK(libv::remap_clamp<float>(-10, -5, -10, 50, 100) == libv::Approx<float>(100));

	CHECK(libv::remap_clamp<float>(-5, -5, -10, -50, -100) == libv::Approx<float>(-50));
	CHECK(libv::remap_clamp<float>(-6, -5, -10, -50, -100) == libv::Approx<float>(-60));
	CHECK(libv::remap_clamp<float>(-10, -5, -10, -50, -100) == libv::Approx<float>(-100));
}

TEST_CASE("remap_clamp saturates values outside", "[libv.math.remap_clamp]") {
	CHECK(libv::remap_clamp<float>(-10, 0, 10, 0, 100) == libv::Approx<float>(0));
	CHECK(libv::remap_clamp<float>(20, 0, 10, 0, 100) == libv::Approx<float>(100));
	CHECK(libv::remap_clamp<float>(-10, 0, 10, 0, -100) == libv::Approx<float>(0));
	CHECK(libv::remap_clamp<float>(20, 0, 10, 0, -100) == libv::Approx<float>(-100));

	CHECK(libv::remap_clamp<float>(-10, 0, 10, -100, 100) == libv::Approx<float>(-100));
	CHECK(libv::remap_clamp<float>(20, 0, 10, -100, 100) == libv::Approx<float>(100));
	CHECK(libv::remap_clamp<float>(-10, 0, 10, 100, -100) == libv::Approx<float>(100));
	CHECK(libv::remap_clamp<float>(20, 0, 10, 100, -100) == libv::Approx<float>(-100));
}
