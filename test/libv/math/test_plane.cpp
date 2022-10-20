// Project: libv.math, File: test/libv/math/test_quat.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// libv
#include <libv/utility/approx.hpp>
// std
#include <sstream>
// pro
#include <libv/math/plane.hpp>


// -------------------------------------------------------------------------------------------------

using vec3 = libv::vec3f;
using plane = libv::planef;

using approx = libv::plane_t<libv::approx<float, -4>>;
using approx_f = libv::approx<float, -4>;
using approx_vec3 = libv::vec3_t<libv::approx<float, -4>>;
using approx_vec4 = libv::vec4_t<libv::approx<float, -4>>;

namespace libv {
template <typename K>
[[nodiscard]] constexpr inline bool operator==(const ::approx& lhs, const libv::plane_t<K>& rhs) noexcept {
	return lhs.normal == rhs.normal && lhs.dist == rhs.dist;
}
} // namespace libv

[[nodiscard]] constexpr inline approx make_approx(float nx, float ny, float nz, float dist) noexcept {
	return approx::from_normal_dist(nx, ny, nz, dist);
}

[[nodiscard]] constexpr inline approx make_approx_v(float vx, float vy, float vz, float dist) noexcept {
	return approx::from_normal_dist(approx_vec3(vec3(vx, vy, vz).normalize()), dist);
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Test plane constructors", "[libv.math.plane]") {
	CHECK(plane().normal == vec3(0, 0, 1));
	CHECK(plane().dist == 0);

	CHECK(plane(1, 2, 3, 4).normal == approx_vec3(0.267261f, 0.534522f, 0.801784f));
	CHECK(plane(1, 2, 3, 4).dist == 4);

	CHECK(plane(vec3(1, 2, 3), 4).normal == approx_vec3(0.267261f, 0.534522f, 0.801784f));
	CHECK(plane(vec3(1, 2, 3), 4).dist == 4);

	CHECK(plane::from_three_point({0, 0, 0}, {1, 0, 0}, {0, 1, 0}) == make_approx(0, 0, 1, 0));
	CHECK(plane::from_three_point({0, 0, 0}, {0, 1, 0}, {1, 0, 0}) == make_approx(0, 0, -1, 0));
	CHECK(plane::from_three_point({10, 0, 0}, {11, 0, 0}, {10, 1, 0}) == make_approx(0, 0, 1, 0));
	CHECK(plane::from_three_point({10, 0, 0}, {10, 1, 0}, {11, 0, 0}) == make_approx(0, 0, -1, 0));
	CHECK(plane::from_three_point({1, 2, 3}, {4, 5, 6}, {8, 9, 0}) == make_approx(-0.707107f, 0.707107f, 0, -0.707107f));

	CHECK(plane::from_vector_dist({1, 2, 3}, 3) == make_approx(0.267261f, 0.534522f, 0.801784f, 3));
	CHECK(plane::from_vector_dist({1, 2, 3}, -3) == make_approx(0.267261f, 0.534522f, 0.801784f, -3));
	CHECK(plane::from_vector_dist({1, 0, 0}, 3) == make_approx(1, 0, 0, 3));
	CHECK(plane::from_vector_dist({1, 0, 0}, -3) == make_approx(1, 0, 0, -3));
	CHECK(plane::from_vector_dist({-1, 0, 0}, 3) == make_approx(-1, 0, 0, 3));
	CHECK(plane::from_vector_dist({-1, 0, 0}, -3) == make_approx(-1, 0, 0, -3));

	CHECK(plane::from_vector_point({1, 2, 3}, {1, 2, 3}) == make_approx_v(1, 2, 3, -3.74166f));
	CHECK(plane::from_vector_point({1, 0, 0}, {1, 2, 3}) == make_approx_v(1, 0, 0, -1));
	CHECK(plane::from_vector_point({-1, 0, 0}, {1, 2, 3}) == make_approx_v(-1, 0, 0, 1));
	CHECK(plane::from_vector_point({0, 1, 0}, {1, 2, 3}) == make_approx_v(0, 1, 0, -2));
	CHECK(plane::from_vector_point({0, -1, 0}, {1, 2, 3}) == make_approx_v(0, -1, 0, 2));
	CHECK(plane::from_vector_point({0, 0, 1}, {1, 2, 3}) == make_approx_v(0, 0, 1, -3));
	CHECK(plane::from_vector_point({0, 0, -1}, {1, 2, 3}) == make_approx_v(0, 0, -1, 3));

	CHECK(plane::from_vector_point({2, 2, 8}, {5, 5, 5}) == make_approx_v(0.235702f, 0.235702f, 0.942809f, -7.07107f));
}

