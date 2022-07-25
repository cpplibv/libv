// Project: libv.math, File: test/libv/math/test_quat.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/utility/approx.hpp>
// pro
#include <libv/math/constants.hpp>
#include <libv/math/quat.hpp>


// -------------------------------------------------------------------------------------------------


using approx = libv::quat_t<libv::approx<float, -4>>;
using approx_f = libv::approx<float, -4>;
using approx_vec3 = libv::vec3_t<libv::approx<float, -4>>;
using approx_vec4 = libv::vec4_t<libv::approx<float, -4>>;

namespace libv {
template <typename K>
[[nodiscard]] constexpr inline bool operator==(const ::approx& lhs, const libv::quat_t<K>& rhs) noexcept {
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}
} // namespace libv

template <typename T>
[[nodiscard]] constexpr inline approx make_approx(T quat) noexcept {
	return approx(quat.w, quat.x, quat.y, quat.z);
}


// -------------------------------------------------------------------------------------------------

TEST_CASE("quat constructors", "[libv.math.quat]") {
	libv::quatf quat{0, 1, 2, 3};

	CHECK(0.f == quat.w);
	CHECK(1.f == quat.x);
	CHECK(2.f == quat.y);
	CHECK(3.f == quat.z);

	CHECK((4 * sizeof (float)) == sizeof (libv::quatf));
	CHECK((4 * sizeof (double)) == sizeof (libv::quatd));

	CHECK(approx{1, 0, 0, 0} == libv::quatf());
	CHECK(approx{0, 1, 0, 0} == libv::quatf::create_rotate(libv::pi, libv::vec3f(1, 0, 0)));
	CHECK(approx{0, 0, 1, 0} == libv::quatf::create_rotate(libv::pi, libv::vec3f(0, 1, 0)));
	CHECK(approx{0, 0, 0, 1} == libv::quatf::create_rotate(libv::pi, libv::vec3f(0, 0, 1)));
}

TEST_CASE("quat mix", "[libv.math.quat]") {
	CHECK(approx{1.0f   , 0.0f   , 0, 0} == libv::mix(libv::quatf(1, 0, 0, 0), libv::quatf(0, 1, 0, 0), 0.f));
	CHECK(approx{0.0f   , 1.0f   , 0, 0} == libv::mix(libv::quatf(1, 0, 0, 0), libv::quatf(0, 1, 0, 0), 1.f));
	CHECK(approx{0.8910f, 0.4539f, 0, 0} == libv::mix(libv::quatf(1, 0, 0, 0), libv::quatf(0, 1, 0, 0), 0.3f));
	CHECK(approx{0.7071f, 0.7071f, 0, 0} == libv::mix(libv::quatf(1, 0, 0, 0), libv::quatf(0, 1, 0, 0), 0.5f));
	CHECK(approx{0.1564f, 0.9876f, 0, 0} == libv::mix(libv::quatf(1, 0, 0, 0), libv::quatf(0, 1, 0, 0), 0.9f));
}

TEST_CASE("quat inverse", "[libv.math.quat]") {
	CHECK(approx{1.0f, 0.0f, 0, 0} == libv::quatf(1, 0, 0, 0).inverse());
	CHECK(approx{0.0f, -1.0f, 0, 0} == libv::quatf(0, 1, 0, 0).inverse());
	CHECK(approx{0.7071f, -0.7071f, 0, 0} == libv::quatf(0.7071f, 0.7071f, 0, 0).inverse());
	CHECK(approx{0.353553f, -0.353553f, -0.5f, 0} == libv::quatf(0.7071f, 0.7071f, 1, 0).inverse());

	CHECK(approx{1.0f, 0.0f, 0, 0} == libv::quatf(1, 0, 0, 0).inverse_copy());
	CHECK(approx{0.0f, -1.0f, 0, 0} == libv::quatf(0, 1, 0, 0).inverse_copy());
	CHECK(approx{0.7071f, -0.7071f, 0, 0} == libv::quatf(0.7071f, 0.7071f, 0, 0).inverse_copy());
	CHECK(approx{0.353553f, -0.353553f, -0.5f, 0} == libv::quatf(0.7071f, 0.7071f, 1, 0).inverse_copy());
}

