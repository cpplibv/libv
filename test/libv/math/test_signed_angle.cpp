// Project: libv.math, File: test/libv/math/test_signed_angle.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// libv
#include <libv/utility/approx.hpp>
// pro
#include <libv/math/constants.hpp>
#include <libv/math/signed_angle.hpp>
#include <libv/math/vec.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("signed_angle", "[libv.math.signed_angle]") {
	const auto pi = libv::pi;

	CHECK(libv::math::signed_angle<float>({1, 0, 0}, {1, 0, 0}) == libv::Approx(0 * pi));
	CHECK(libv::math::signed_angle<float>({2, 0, 0}, {2, 0, 0}) == libv::Approx(0 * pi));
	CHECK(libv::math::signed_angle<float>({2, 2, 0}, {-2, 2, 0}) == libv::Approx(0.5f * pi));
	CHECK(libv::math::signed_angle<float>({-2, 2, 0}, {2, 2, 0}) == libv::Approx(-0.5f * pi));
	CHECK(libv::math::signed_angle<float>({-1, 0, 0}, {1, 0, 0}) == libv::Approx(1 * pi));
	CHECK(libv::math::signed_angle<float>({-2, 0, 0}, {2, 0, 0}) == libv::Approx(1 * pi));
	CHECK(libv::math::signed_angle<float>({-2, -2, 0}, {2, 2, 0}) == libv::Approx<float, -3>(1 * pi));
}

TEST_CASE("signed_angle 2D", "[libv.math.signed_angle]") {
	const auto pi = libv::pi;

	CHECK(libv::math::signed_angle<float>({1, 0}, {1, 0}) == libv::Approx(0 * pi));
	CHECK(libv::math::signed_angle<float>({2, 0}, {2, 0}) == libv::Approx(0 * pi));
	CHECK(libv::math::signed_angle<float>({2, 2}, {-2, 2}) == libv::Approx(0.5f * pi));
	CHECK(libv::math::signed_angle<float>({-2, 2}, {2, 2}) == libv::Approx(-0.5f * pi));
	CHECK(libv::math::signed_angle<float>({-1, 0}, {1, 0}) == libv::Approx(1 * pi));
	CHECK(libv::math::signed_angle<float>({-2, 0}, {2, 0}) == libv::Approx(1 * pi));
	CHECK(libv::math::signed_angle<float>({-2, -2}, {2, 2}) == libv::Approx<float, -3>(1 * pi));
}
