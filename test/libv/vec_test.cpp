// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <libv/vec.hpp>

#include <catch.hpp>
#include <string>

using namespace libv;

TEST_CASE("unionValue") {
	vec4 vec(0, 1, 2, 3);

	CHECK(0 == vec.x);
	CHECK(1 == vec.y);
	CHECK(2 == vec.z);
	CHECK(3 == vec.w);

	vec.x = 42;

	CHECK(42 == vec.x);
	CHECK(42 == vec.r);
	CHECK(42 == vec.s);
	CHECK(42 == vec.ptr[0]);
	CHECK(1 == vec.y);
	CHECK(1 == vec.g);
	CHECK(1 == vec.t);
	CHECK(1 == vec.ptr[1]);
	CHECK(2 == vec.z);
	CHECK(2 == vec.b);
	CHECK(2 == vec.p);
	CHECK(2 == vec.ptr[2]);
	CHECK(3 == vec.w);
	CHECK(3 == vec.a);
	CHECK(3 == vec.q);
	CHECK(3 == vec.ptr[3]);

	CHECK((4 * sizeof (float)) == sizeof (vec));
}

TEST_CASE("unionAddress") {
	vec4_t<double> vec(0, 1, 2, 3);

	CHECK(&vec.x == vec.ptr);
	CHECK(&vec.y == vec.ptr + 1);
	CHECK(&vec.z == vec.ptr + 2);
	CHECK(&vec.w == vec.ptr + 3);

	CHECK(&vec.x == &vec.ptr[0]);
	CHECK(&vec.x == &vec.r);
	CHECK(&vec.x == &vec.s);

	CHECK(&vec.y == &vec.ptr[1]);
	CHECK(&vec.y == &vec.g);
	CHECK(&vec.y == &vec.t);

	CHECK(&vec.z == &vec.ptr[2]);
	CHECK(&vec.z == &vec.b);
	CHECK(&vec.z == &vec.p);

	CHECK(&vec.w == &vec.ptr[3]);
	CHECK(&vec.w == &vec.a);
	CHECK(&vec.w == &vec.q);

	CHECK((4 * sizeof (double)) == sizeof (vec));
}

TEST_CASE("dimMacro") {
	vec2 veca2(1, 2);
	vec3 veca3(1, 2, 3);
	vec4 veca4(1, 2, 3, 4);
	vec2 vecb2(1, 2);
	vec3 vecb3(1, 2, 3);
	vec4 vecb4(1, 2, 3, 4);

	veca2 += vecb2;
	veca3 += vecb3;
	veca4 += vecb4;

	CHECK(veca2 == vec2(2, 4));
	CHECK(veca3 == vec3(2, 4, 6));
	CHECK(veca4 == vec4(2, 4, 6, 8));
}

TEST_CASE("ctor") {
	vec2 vec2d;
	vec2 vec20(1, 2);
	vec2 vec21(vec20);
	vec3 vec3d;
	vec3 vec30(1, 2, 3);
	vec3 vec31(1, vec20);
	vec3 vec32(vec20, 3);
	vec3 vec33(vec30);
	vec4 vec4d;
	vec4 vec40(1, 2, 3, 4);
	vec4 vec41(vec20, 3, 4);
	vec4 vec42(1, vec20, 4);
	vec4 vec43(1, 2, vec20);
	vec4 vec44(vec20, vec20);
	vec4 vec45(vec30, 4);
	vec4 vec46(1, vec30);
	vec4 vec47(vec40);

	CHECK(vec2d == vec2(0, 0));
	CHECK(vec20 == vec2(1, 2));
	CHECK(vec21 == vec2(1, 2));
	CHECK(vec3d == vec3(0, 0, 0));
	CHECK(vec30 == vec3(1, 2, 3));
	CHECK(vec31 == vec3(1, 1, 2));
	CHECK(vec32 == vec3(1, 2, 3));
	CHECK(vec33 == vec3(1, 2, 3));
	CHECK(vec4d == vec4(0, 0, 0, 0));
	CHECK(vec40 == vec4(1, 2, 3, 4));
	CHECK(vec41 == vec4(1, 2, 3, 4));
	CHECK(vec42 == vec4(1, 1, 2, 4));
	CHECK(vec43 == vec4(1, 2, 1, 2));
	CHECK(vec44 == vec4(1, 2, 1, 2));
	CHECK(vec45 == vec4(1, 2, 3, 4));
	CHECK(vec46 == vec4(1, 1, 2, 3));
	CHECK(vec47 == vec4(1, 2, 3, 4));

	vec2 vec200(1.2f, 2.2f);
	vec3 vec300(1.8f, 2.2f, 3.6f);
	vec4 vec400(1.2f, 2.3f, 3.2f, 4.7f);
	ivec2 vec201(vec200);
	ivec3 vec301(vec300);
	ivec4 vec401(vec400);

	CHECK(vec200 != vec2(1, 2));
	CHECK(vec300 != vec3(1, 2, 3));
	CHECK(vec400 != vec4(1, 2, 3, 4));
	CHECK(vec201 == vec2(1, 2));
	CHECK(vec301 == vec3(1, 2, 3));
	CHECK(vec401 == vec4(1, 2, 3, 4));
	CHECK(vec200 != ivec2(1, 2));
	CHECK(vec300 != ivec3(1, 2, 3));
	CHECK(vec400 != ivec4(1, 2, 3, 4));
	CHECK(vec201 == ivec2(1, 2));
	CHECK(vec301 == ivec3(1, 2, 3));
	CHECK(vec401 == ivec4(1, 2, 3, 4));
}