TEST_CASE("quat to_matrix", "[libv.math.quat]") {
	CHECK(libv::mat3f::identity() == libv::to_mat3(libv::quatf(1, 0, 0, 0)));
	CHECK(libv::mat4f::identity() == libv::to_mat4(libv::quatf(1, 0, 0, 0)));

	const auto m0 = libv::to_mat3(libv::quatf(0.7071f, 0.7071f, 0, 0));
	CHECK(approx_vec3(1, 0, 0) == m0.at_v(0));
	CHECK(approx_vec3(0, 0, 1) == m0.at_v(1));
	CHECK(approx_vec3(0, -1, 0) == m0.at_v(2));

	const auto m1 = libv::to_mat4(libv::quatf(0.7071f, 0.7071f, 0, 0));
	CHECK(approx_vec4(1, 0, 0, 0) == m1.at_v(0));
	CHECK(approx_vec4(0, 0, 1, 0) == m1.at_v(1));
	CHECK(approx_vec4(0, -1, 0, 0) == m1.at_v(2));
	CHECK(approx_vec4(0, 0, 0, 1) == m1.at_v(3));

	const auto m2 = libv::to_mat4(libv::quatf(0.7071f, 0.7071f, 1.f, 0));
	CHECK(approx_vec4(-1, 1.4142f, -1.4142f, 0) == m2.at_v(0));
	CHECK(approx_vec4(1.4142f, 0, 1, 0) == m2.at_v(1));
	CHECK(approx_vec4(1.4142f, -1, -2, 0) == m2.at_v(2));
	CHECK(approx_vec4(0, 0, 0, 1) == m2.at_v(3));
}

TEST_CASE("quat cross", "[libv.math.quat]") {
	CHECK(approx{0, 1, 0, 0} == libv::cross(libv::quatf(1, 0, 0, 0), libv::quatf(0, 1, 0, 0)));
	CHECK(approx{0, 0, 0, 1} == libv::cross(libv::quatf(0, 1, 0, 0), libv::quatf(0, 0, 1, 0)));
	CHECK(approx{0, 0, 0, -1} == libv::cross(libv::quatf(0, 0, 1, 0), libv::quatf(0, 1, 0, 0)));

	CHECK(approx{-0.7071f, 0.7071f, 0, -1} == libv::cross(libv::quatf(0.7071f, 0.7071f, 1.f, 0), libv::quatf(0, 1, 0, 0)));
}

TEST_CASE("quat normalize", "[libv.math.quat]") {
	CHECK(approx{1, 0, 0, 0} == libv::quatf(1, 0, 0, 0).normalize());
	CHECK(approx{1, 0, 0, 0} == libv::quatf(2, 0, 0, 0).normalize());
	CHECK(approx{0, 1, 0, 0} == libv::quatf(0, 1, 0, 0).normalize());
	CHECK(approx{0, 0, -1, 0} == libv::quatf(0, 0, -1, 0).normalize());

	CHECK(approx{0.7071f, 0, 0.7071f, 0} == libv::quatf(1, 0, 1, 0).normalize());
	CHECK(approx{0.5f, 0.5f, 0.7071f, 0} == libv::quatf(0.7071f, 0.7071f, 1.f, 0).normalize());

	CHECK(approx{1, 0, 0, 0} == libv::quatf(1, 0, 0, 0).normalize_copy());
	CHECK(approx{1, 0, 0, 0} == libv::quatf(2, 0, 0, 0).normalize_copy());
	CHECK(approx{0, 1, 0, 0} == libv::quatf(0, 1, 0, 0).normalize_copy());
	CHECK(approx{0, 0, -1, 0} == libv::quatf(0, 0, -1, 0).normalize_copy());

	CHECK(approx{0.7071f, 0, 0.7071f, 0} == libv::quatf(1, 0, 1, 0).normalize_copy());
	CHECK(approx{0.5f, 0.5f, 0.7071f, 0} == libv::quatf(0.7071f, 0.7071f, 1.f, 0).normalize_copy());
}

