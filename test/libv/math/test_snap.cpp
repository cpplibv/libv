// Project: libv.math, File: test/libv/math/test_snap.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/utility/approx.hpp>
// pro
#include <libv/math/snap.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("snap with zero", "[libv.math.snap]") {
	CHECK(libv::math::snap(0.0, 0.0) == libv::Approx(0.0));
	CHECK(libv::math::snap(1.0, 0.0) == libv::Approx(1.0));
	CHECK(libv::math::snap(1.5, 0.0) == libv::Approx(1.5));
	CHECK(libv::math::snap(2.0, 0.0) == libv::Approx(2.0));
	CHECK(libv::math::snap(-0.0, 0.0) == libv::Approx(-0.0));
	CHECK(libv::math::snap(-1.0, 0.0) == libv::Approx(-1.0));
	CHECK(libv::math::snap(-1.5, 0.0) == libv::Approx(-1.5));
	CHECK(libv::math::snap(-2.0, 0.0) == libv::Approx(-2.0));
}

TEST_CASE("snap with value", "[libv.math.snap]") {
	CHECK(libv::math::snap(0.0, 1.0) == libv::Approx(0.0));
	CHECK(libv::math::snap(1.0, 1.0) == libv::Approx(1.0));
	CHECK(libv::math::snap(1.5, 1.0) == libv::Approx(2.0));
	CHECK(libv::math::snap(2.0, 1.0) == libv::Approx(2.0));
	CHECK(libv::math::snap(-0.0, 1.0) == libv::Approx(-0.0));
	CHECK(libv::math::snap(-1.0, 1.0) == libv::Approx(-1.0));
	CHECK(libv::math::snap(-1.5, 1.0) == libv::Approx(-2.0));
	CHECK(libv::math::snap(-2.0, 1.0) == libv::Approx(-2.0));

	CHECK(libv::math::snap(0.0, 2.0) == libv::Approx(0.0));
	CHECK(libv::math::snap(0.999999999, 2.0) == libv::Approx(0.0));
	CHECK(libv::math::snap(1.0, 2.0) == libv::Approx(2.0));
	CHECK(libv::math::snap(1.5, 2.0) == libv::Approx(2.0));
	CHECK(libv::math::snap(2.0, 2.0) == libv::Approx(2.0));
	CHECK(libv::math::snap(-0.0, 2.0) == libv::Approx(-0.0));
	CHECK(libv::math::snap(-0.999999999, 2.0) == libv::Approx(-0.0));
	CHECK(libv::math::snap(-1.0, 2.0) == libv::Approx(-2.0));
	CHECK(libv::math::snap(-1.5, 2.0) == libv::Approx(-2.0));
	CHECK(libv::math::snap(-2.0, 2.0) == libv::Approx(-2.0));
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("snap_interval with zero", "[libv.math.snap_interval]") {
	CHECK(libv::math::snap_interval(0.0, 0.0, -10.0, 10.0) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(1.0, 0.0, -10.0, 10.0) == libv::Approx(1.0));
	CHECK(libv::math::snap_interval(1.5, 0.0, -10.0, 10.0) == libv::Approx(1.5));
	CHECK(libv::math::snap_interval(2.0, 0.0, -10.0, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(-0.0, 0.0, -10.0, 10.0) == libv::Approx(-0.0));
	CHECK(libv::math::snap_interval(-1.0, 0.0, -10.0, 10.0) == libv::Approx(-1.0));
	CHECK(libv::math::snap_interval(-1.5, 0.0, -10.0, 10.0) == libv::Approx(-1.5));
	CHECK(libv::math::snap_interval(-2.0, 0.0, -10.0, 10.0) == libv::Approx(-2.0));
}

TEST_CASE("snap_interval with value inside interval", "[libv.math.snap_interval]") {
	CHECK(libv::math::snap_interval(1.0, 1.0, 0.5, 10.0) == libv::Approx(1.0));
	CHECK(libv::math::snap_interval(1.5, 1.0, 0.5, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(2.0, 1.0, 0.5, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(-1.0, 1.0, -10.0, -0.5) == libv::Approx(-1.0));
	CHECK(libv::math::snap_interval(-1.5, 1.0, -10.0, -0.5) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-2.0, 1.0, -10.0, -0.5) == libv::Approx(-2.0));

	CHECK(libv::math::snap_interval(1.0, 1.0, 0.5, 10.0) == libv::Approx(1.0));
	CHECK(libv::math::snap_interval(1.5, 1.0, 0.5, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(2.0, 1.0, 0.5, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(-1.0, 1.0, -10.0, -0.5) == libv::Approx(-1.0));
	CHECK(libv::math::snap_interval(-1.5, 1.0, -10.0, -0.5) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-2.0, 1.0, -10.0, -0.5) == libv::Approx(-2.0));

	CHECK(libv::math::snap_interval(0.0, 1.0, 0.0, 10.0) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(1.0, 1.0, 0.0, 10.0) == libv::Approx(1.0));
	CHECK(libv::math::snap_interval(1.5, 1.0, 0.0, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(2.0, 1.0, 0.0, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(-0.0, 1.0, -10.0, 0.0) == libv::Approx(-0.0));
	CHECK(libv::math::snap_interval(-1.0, 1.0, -10.0, 0.0) == libv::Approx(-1.0));
	CHECK(libv::math::snap_interval(-1.5, 1.0, -10.0, 0.0) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-2.0, 1.0, -10.0, 0.0) == libv::Approx(-2.0));

	CHECK(libv::math::snap_interval(0.0, 1.0, -10.0, 10.0) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(1.0, 1.0, -10.0, 10.0) == libv::Approx(1.0));
	CHECK(libv::math::snap_interval(1.5, 1.0, -10.0, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(2.0, 1.0, -10.0, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(-0.0, 1.0, -10.0, 10.0) == libv::Approx(-0.0));
	CHECK(libv::math::snap_interval(-1.0, 1.0, -10.0, 10.0) == libv::Approx(-1.0));
	CHECK(libv::math::snap_interval(-1.5, 1.0, -10.0, 10.0) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-2.0, 1.0, -10.0, 10.0) == libv::Approx(-2.0));

	CHECK(libv::math::snap_interval(0.0, 2.0, -10.0, 10.0) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(0.999999999, 2.0, -10.0, 10.0) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(1.0, 2.0, -10.0, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(1.5, 2.0, -10.0, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(2.0, 2.0, -10.0, 10.0) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(-0.0, 2.0, -10.0, 10.0) == libv::Approx(-0.0));
	CHECK(libv::math::snap_interval(-0.999999999, 2.0, -10.0, 10.0) == libv::Approx(-0.0));
	CHECK(libv::math::snap_interval(-1.0, 2.0, -10.0, 10.0) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-1.5, 2.0, -10.0, 10.0) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-2.0, 2.0, -10.0, 10.0) == libv::Approx(-2.0));
}

TEST_CASE("snap_interval with value outside interval", "[libv.math.snap_interval]") {
	CHECK(libv::math::snap_interval(0.0, 1.0, -10.0, -0.5) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(0.0, 1.0, 0.5, 10.0) == libv::Approx(0.5));

	CHECK(libv::math::snap_interval(1.0, 1.0, -10.0, -0.5) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-1.0, 1.0, 0.5, 10.0) == libv::Approx(0.5));

	CHECK(libv::math::snap_interval(-11.0, 1.0, -10.0, -0.5) == libv::Approx(-10.0));
	CHECK(libv::math::snap_interval(11.0, 1.0, 0.5, 10.0) == libv::Approx(10.0));

	CHECK(libv::math::snap_interval(-11.0, 1.0, -10.0, -0.5) == libv::Approx(-10.0));
	CHECK(libv::math::snap_interval(11.0, 1.0, 0.5, 10.0) == libv::Approx(10.0));
}

TEST_CASE("snap_interval with value on rounding edge", "[libv.math.snap_interval]") {
	CHECK(libv::math::snap_interval(+0.749999, 1.0, +0.5, +10.5) == libv::Approx(+0.5));
	CHECK(libv::math::snap_interval(+0.750000, 1.0, +0.5, +10.5) == libv::Approx(+1.0));
	CHECK(libv::math::snap_interval(+0.750001, 1.0, +0.5, +10.5) == libv::Approx(+1.0));
	CHECK(libv::math::snap_interval(-0.749999, 1.0, -10.5, -0.5) == libv::Approx(-0.5));
	// Not sure on this rounding, it rounds upward instead of away from zero or toward borders:
	CHECK(libv::math::snap_interval(-0.750000, 1.0, -10.5, -0.5) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-0.750001, 1.0, -10.5, -0.5) == libv::Approx(-1.0));

	CHECK(libv::math::snap_interval(+10.249999, 1.0, +0.5, +10.5) == libv::Approx(+10.0));
	CHECK(libv::math::snap_interval(+10.250000, 1.0, +0.5, +10.5) == libv::Approx(+10.5));
	CHECK(libv::math::snap_interval(+10.250001, 1.0, +0.5, +10.5) == libv::Approx(+10.5));
	CHECK(libv::math::snap_interval(-10.249999, 1.0, -10.5, -0.5) == libv::Approx(-10.0));
	// Not sure on this rounding, it rounds upward instead of away from zero or toward borders:
	CHECK(libv::math::snap_interval(-10.250000, 1.0, -10.5, -0.5) == libv::Approx(-10.0));
	CHECK(libv::math::snap_interval(-10.250001, 1.0, -10.5, -0.5) == libv::Approx(-10.5));

	CHECK(libv::math::snap_interval(+4.499999, 1.0, +2.0, +7.0) == libv::Approx(+4.0));
	CHECK(libv::math::snap_interval(+4.500000, 1.0, +2.0, +7.0) == libv::Approx(+5.0));
	CHECK(libv::math::snap_interval(+4.500001, 1.0, +2.0, +7.0) == libv::Approx(+5.0));
	CHECK(libv::math::snap_interval(-4.499999, 1.0, -2.0, -7.0) == libv::Approx(-4.0));
	CHECK(libv::math::snap_interval(-4.500000, 1.0, -2.0, -7.0) == libv::Approx(-5.0));
	CHECK(libv::math::snap_interval(-4.500001, 1.0, -2.0, -7.0) == libv::Approx(-5.0));
}

TEST_CASE("snap_interval scan test", "[libv.math.snap_interval]") {
	CHECK(libv::math::snap_interval(-1., 2.0, 0.5, 5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(-.5, 2.0, 0.5, 5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(-.2, 2.0, 0.5, 5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(0.0, 2.0, 0.5, 5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(0.5, 2.0, 0.5, 5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(0.9, 2.0, 0.5, 5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(1.0, 2.0, 0.5, 5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(1.1, 2.0, 0.5, 5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(1.5, 2.0, 0.5, 5.2) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(1.9, 2.0, 0.5, 5.2) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(2.0, 2.0, 0.5, 5.2) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(3.0, 2.0, 0.5, 5.2) == libv::Approx(4.0));
	CHECK(libv::math::snap_interval(4.0, 2.0, 0.5, 5.2) == libv::Approx(4.0));
	CHECK(libv::math::snap_interval(4.5, 2.0, 0.5, 5.2) == libv::Approx(4.0));
	CHECK(libv::math::snap_interval(4.6, 2.0, 0.5, 5.2) == libv::Approx(4.0));
	CHECK(libv::math::snap_interval(4.7, 2.0, 0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(4.8, 2.0, 0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(4.9, 2.0, 0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(5.0, 2.0, 0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(5.1, 2.0, 0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(5.2, 2.0, 0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(5.3, 2.0, 0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(6.0, 2.0, 0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(7.0, 2.0, 0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(8.0, 2.0, 0.5, 5.2) == libv::Approx(5.2));

	CHECK(libv::math::snap_interval(-1., 2.0, -0.5, 5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-.5, 2.0, -0.5, 5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-.2, 2.0, -0.5, 5.2) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(0.0, 2.0, -0.5, 5.2) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(0.5, 2.0, -0.5, 5.2) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(0.9, 2.0, -0.5, 5.2) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(1.0, 2.0, -0.5, 5.2) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(1.1, 2.0, -0.5, 5.2) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(1.5, 2.0, -0.5, 5.2) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(1.9, 2.0, -0.5, 5.2) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(2.0, 2.0, -0.5, 5.2) == libv::Approx(2.0));
	CHECK(libv::math::snap_interval(3.0, 2.0, -0.5, 5.2) == libv::Approx(4.0));
	CHECK(libv::math::snap_interval(4.0, 2.0, -0.5, 5.2) == libv::Approx(4.0));
	CHECK(libv::math::snap_interval(4.5, 2.0, -0.5, 5.2) == libv::Approx(4.0));
	CHECK(libv::math::snap_interval(4.6, 2.0, -0.5, 5.2) == libv::Approx(4.0));
	CHECK(libv::math::snap_interval(4.7, 2.0, -0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(4.8, 2.0, -0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(4.9, 2.0, -0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(5.0, 2.0, -0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(5.1, 2.0, -0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(5.2, 2.0, -0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(5.3, 2.0, -0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(6.0, 2.0, -0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(7.0, 2.0, -0.5, 5.2) == libv::Approx(5.2));
	CHECK(libv::math::snap_interval(8.0, 2.0, -0.5, 5.2) == libv::Approx(5.2));

	CHECK(libv::math::snap_interval(-1., 2.0, 0.5, -5.2) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-.5, 2.0, 0.5, -5.2) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(-.2, 2.0, 0.5, -5.2) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(0.0, 2.0, 0.5, -5.2) == libv::Approx(0.0));
	CHECK(libv::math::snap_interval(0.5, 2.0, 0.5, -5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(0.9, 2.0, 0.5, -5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(1.0, 2.0, 0.5, -5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(1.1, 2.0, 0.5, -5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(1.5, 2.0, 0.5, -5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(1.9, 2.0, 0.5, -5.2) == libv::Approx(0.5));
	CHECK(libv::math::snap_interval(2.0, 2.0, 0.5, -5.2) == libv::Approx(0.5));

	CHECK(libv::math::snap_interval(-2., 2.0, -0.5, -5.2) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-1., 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-.5, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-.2, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(0.0, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(0.5, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(0.9, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(1.0, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(1.1, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(1.5, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(1.9, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(2.0, 2.0, -0.5, -5.2) == libv::Approx(-0.5));

	CHECK(libv::math::snap_interval(+1.0, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(+0.5, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(+0.2, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-0.0, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-0.5, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-0.9, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-1.0, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-1.1, 2.0, -0.5, -5.2) == libv::Approx(-0.5));
	CHECK(libv::math::snap_interval(-1.5, 2.0, -0.5, -5.2) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-1.9, 2.0, -0.5, -5.2) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-2.0, 2.0, -0.5, -5.2) == libv::Approx(-2.0));
	CHECK(libv::math::snap_interval(-3.0, 2.0, -0.5, -5.2) == libv::Approx(-4.0));
	CHECK(libv::math::snap_interval(-4.0, 2.0, -0.5, -5.2) == libv::Approx(-4.0));
	CHECK(libv::math::snap_interval(-4.5, 2.0, -0.5, -5.2) == libv::Approx(-4.0));
	CHECK(libv::math::snap_interval(-4.6, 2.0, -0.5, -5.2) == libv::Approx(-4.0));
	CHECK(libv::math::snap_interval(-4.7, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
	CHECK(libv::math::snap_interval(-4.8, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
	CHECK(libv::math::snap_interval(-4.9, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
	CHECK(libv::math::snap_interval(-5.0, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
	CHECK(libv::math::snap_interval(-5.1, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
	CHECK(libv::math::snap_interval(-5.2, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
	CHECK(libv::math::snap_interval(-5.3, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
	CHECK(libv::math::snap_interval(-6.0, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
	CHECK(libv::math::snap_interval(-7.0, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
	CHECK(libv::math::snap_interval(-8.0, 2.0, -0.5, -5.2) == libv::Approx(-5.2));
}
