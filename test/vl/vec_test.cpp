// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <vl/vec.hpp>

#include <catch.hpp>
#include <iostream>

using namespace vl;

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
	vec2 vec20(1, 2);
	vec2 vec21(vec20);
	vec3 vec30(1, 2, 3);
	vec3 vec31(1, vec20);
	vec3 vec32(vec20, 3);
	vec3 vec33(vec30);
	vec4 vec40(1, 2, 3, 4);
	vec4 vec41(vec20, 3, 4);
	vec4 vec42(1, vec20, 4);
	vec4 vec43(1, 2, vec20);
	vec4 vec44(vec20, vec20);
	vec4 vec45(vec30, 4);
	vec4 vec46(1, vec30);
	vec4 vec47(vec40);

	CHECK(vec20 == vec2(1, 2));
	CHECK(vec21 == vec2(1, 2));
	CHECK(vec30 == vec3(1, 2, 3));
	CHECK(vec31 == vec3(1, 1, 2));
	CHECK(vec32 == vec3(1, 2, 3));
	CHECK(vec33 == vec3(1, 2, 3));
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
	CHECK(vec0.normalized() < 2);
	CHECK(2 < vec0);
	CHECK(2 > vec0.normalized());

	CHECK(1 == Approx(vec0.normalized().length()).epsilon(0.0001));
}