TEST_CASE("quat mul", "[libv.math.quat]") {
	CHECK(approx_vec3{1, 0, 0} == libv::quatf(1, 0, 0, 0) * libv::vec3f(1, 0, 0));
	CHECK(approx_vec3{0, 1, 0} == libv::quatf(1, 0, 0, 0) * libv::vec3f(0, 1, 0));
	CHECK(approx_vec3{0, 0, 1} == libv::quatf(1, 0, 0, 0) * libv::vec3f(0, 0, 1));

	CHECK(approx_vec4{1, 0, 0, 0} == libv::quatf(1, 0, 0, 0) * libv::vec4f(1, 0, 0, 0));
	CHECK(approx_vec4{0, 1, 0, 0} == libv::quatf(1, 0, 0, 0) * libv::vec4f(0, 1, 0, 0));
	CHECK(approx_vec4{0, 0, 1, 0} == libv::quatf(1, 0, 0, 0) * libv::vec4f(0, 0, 1, 0));

	CHECK(approx_vec4{1, 0, 0, 1} == libv::quatf(1, 0, 0, 0) * libv::vec4f(1, 0, 0, 1));
	CHECK(approx_vec4{0, 1, 0, 1} == libv::quatf(1, 0, 0, 0) * libv::vec4f(0, 1, 0, 1));
	CHECK(approx_vec4{0, 0, 1, 1} == libv::quatf(1, 0, 0, 0) * libv::vec4f(0, 0, 1, 1));

	const auto angle_axis = [](float angle, libv::vec3f axis) {
		return libv::quatf::angle_axis(angle, axis);
	};

	CHECK(approx_vec3{0, 1, 0} == angle_axis(libv::pi / 2.f, {0, 0, 1}) * libv::vec3f(1, 0, 0));
	CHECK(approx_vec3{-1, 0, 0} == angle_axis(libv::pi / 2.f, {0, 0, 1}) * libv::vec3f(0, 1, 0));
	CHECK(approx_vec3{0, 0, 1} == angle_axis(libv::pi / 2.f, {0, 0, 1}) * libv::vec3f(0, 0, 1));

	CHECK(approx_vec4{0, 1, 0, 0} == angle_axis(libv::pi / 2.f, {0, 0, 1}) * libv::vec4f(1, 0, 0, 0));
	CHECK(approx_vec4{-1, 0, 0, 0} == angle_axis(libv::pi / 2.f, {0, 0, 1}) * libv::vec4f(0, 1, 0, 0));
	CHECK(approx_vec4{0, 0, 1, 0} == angle_axis(libv::pi / 2.f, {0, 0, 1}) * libv::vec4f(0, 0, 1, 0));

	CHECK(approx_vec4{0, 1, 0, 1} == angle_axis(libv::pi / 2.f, {0, 0, 1}) * libv::vec4f(1, 0, 0, 1));
	CHECK(approx_vec4{-1, 0, 0, 1} == angle_axis(libv::pi / 2.f, {0, 0, 1}) * libv::vec4f(0, 1, 0, 1));
	CHECK(approx_vec4{0, 0, 1, 1} == angle_axis(libv::pi / 2.f, {0, 0, 1}) * libv::vec4f(0, 0, 1, 1));
}