TEST_CASE("nonMemberOperator") {
	dvec3 vec0(1.5, 4.2, 3.14);

	dvec3 vec1 = vec0 / 2;
	dvec3 vec2 = 0.5 * vec0;
	vec0 *= 0.5f;

	CHECK(vec0 == dvec3(0.75, 2.1, 1.57));
	CHECK(dvec3(0.75, 2.1, 1.57) == vec0);
	CHECK(vec1 == dvec3(0.75, 2.1, 1.57));
	CHECK(dvec3(0.75, 2.1, 1.57) == vec1);
	CHECK(vec2 == dvec3(0.75, 2.1, 1.57));
	CHECK(dvec3(0.75, 2.1, 1.57) == vec2);
}

TEST_CASE("normalize") {
	dvec2 vec0(15, 15);

	CHECK(vec0 > 2);
	CHECK(vec0.normalize_copy() < 2);
	CHECK(2 < vec0);
	CHECK(2 > vec0.normalize_copy());

	CHECK(1 == Approx(vec0.normalize_copy().length()));
}

TEST_CASE("Custom getter functions") {
	ivec4 vec0(1, 2, 3, 4);

	CHECK(vec0.xy() == ivec2(1, 2));
	CHECK(vec0.xyz() == ivec3(1, 2, 3));
	CHECK(vec0.xyzw() == vec0);

	CHECK(vec0.xxww() == ivec4(1, 1, 4, 4));

	CHECK(vec0[0] == 1);
	CHECK(vec0[1] == 2);
	CHECK(vec0[2] == 3);
	CHECK(vec0[3] == 4);
}

TEST_CASE("Copy ctor from different type") {
	vec4 v0(1.1, 2.1, 3.1, 4.1);
	ivec4 v1(v0);

	CHECK(v1 == ivec4(1, 2, 3, 4));
}

TEST_CASE("Non trivially destructible type") {
	vec_t<2, std::string> v0("x", "y");

	CHECK(v0[0] == "x");
	CHECK(v0[1] == "y");
}

TEST_CASE("Operator=") {
	vec_t<2, float> v0(1.1, 2.1);
	vec_t<2, float> v1(4.1, 5.1);
	vec_t<2, int> v2(1, 2);

	v0 = v1;
	v2 = v0;

	CHECK(v2 == ivec2(4, 5));
}

TEST_CASE("Operator*=") {
	vec_t<2, int> v0(1, 2);

	vec_t<2, int> v1(1, 1);
	vec_t<2, int> v2(4, 4);
	vec_t<2, int> v3(2, 2);
	vec_t<2, int> v4(3, 0);

	v0 += v1;
	CHECK(v0 == ivec2(2, 3));
	v0 *= v2;
	CHECK(v0 == ivec2(8, 12));
	v0 /= v3;
	CHECK(v0 == ivec2(4, 6));
	v0 -= v4;
	CHECK(v0 == ivec2(1, 6));
}

