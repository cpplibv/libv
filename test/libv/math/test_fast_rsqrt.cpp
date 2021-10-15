// Project: libv.math, File: test/libv/math/test_fast_rsqrt.cpp


// hpp
#include <catch/catch.hpp>
// std
#include <cmath>
// pro
#include <libv/math/fast_rsqrt.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("fast_rsqrt error rate", "[libv.math.fast_rsqrt]") {
	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 0.0001f;
		CHECK(std::abs(1.0f / std::sqrt(v) - libv::math::fast_rsqrt(v)) < std::max(0.02f, v * 0.0018f));
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 0.001f;
		CHECK(std::abs(1.0f / std::sqrt(v) - libv::math::fast_rsqrt(v)) < std::max(0.006f, v * 0.0018f));
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 0.01f;

		CHECK(std::abs(1.0f / std::sqrt(v) - libv::math::fast_rsqrt(v)) < v * 0.0018f);
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 0.1f;

		CHECK(std::abs(1.0f / std::sqrt(v) - libv::math::fast_rsqrt(v)) < v * 0.00018f);
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 1.f;

		CHECK(std::abs(1.0f / std::sqrt(v) - libv::math::fast_rsqrt(v)) < v * 0.000018f);
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 10.f;

		CHECK(std::abs(1.0f / std::sqrt(v) - libv::math::fast_rsqrt(v)) < v * 0.0000018f);
	}
}