TEST_CASE("quat from mat", "[libv.math.quat]") {
	CHECK(approx{1, 0, 0, 0} == libv::quatf(libv::quatf(1, 0, 0, 0).to_mat3()));
	CHECK(approx{0, 1, 0, 0} == libv::quatf(libv::quatf(0, 1, 0, 0).to_mat3()));
	CHECK(approx{0, 0, 1, 0} == libv::quatf(libv::quatf(0, 0, 1, 0).to_mat3()));
	CHECK(approx{0, 0, 0, 1} == libv::quatf(libv::quatf(0, 0, 0, 1).to_mat3()));

	CHECK(approx{1, 0, 0, 0} == libv::quatf(libv::quatf(1, 0, 0, 0).to_mat4()));
	CHECK(approx{0, 1, 0, 0} == libv::quatf(libv::quatf(0, 1, 0, 0).to_mat4()));
	CHECK(approx{0, 0, 1, 0} == libv::quatf(libv::quatf(0, 0, 1, 0).to_mat4()));
	CHECK(approx{0, 0, 0, 1} == libv::quatf(libv::quatf(0, 0, 0, 1).to_mat4()));

	CHECK(approx{0.7071f, 0.7071f, 0, 0} == libv::quatf(libv::quatf(0.7071f, 0.7071f, 0, 0).to_mat3()));
	CHECK(approx{0.7071f, 0.7071f, 0, 0} == libv::quatf(libv::quatf(0.7071f, 0.7071f, 0, 0).to_mat3()));
}

TEST_CASE("quat look_at", "[libv.math.quat]") {
	CHECK(approx{1, 0, 0, 0} == libv::quatf::look_at(libv::vec3f(1, 0, 0), libv::vec3f(0, 0, 1)));
	CHECK(approx{0.707107f, -0.707107f, 0, 0} == libv::quatf::look_at(libv::vec3f(1, 0, 0), libv::vec3f(0, 1, 0)));
	CHECK(approx{0.707107f, 0, 0, 0} == libv::quatf::look_at(libv::vec3f(1, 0, 0), libv::vec3f(1, 0, 0))); // Look at up error

	CHECK(approx{0.707107f, 0, 0, 0.707107f} == libv::quatf::look_at(libv::vec3f(0, 1, 0), libv::vec3f(0, 0, 1)));
	CHECK(approx{0.5f, 0, -0.5f, 0} == libv::quatf::look_at(libv::vec3f(0, 0, 1), libv::vec3f(0, 0, 1))); // Look at up error

	CHECK(approx{0.9238795f, 0, 0, 0.3826834f} == libv::quatf::look_at(libv::vec3f(0.707107f, 0.707107f, 0), libv::vec3f(0, 0, 1)));
	CHECK(approx{0.880476f, 0.115917f, -0.279848f, 0.364705f} == libv::quatf::look_at(libv::vec3f(0.57735f, 0.57735f, 0.57735f), libv::vec3f(0, 0, 1)));

	const auto look_at = [](libv::vec3f dir, libv::vec3f up) {
		return libv::quatf::look_at(dir, up);
	};

	CHECK(approx_vec3{1, 0, 0} == look_at({1, 0, 0}, {0, 0, 1}) * libv::vec3f(1, 0, 0));
	CHECK(approx_vec3{0, 1, 0} == look_at({1, 0, 0}, {0, 0, 1}) * libv::vec3f(0, 1, 0));
	CHECK(approx_vec3{0, 0, 1} == look_at({1, 0, 0}, {0, 0, 1}) * libv::vec3f(0, 0, 1));

	CHECK(approx_vec3{1, 0, 0} == look_at({1, 0, 0}, {0, 0, -1}) * libv::vec3f(1, 0, 0));
	CHECK(approx_vec3{0, -1, 0} == look_at({1, 0, 0}, {0, 0, -1}) * libv::vec3f(0, 1, 0));
	CHECK(approx_vec3{0, 0, -1} == look_at({1, 0, 0}, {0, 0, -1}) * libv::vec3f(0, 0, 1));

	CHECK(approx_vec3{0, 1, 0} == look_at({0, 1, 0}, {0, 0, 1}) * libv::vec3f(1, 0, 0));
	CHECK(approx_vec3{-1, 0, 0} == look_at({0, 1, 0}, {0, 0, 1}) * libv::vec3f(0, 1, 0));
	CHECK(approx_vec3{0, 0, 1} == look_at({0, 1, 0}, {0, 0, 1}) * libv::vec3f(0, 0, 1));

	CHECK(approx_vec3{0.5f, 0, 0.5f} == look_at({0, 0, 1}, {0, 0, 1}) * libv::vec3f(1, 0, 0)); // Look at up error
	CHECK(approx_vec3{0, 1, 0} == look_at({0, 0, 1}, {0, 0, 1}) * libv::vec3f(0, 1, 0));
	CHECK(approx_vec3{-0.5f, 0, 0.5f} == look_at({0, 0, 1}, {0, 0, 1}) * libv::vec3f(0, 0, 1)); // Look at up error

	CHECK(approx_vec3{0.7071f, 0.7071f, 0} == look_at({0.7071f, 0.7071f, 0}, {0, 0, 1}) * libv::vec3f(1, 0, 0));
	CHECK(approx_vec3{-0.7071f, 0.7071f, 0} == look_at({0.7071f, 0.7071f, 0}, {0, 0, 1}) * libv::vec3f(0, 1, 0));
	CHECK(approx_vec3{0, 0, 1} == look_at({0.7071f, 0.7071f, 0}, {0, 0, 1}) * libv::vec3f(0, 0, 1));

	CHECK(approx_vec3{0.57735f, 0.57735f, 0.57735f} == look_at({0.57735f, 0.57735f, 0.57735f}, {0, 0, 1}) * libv::vec3f(1, 0, 0));
	CHECK(approx_vec3{-0.7071f, 0.7071f, 0} == look_at({0.57735f, 0.57735f, 0.57735f}, {0, 0, 1}) * libv::vec3f(0, 1, 0));
	CHECK(approx_vec3{-0.408248f, -0.408248f, 0.816497f} == look_at({0.57735f, 0.57735f, 0.57735f}, {0, 0, 1}) * libv::vec3f(0, 0, 1));
}

