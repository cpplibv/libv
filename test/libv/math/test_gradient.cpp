// Project: libv.math, File: test/libv/math/test_quat.cpp

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/utility/approx.hpp>
// pro
#include <libv/math/gradient.hpp>
#include <libv/math/vec.hpp>


// -------------------------------------------------------------------------------------------------

using approx_f = libv::approx<float, -4>;
using approx_vec3 = libv::vec3_t<approx_f>;

// -------------------------------------------------------------------------------------------------

TEST_CASE("gradient simple float float", "[libv.math.gradient]") {
	libv::gradient<float, float> gradient{};

	gradient.add(0.f, 0.f);
	gradient.add(1.f, 1.f);

	CHECK(gradient.min_key() == 0.f);
	CHECK(gradient.max_key() == 1.f);

	CHECK(gradient.sample(-1.0f) == 0.0f);
	CHECK(gradient.sample(0.0f) == 0.0f);
	CHECK(gradient.sample(0.5f) == 0.5f);
	CHECK(gradient.sample(1.0f) == 1.0f);
	CHECK(gradient.sample(2.0f) == 1.0f);
}

TEST_CASE("gradient simple float vec3", "[libv.math.gradient]") {
	libv::gradient<float, libv::vec3f> gradient{};

	gradient.add(2.f, {4, 40, 400});
	gradient.add(3.f, {6, 60, 600});

	CHECK(gradient.min_key() == 2.f);
	CHECK(gradient.max_key() == 3.f);

	CHECK(gradient.sample(-1.0f) == approx_vec3{4.0f, 40.0f, 400.0f});
	CHECK(gradient.sample(0.0f) == approx_vec3{4.0f, 40.0f, 400.0f});
	CHECK(gradient.sample(2.0f) == approx_vec3{4.0f, 40.0f, 400.0f});
	CHECK(gradient.sample(2.1f) == approx_vec3{4.2f, 42.0f, 420.0f});
	CHECK(gradient.sample(2.5f) == approx_vec3{5.0f, 50.0f, 500.0f});
	CHECK(gradient.sample(3.0f) == approx_vec3{6.0f, 60.0f, 600.0f});
	CHECK(gradient.sample(4.0f) == approx_vec3{6.0f, 60.0f, 600.0f});
}

TEST_CASE("gradient matching keys to introduce discontinuity", "[libv.math.gradient]") {
	libv::gradient<float> gradient{};

	gradient.add(0.f, 0.f);
	gradient.add(1.f, 0.f);
	gradient.add(1.f, 1.f);
	gradient.add(2.f, 1.f);

	CHECK(gradient.sample(0.0f) == 0.0f);
	CHECK(gradient.sample(0.9999f) == 0.0f);
	CHECK(gradient.sample(1.0f) == 1.0f);
	CHECK(gradient.sample(1.0001f) == 1.0f);
	CHECK(gradient.sample(2.0f) == 1.0f);
}
