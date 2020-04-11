// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader


// hpp
#include <catch/catch.hpp>
// std
#include <cmath>
// pro
#include <libv/math/fast_sqrt.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("fast_sqrt error rate", "[libv.math.fast_sqrt]") {
	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 0.0001f;
		CHECK(std::abs(std::sqrt(v) - libv::math::fast_sqrt(v)) < std::max(0.04f, v * 0.018f));
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 0.001f;
		CHECK(std::abs(std::sqrt(v) - libv::math::fast_sqrt(v)) < std::max(0.012f, v * 0.018f));
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 0.01f;

		CHECK(std::abs(std::sqrt(v) - libv::math::fast_sqrt(v)) < v * 0.018f);
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 0.1f;

		CHECK(std::abs(std::sqrt(v) - libv::math::fast_sqrt(v)) < v * 0.0018f);
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 1.f;

		CHECK(std::abs(std::sqrt(v) - libv::math::fast_sqrt(v)) < v * 0.00018f);
	}

	for (int i = 100; i < 1000; ++i) {
		const auto v = static_cast<float>(i) * 10.f;

		CHECK(std::abs(std::sqrt(v) - libv::math::fast_sqrt(v)) < v * 0.00006f);
	}
}