TEST_CASE("quat angle_between", "[libv.math.quat]") {
	const auto q1 = libv::quatf::angle_axis(libv::deg_to_rad(0.0f), libv::vec3f(0, 0, 1));
	const auto q2 = libv::quatf::angle_axis(libv::deg_to_rad(45.0f), libv::vec3f(0, 0, 1));
	const auto q3 = libv::quatf::angle_axis(libv::deg_to_rad(90.0f), libv::vec3f(0, 0, 1));
	const auto q4 = libv::quatf::look_at(libv::vec3f{0, 0, 1}, {-1, 0, 0});

	CHECK(approx_f(libv::deg_to_rad(45.0f)) == libv::angle_between(q1, q2));
	CHECK(approx_f(libv::deg_to_rad(45.0f)) == libv::angle_between(q2, q3));
	CHECK(approx_f(libv::deg_to_rad(90.0f)) == libv::angle_between(q1, q3));

	CHECK(approx_f(libv::deg_to_rad(90.0f)) == libv::angle_between(q1, q4));
	CHECK(approx_f(1.71777f) == libv::angle_between(q2, q4));
	CHECK(approx_f(2.0944f) == libv::angle_between(q3, q4));

	const auto q5 = libv::quatf::look_at(libv::vec3f{1, 1, 1}.normalize(), {0, 0, 1});
	const auto q6 = libv::quatf::look_at(libv::vec3f{1, 1, 1}.normalize(), {0, 0, -1});
	CHECK(approx_f(libv::deg_to_rad(180.0f)) == libv::angle_between(q5, q6));
}