TEST_CASE("length") {
	vec_t<2, float> v0(0, 10);
	vec_t<2, float> v1(10, 10);

	CHECK(10.0f == Approx(v0.length()));
	CHECK(100.0f == Approx(v0.lengthSquare()));
	CHECK(14.1421f == Approx(v1.length()));
	CHECK(200.0f == Approx(v1.lengthSquare()));
}

TEST_CASE("operator*(vec, vec)") {
	vec_t<2, int> v0(1, 2);

	vec_t<2, int> v1(1, 1);
	vec_t<2, int> v2(4, 4);
	vec_t<2, int> v3(2, 2);
	vec_t<2, int> v4(3, 0);

	auto r0 = (v0 + v1);
	auto r1 = (v0 * v2);
	auto r2 = (v0 / v3);
	auto r3 = (v0 - v4);

	static_assert(std::is_same<vec_t<2, int>, decltype(r0)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, int>, decltype(r1)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, int>, decltype(r2)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, int>, decltype(r3)>::value, "Wrong result type.");

	CHECK((v0 + v1) == ivec2(2, 3));
	CHECK((v0 * v2) == ivec2(4, 8));
	CHECK((v0 / v3) == ivec2(0, 1));
	CHECK((v0 - v4) == ivec2(-2, 2));
}

TEST_CASE("operator*(vec, vec) with different types") {
	vec_t<2, float> v0(1, 2);

	vec_t<2, int> v1(1, 1);
	vec_t<2, int> v2(4, 4);
	vec_t<2, int> v3(2, 2);
	vec_t<2, int> v4(3, 0);

	auto r0 = (v0 + v1);
	auto r1 = (v0 * v2);
	auto r2 = (v0 / v3);
	auto r3 = (v0 - v4);

	static_assert(std::is_same<vec_t<2, float>, decltype(r0)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, float>, decltype(r1)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, float>, decltype(r2)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, float>, decltype(r3)>::value, "Wrong result type.");

	CHECK(r0 == fvec2(2, 3));
	CHECK(r1 == fvec2(4, 8));
	CHECK(r2 == fvec2(0.5f, 1));
	CHECK(r3 == fvec2(-2, 2));
}

TEST_CASE("operator*(vec, skalar)") {
	vec_t<2, int> v0(4, 8);

	int v1 = 2;
	int v2 = 8;

	auto r0 = (v0 * v1);
	auto r1 = (v0 / v2);
	auto r2 = (v1 * v0);
	auto r3 = (v2 / v0);

	static_assert(std::is_same<vec_t<2, int>, decltype(r0)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, int>, decltype(r1)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, int>, decltype(r2)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, int>, decltype(r3)>::value, "Wrong result type.");

	CHECK(r0 == ivec2(8, 16));
	CHECK(r1 == ivec2(0, 1));
	CHECK(r2 == ivec2(8, 16));
	CHECK(r3 == ivec2(2, 1));
}

TEST_CASE("operator*(vec, skalar) with different types") {
	vec_t<2, int> v0(4, 8);

	float v1 = 2;
	float v2 = 8;

	auto r0 = (v0 * v1);
	auto r1 = (v0 / v2);
	auto r2 = (v1 * v0);
	auto r3 = (v2 / v0);

	static_assert(std::is_same<vec_t<2, float>, decltype(r0)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, float>, decltype(r1)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, float>, decltype(r2)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, float>, decltype(r3)>::value, "Wrong result type.");

	CHECK(r0 == fvec2(8, 16));
	CHECK(r1 == fvec2(0.5f, 1));
	CHECK(r2 == fvec2(8, 16));
	CHECK(r3 == fvec2(2, 1));
}

TEST_CASE("operator+") {
	vec_t<2, int> v0(4, 8);

	auto r0 = +v0;
	auto r1 = -v0;

	static_assert(std::is_same<vec_t<2, int>, decltype(r0)>::value, "Wrong result type.");
	static_assert(std::is_same<vec_t<2, int>, decltype(r1)>::value, "Wrong result type.");

	CHECK(r0 == ivec2(4, 8));
	CHECK(r1 == ivec2(-4, -8));
}