TEST_CASE("Test plane distance_from_point", "[libv.math.plane]") {
	CHECK(plane::from_vector_dist({1, 0, 0}, 0).distance_from_point(5, 0, 0) == approx_f(5));
	CHECK(plane::from_vector_dist({1, 0, 0}, 0).distance_from_point(-5, 0, 0) == approx_f(-5));
	CHECK(plane::from_vector_dist({1, 0, 0}, 2).distance_from_point(5, 0, 0) == approx_f(7));
	CHECK(plane::from_vector_dist({1, 0, 0}, 2).distance_from_point(-5, 0, 0) == approx_f(-3));
	CHECK(plane::from_vector_dist({1, 0, 0}, -2).distance_from_point(5, 0, 0) == approx_f(3));
	CHECK(plane::from_vector_dist({1, 0, 0}, -2).distance_from_point(-5, 0, 0) == approx_f(-7));

	CHECK(plane::from_vector_dist({1, 2, 3}, -3).distance_from_point(0, 0, 0) == approx_f(-3));
	CHECK(plane::from_vector_dist({1, 2, 3}, 3).distance_from_point(0, 0, 0) == approx_f(3));

	CHECK(plane::from_vector_dist({-1, -2, -3}, -3).distance_from_point(0, 0, 0) == approx_f(-3));
	CHECK(plane::from_vector_dist({-1, -2, -3}, 3).distance_from_point(0, 0, 0) == approx_f(3));

	CHECK(plane::from_vector_dist({5, 3, 8}, 2).distance_from_point(9, 7, 1) == approx_f(9.47513f));
	CHECK(plane::from_vector_dist({5, -3, 8}, -2).distance_from_point(-9, 7, 1) == approx_f(-7.85888f));

	CHECK(plane::from_vector_point({2, 2, 8}, {5, 5, 5}).distance_from_point(1, 1, 15) == approx_f(7.54247f));
}

TEST_CASE("Test plane ostream", "[libv.math.plane]") {
	std::stringstream ss;
	ss.str(""); ss << plane(1, 0, 0, 5); CHECK(ss.str() == "N:1 0 0 D:5");
	ss.str(""); ss << plane(1, 2, 3, 5); CHECK(ss.str() == "N:0.267261 0.534522 0.801784 D:5");
}

//TEST_CASE("Test plane formatter", "[libv.math.plane]") {
//	libv::quatf v0(0, 42.0f, libv::pi, -0.3f);
//	libv::quatf v1(0, 1, 2, 3);
//
//	CHECK(fmt::format("{}", v1) == "0 1 2 3");
//	CHECK(fmt::format("{}", v0) == "0 42 3.1415927 -0.3");
//	CHECK(fmt::format("{:}", v0) == "0423.1415927-0.3");
//	CHECK(fmt::format("{: }", v0) == "0 42 3.1415927 -0.3");
//	CHECK(fmt::format("{::}", v0) == "0423.1415927-0.3");
//	CHECK(fmt::format("{: :}", v0) == "0 42 3.1415927 -0.3");
//
//	CHECK(fmt::format("{:, }", v0) == "0, 42, 3.1415927, -0.3");
//
//	CHECK(fmt::format("{::07.3f}", v0) == "000.000042.000003.142-00.300");
//	CHECK(fmt::format("{::<6}", v0) == "0     42    3.1415927-0.3  ");
//	CHECK(fmt::format("{::^6}", v0) == "  0     42  3.1415927 -0.3 ");
//	CHECK(fmt::format("{::>6}", v0) == "     0    423.1415927  -0.3");
//
//	CHECK(fmt::format("{: :07.3f}", v0) == "000.000 042.000 003.142 -00.300");
//	CHECK(fmt::format("{: :<6}", v0) == "0      42     3.1415927 -0.3  ");
//	CHECK(fmt::format("{: :^6}", v0) == "  0      42   3.1415927  -0.3 ");
//	CHECK(fmt::format("{: :>6}", v0) == "     0     42 3.1415927   -0.3");
//
//	CHECK(fmt::format("{:, :07.3f}", v0) == "000.000, 042.000, 003.142, -00.300");
//	CHECK(fmt::format("{:, :<6}", v0) == "0     , 42    , 3.1415927, -0.3  ");
//	CHECK(fmt::format("{:, :^6}", v0) == "  0   ,   42  , 3.1415927,  -0.3 ");
//	CHECK(fmt::format("{:, :>6}", v0) == "     0,     42, 3.1415927,   -0.3");
//}