TEST_CASE("quat rotate_towards", "[libv.math.quat]") {
	const auto q1 = libv::quatf::look_at(libv::vec3f(1, 0, 0), libv::vec3f(0, 0, 1));
	const auto q2 = libv::quatf::look_at(libv::vec3f(0, 1, 0), libv::vec3f(0, 0, 1));

	CHECK(make_approx(q1) == libv::rotate_towards(q1, q2, -libv::pi));
	CHECK(make_approx(q1) == libv::rotate_towards(q1, q2, -1.f));
	CHECK(make_approx(q1) == libv::rotate_towards(q1, q2, 0.f));
	CHECK(approx{0.9990482f, 0, 0, 0.0436194f} == libv::rotate_towards(q1, q2, libv::deg_to_rad(5.0f)));
	CHECK(approx{0.9807853f, 0, 0, 0.1950903f} == libv::rotate_towards(q1, q2, libv::deg_to_rad(22.5f)));
	CHECK(approx{0.9238795f, 0, 0, 0.3826834f} == libv::rotate_towards(q1, q2, libv::deg_to_rad(45.0f)));
	CHECK(make_approx(q2) == libv::rotate_towards(q1, q2, libv::pi / 2.f));
	CHECK(make_approx(q2) == libv::rotate_towards(q1, q2, libv::pi));
	CHECK(make_approx(q2) == libv::rotate_towards(q1, q2, libv::tau));

	CHECK(make_approx(libv::slerp(q1, q2, 0.01f)) == libv::rotate_towards(q1, q2, libv::deg_to_rad(90.f * 0.01f)));
	CHECK(make_approx(libv::slerp(q1, q2, 0.25f)) == libv::rotate_towards(q1, q2, libv::deg_to_rad(22.5f)));
	CHECK(make_approx(libv::slerp(q1, q2, 0.5f)) == libv::rotate_towards(q1, q2, libv::deg_to_rad(45.0f)));
	CHECK(make_approx(libv::slerp(q1, q2, 0.99f)) == libv::rotate_towards(q1, q2, libv::deg_to_rad(90.f * 0.99f)));

	const auto q3 = libv::quatf::look_at(libv::vec3f(1, 1, 0).normalize(), libv::vec3f(0, 0, 1));
	const auto q4 = libv::quatf::look_at(libv::vec3f(0, 2, 1).normalize(), libv::vec3f(0, 1, 0));

	CHECK(make_approx(q3) == libv::rotate_towards(q3, q4, -libv::pi));
	CHECK(make_approx(q3) == libv::rotate_towards(q3, q4, 0.f));
	CHECK(make_approx(libv::slerp(q3, q4, 0.1f)) == libv::rotate_towards(q3, q4, libv::angle_between(q3, q4) * 0.1f));
	CHECK(make_approx(libv::slerp(q3, q4, 0.5f)) == libv::rotate_towards(q3, q4, libv::angle_between(q3, q4) * 0.5f));
	CHECK(make_approx(libv::slerp(q3, q4, 0.9f)) == libv::rotate_towards(q3, q4, libv::angle_between(q3, q4) * 0.9f));
	CHECK(make_approx(libv::slerp(q3, q4, 0.99f)) == libv::rotate_towards(q3, q4, libv::angle_between(q3, q4) * 0.99f));
	CHECK(not (make_approx(q4) == libv::rotate_towards(q3, q4, libv::angle_between(q3, q4) - 0.0001f)));
	CHECK(make_approx(q4) == libv::rotate_towards(q3, q4, libv::angle_between(q3, q4) + 0.0001f));
	CHECK(make_approx(q4) == libv::rotate_towards(q3, q4, libv::pi));
	CHECK(make_approx(q4) == libv::rotate_towards(q3, q4, libv::tau));

	CHECK(q1 == libv::rotate_towards(q1, q1, libv::tau));
	CHECK(q2 == libv::rotate_towards(q2, q2, libv::tau));
	CHECK(q3 == libv::rotate_towards(q3, q3, libv::tau));
	CHECK(q4 == libv::rotate_towards(q4, q4, libv::tau));

	CHECK(q1 == libv::rotate_towards(q1, q1, 0.0002f));
	CHECK(q2 == libv::rotate_towards(q2, q2, 0.0002f));
	CHECK(q3 == libv::rotate_towards(q3, q3, 0.0002f));
	CHECK(q4 == libv::rotate_towards(q4, q4, 0.0002f));
}

TEST_CASE("quat from_vectors", "[libv.math.quat]") {
	CHECK(approx{0.707107f, 0, 0, 0.707107f} == libv::quatf::from_unit_vectors(libv::vec3f(1, 0, 0), libv::vec3f(0, 1, 0)));
	CHECK(approx{0.707107f, 0, 0, 0.707107f} == libv::quatf::from_non_unit_vectors(libv::vec3f(1, 0, 0), libv::vec3f(0, 1, 0)));

	const auto s0 = libv::vec3f{1, 0, 0};
	const auto d0 = libv::vec3f{0, 1, 0};
	CHECK(approx_vec3{d0} == libv::quatf::from_unit_vectors(s0, d0) * s0);
	CHECK(approx_vec3{d0} == libv::quatf::from_non_unit_vectors(s0, d0) * s0);

	const auto s1 = libv::vec3f{1, 14, 0};
	const auto d1 = libv::vec3f{5, 1, 2};
	CHECK(approx_vec3{d1.normalize_copy()} == libv::quatf::from_unit_vectors(s1.normalize_copy(), d1.normalize_copy()) * s1.normalize_copy());
	CHECK(approx_vec3{d1.normalize_copy()} == libv::quatf::from_non_unit_vectors(s1, d1) * s1.normalize_copy());
}

// =================================================================================================

TEST_CASE("Test math.quat formatter", "[libv.math.quat]") {
	libv::quatf v0(0, 42.0f, libv::pi, -0.3f);
	libv::quatf v1(0, 1, 2, 3);

	CHECK(fmt::format("{}", v1) == "0 1 2 3");
	CHECK(fmt::format("{}", v0) == "0 42 3.1415927 -0.3");
	CHECK(fmt::format("{:}", v0) == "0423.1415927-0.3");
	CHECK(fmt::format("{: }", v0) == "0 42 3.1415927 -0.3");
	CHECK(fmt::format("{::}", v0) == "0423.1415927-0.3");
	CHECK(fmt::format("{: :}", v0) == "0 42 3.1415927 -0.3");

	CHECK(fmt::format("{:, }", v0) == "0, 42, 3.1415927, -0.3");

	CHECK(fmt::format("{::07.3f}", v0) == "000.000042.000003.142-00.300");
	CHECK(fmt::format("{::<6}", v0) == "0     42    3.1415927-0.3  ");
	CHECK(fmt::format("{::^6}", v0) == "  0     42  3.1415927 -0.3 ");
	CHECK(fmt::format("{::>6}", v0) == "     0    423.1415927  -0.3");

	CHECK(fmt::format("{: :07.3f}", v0) == "000.000 042.000 003.142 -00.300");
	CHECK(fmt::format("{: :<6}", v0) == "0      42     3.1415927 -0.3  ");
	CHECK(fmt::format("{: :^6}", v0) == "  0      42   3.1415927  -0.3 ");
	CHECK(fmt::format("{: :>6}", v0) == "     0     42 3.1415927   -0.3");

	CHECK(fmt::format("{:, :07.3f}", v0) == "000.000, 042.000, 003.142, -00.300");
	CHECK(fmt::format("{:, :<6}", v0) == "0     , 42    , 3.1415927, -0.3  ");
	CHECK(fmt::format("{:, :^6}", v0) == "  0   ,   42  , 3.1415927,  -0.3 ");
	CHECK(fmt::format("{:, :>6}", v0) == "     0,     42, 3.1415927,   -0.3");
}
