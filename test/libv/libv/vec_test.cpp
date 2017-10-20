// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <libv/math/vec.hpp>

#include <catch.hpp>
#include <string>


// -------------------------------------------------------------------------------------------------

template <typename Tag>
struct CtorCounter {
	static inline size_t default_ctor = 0;
	static inline size_t copy_ctor = 0;
	static inline size_t move_ctor = 0;
	static inline size_t copy_assign = 0;
	static inline size_t move_assign = 0;
	static inline size_t dtor = 0;

	static inline void reset () {
		default_ctor = 0;
		copy_ctor = 0;
		move_ctor = 0;
		copy_assign = 0;
		move_assign = 0;
		dtor = 0;
	}

	CtorCounter() { ++default_ctor; }
	CtorCounter(const CtorCounter&) { ++copy_ctor; }
	CtorCounter(CtorCounter&&) { ++move_ctor; }
	CtorCounter& operator=(const CtorCounter&) { ++copy_assign; return *this; }
	CtorCounter& operator=(CtorCounter&&) { ++move_assign; return *this; }
	~CtorCounter() { ++dtor; }
};

// -------------------------------------------------------------------------------------------------

TEST_CASE("unionValue") {
	libv::vec4f vec(0, 1, 2, 3);

	CHECK(0.f == vec.x);
	CHECK(1.f == vec.y);
	CHECK(2.f == vec.z);
	CHECK(3.f == vec.w);

	vec.x = 42;

	CHECK(42.f == vec.x);
	CHECK(42.f == vec.r);
	CHECK(42.f == vec.s);
	CHECK(42.f == vec.data[0]);
	CHECK(1.f == vec.y);
	CHECK(1.f == vec.g);
	CHECK(1.f == vec.t);
	CHECK(1.f == vec.data[1]);
	CHECK(2.f == vec.z);
	CHECK(2.f == vec.b);
	CHECK(2.f == vec.p);
	CHECK(2.f == vec.data[2]);
	CHECK(3.f == vec.w);
	CHECK(3.f == vec.a);
	CHECK(3.f == vec.q);
	CHECK(3.f == vec.data[3]);

	CHECK((4 * sizeof (float)) == sizeof (vec));
}

TEST_CASE("unionAddress") {
	libv::vec4_t<double> vec(0, 1, 2, 3);

	CHECK(&vec.x == &vec.data[0]);
	CHECK(&vec.y == &vec.data[0] + 1);
	CHECK(&vec.z == &vec.data[0] + 2);
	CHECK(&vec.w == &vec.data[0] + 3);

	CHECK(&vec.x == &vec.data[0]);
	CHECK(&vec.x == &vec.r);
	CHECK(&vec.x == &vec.s);

	CHECK(&vec.y == &vec.data[1]);
	CHECK(&vec.y == &vec.g);
	CHECK(&vec.y == &vec.t);

	CHECK(&vec.z == &vec.data[2]);
	CHECK(&vec.z == &vec.b);
	CHECK(&vec.z == &vec.p);

	CHECK(&vec.w == &vec.data[3]);
	CHECK(&vec.w == &vec.a);
	CHECK(&vec.w == &vec.q);

	CHECK((4 * sizeof (double)) == sizeof (vec));
}

TEST_CASE("dimMacro") {
	libv::vec2u veca2(1, 2);
	libv::vec3u veca3(1, 2, 3);
	libv::vec4u veca4(1, 2, 3, 4);
	libv::vec2u vecb2(1, 2);
	libv::vec3u vecb3(1, 2, 3);
	libv::vec4u vecb4(1, 2, 3, 4);

	veca2 += vecb2;
	veca3 += vecb3;
	veca4 += vecb4;

	CHECK(veca2 == libv::vec2f(2, 4));
	CHECK(veca3 == libv::vec3f(2, 4, 6));
	CHECK(veca4 == libv::vec4f(2, 4, 6, 8));
}

TEST_CASE("ctor") {
	libv::vec2f vec2d;
	libv::vec2f vec20(1, 2);
	libv::vec2f vec21(vec20);
	libv::vec3f vec3d;
	libv::vec3f vec30(1, 2, 3);
	libv::vec3f vec31(1, vec20);
	libv::vec3f vec32(vec20, 3);
	libv::vec3f vec33(vec30);
	libv::vec4f vec4d;
	libv::vec4f vec40(1, 2, 3, 4);
	libv::vec4f vec41(vec20, 3, 4);
	libv::vec4f vec42(1, vec20, 4);
	libv::vec4f vec43(1, 2, vec20);
	libv::vec4f vec44(vec20, vec20);
	libv::vec4f vec45(vec30, 4);
	libv::vec4f vec46(1, vec30);
	libv::vec4f vec47(vec40);

	CHECK(vec2d == libv::vec2f(0, 0));
	CHECK(vec20 == libv::vec2f(1, 2));
	CHECK(vec21 == libv::vec2f(1, 2));
	CHECK(vec3d == libv::vec3f(0, 0, 0));
	CHECK(vec30 == libv::vec3f(1, 2, 3));
	CHECK(vec31 == libv::vec3f(1, 1, 2));
	CHECK(vec32 == libv::vec3f(1, 2, 3));
	CHECK(vec33 == libv::vec3f(1, 2, 3));
	CHECK(vec4d == libv::vec4f(0, 0, 0, 0));
	CHECK(vec40 == libv::vec4f(1, 2, 3, 4));
	CHECK(vec41 == libv::vec4f(1, 2, 3, 4));
	CHECK(vec42 == libv::vec4f(1, 1, 2, 4));
	CHECK(vec43 == libv::vec4f(1, 2, 1, 2));
	CHECK(vec44 == libv::vec4f(1, 2, 1, 2));
	CHECK(vec45 == libv::vec4f(1, 2, 3, 4));
	CHECK(vec46 == libv::vec4f(1, 1, 2, 3));
	CHECK(vec47 == libv::vec4f(1, 2, 3, 4));

	libv::vec2f vec200(1.2f, 2.2f);
	libv::vec3f vec300(1.8f, 2.2f, 3.6f);
	libv::vec4f vec400(1.2f, 2.3f, 3.2f, 4.7f);
	libv::vec2i vec201(vec200);
	libv::vec3i vec301(vec300);
	libv::vec4i vec401(vec400);

	CHECK(vec200 != libv::vec2f(1, 2));
	CHECK(vec300 != libv::vec3f(1, 2, 3));
	CHECK(vec400 != libv::vec4f(1, 2, 3, 4));
	CHECK(vec201 == libv::vec2f(1, 2));
	CHECK(vec301 == libv::vec3f(1, 2, 3));
	CHECK(vec401 == libv::vec4f(1, 2, 3, 4));
	CHECK(vec200 != libv::vec2i(1, 2));
	CHECK(vec300 != libv::vec3i(1, 2, 3));
	CHECK(vec400 != libv::vec4i(1, 2, 3, 4));
	CHECK(vec201 == libv::vec2i(1, 2));
	CHECK(vec301 == libv::vec3i(1, 2, 3));
	CHECK(vec401 == libv::vec4i(1, 2, 3, 4));
}

TEST_CASE("nonMemberOperator") {
	libv::vec3d vec0(1.5, 4.2, 3.14);

	libv::vec3d vec1 = vec0 / 2;
	libv::vec3d vec2 = 0.5 * vec0;
	vec0 *= 0.5;

	CHECK(vec0 == libv::vec3d(0.75, 2.1, 1.57));
	CHECK(libv::vec3d(0.75, 2.1, 1.57) == vec0);
	CHECK(vec1 == libv::vec3d(0.75, 2.1, 1.57));
	CHECK(libv::vec3d(0.75, 2.1, 1.57) == vec1);
	CHECK(vec2 == libv::vec3d(0.75, 2.1, 1.57));
	CHECK(libv::vec3d(0.75, 2.1, 1.57) == vec2);
}

TEST_CASE("normalize") {
	libv::vec2d vec0(15, 15);

	CHECK(vec0 > 2);
	CHECK(vec0.normalize_copy() < 2);
	CHECK(2 < vec0);
	CHECK(2 > vec0.normalize_copy());

	CHECK(1 == Approx(vec0.normalize_copy().length()));
}

TEST_CASE("Custom getter functions") {
	libv::vec4i vec0(1, 2, 3, 4);

	CHECK(libv::vec::xy(vec0) == libv::vec2i(1, 2));
	CHECK(libv::vec::xyz(vec0) == libv::vec3i(1, 2, 3));
	CHECK(libv::vec::xyzw(vec0) == vec0);
	CHECK(libv::vec::xxww(vec0) == libv::vec4i(1, 1, 4, 4));

	CHECK(xy(vec0) == libv::vec2i(1, 2));
	CHECK(xyz(vec0) == libv::vec3i(1, 2, 3));
	CHECK(xyzw(vec0) == vec0);
	CHECK(xxww(vec0) == libv::vec4i(1, 1, 4, 4));

	CHECK(vec0[0] == 1);
	CHECK(vec0[1] == 2);
	CHECK(vec0[2] == 3);
	CHECK(vec0[3] == 4);
}

TEST_CASE("Freestanding one dim getter functions") {
	libv::vec4i vec0(1, 2, 3, 4);

	CHECK(vec0.x == 1);
	CHECK(x(vec0) == 1);
	CHECK(y(vec0) == 2);
	CHECK(z(vec0) == 3);
	CHECK(w(vec0) == 4);

	x(vec0) = 50;

	CHECK(vec0.x == 50);
	CHECK(x(vec0) == 50);
}

TEST_CASE("Copy ctor from different type") {
	libv::vec4d v0(1.1, 2.1, 3.1, 4.1);
	libv::vec4i v1(v0);

	CHECK(v1 == libv::vec4i(1, 2, 3, 4));
}

TEST_CASE("Non trivially destructible type") {
	libv::vec_t<2, std::string> v0("x", "y");

	CHECK(v0[0] == "x");
	CHECK(v0[1] == "y");
}

TEST_CASE("operator=") {
	libv::vec_t<2, float> v0(1.1, 2.1);
	libv::vec_t<2, float> v1(4.1, 5.1);
	libv::vec_t<2, int> v2(1, 2);

	v0 = v1;
	v2 = v0;

	CHECK(v2 == libv::vec2i(4, 5));
}

TEST_CASE("copy / move assignment") {
	class Tag;
	using A = CtorCounter<Tag>;

	{
		libv::vec_t<3, A> v0{};
		libv::vec_t<3, A> v1{};
		auto make_prvalue = []{ return libv::vec_t<3, A>{}; }; // Guaranteed copy elision

		v0 = v1;
		v0 = std::move(v1);
		v0 = make_prvalue();
	}

	CHECK(A::default_ctor == 9);
	CHECK(A::copy_ctor == 0);
	CHECK(A::move_ctor == 0);
	CHECK(A::copy_assign == 3);
	CHECK(A::move_assign == 6);
	CHECK(A::dtor == 9);
}

TEST_CASE("copy / move ctor") {
	class Tag;
	using A = CtorCounter<Tag>;

	{
		libv::vec_t<3, A> v1{};
		auto make_prvalue = []{ return libv::vec_t<3, A>{}; }; // Guaranteed copy elision

		SECTION("=") {
			auto r0 = v1;
			auto r1 = std::move(v1);
			auto r2 = make_prvalue();
			(void) r0; (void) r1; (void) r2;
		}

		SECTION("()") {
			auto r0(v1);
			auto r1(std::move(v1));
			auto r2(make_prvalue());
		}

		SECTION("{}") {
			auto r0{v1};
			auto r1{std::move(v1)};
			auto r2{make_prvalue()};
		}
	}

	CHECK(A::default_ctor == 6);
	CHECK(A::copy_ctor == 3);
	CHECK(A::move_ctor == 3);
	CHECK(A::copy_assign == 0);
	CHECK(A::move_assign == 0);
	CHECK(A::dtor == 12);
	A::reset();
}

TEST_CASE("copy / move assignment with non trivial payload") {
	class Tag;
	using A = CtorCounter<Tag>;
	struct Payload {
		A counter;
		std::string payload;
	};

	{
		libv::vec_t<3, Payload> v0{};
		libv::vec_t<3, Payload> v1{};
		auto make_prvalue = []{ return libv::vec_t<3, Payload>{}; }; // Guaranteed copy elision

		v0 = v1;
		v0 = std::move(v1);
		v0 = make_prvalue();
	}

	CHECK(A::default_ctor == 9);
	CHECK(A::copy_ctor == 0);
	CHECK(A::move_ctor == 0);
	CHECK(A::copy_assign == 3);
	CHECK(A::move_assign == 6);
	CHECK(A::dtor == 9);
}

TEST_CASE("copy / move ctor with non trivial payload") {
	class Tag;
	using A = CtorCounter<Tag>;
	struct Payload {
		A counter;
		std::string payload;
	};

	{
		libv::vec_t<3, Payload> v1{};
		auto make_prvalue = []{ return libv::vec_t<3, Payload>{}; }; // Guaranteed copy elision

		SECTION("=") {
			auto r0 = v1;
			auto r1 = std::move(v1);
			auto r2 = make_prvalue();
			(void) r0; (void) r1; (void) r2;
		}

		SECTION("()") {
			auto r0(v1);
			auto r1(std::move(v1));
			auto r2(make_prvalue());
		}

		SECTION("{}") {
			auto r0{v1};
			auto r1{std::move(v1)};
			auto r2{make_prvalue()};
		}
	}

	CHECK(A::default_ctor == 6);
	CHECK(A::copy_ctor == 3);
	CHECK(A::move_ctor == 3);
	CHECK(A::copy_assign == 0);
	CHECK(A::move_assign == 0);
	CHECK(A::dtor == 12);
	A::reset();
}

TEST_CASE("operator*=") {
	libv::vec_t<2, int> v0(1, 2);

	libv::vec_t<2, int> v1(1, 1);
	libv::vec_t<2, int> v2(4, 4);
	libv::vec_t<2, int> v3(2, 2);
	libv::vec_t<2, int> v4(3, 0);

	v0 += v1;
	CHECK(v0 == libv::vec2i(2, 3));
	v0 *= v2;
	CHECK(v0 == libv::vec2i(8, 12));
	v0 /= v3;
	CHECK(v0 == libv::vec2i(4, 6));
	v0 -= v4;
	CHECK(v0 == libv::vec2i(1, 6));
}

TEST_CASE("length") {
	libv::vec_t<2, float> v0(0, 10);
	libv::vec_t<2, float> v1(10, 10);

	CHECK(10.0f == Approx(v0.length()));
	CHECK(100.0f == Approx(v0.lengthSQ()));
	CHECK(14.1421f == Approx(v1.length()));
	CHECK(200.0f == Approx(v1.lengthSQ()));
}

TEST_CASE("operator*(vec, vec)") {
	libv::vec_t<2, int> v0(1, 2);

	libv::vec_t<2, int> v1(1, 1);
	libv::vec_t<2, int> v2(4, 4);
	libv::vec_t<2, int> v3(2, 2);
	libv::vec_t<2, int> v4(3, 0);

	auto r0 = (v0 + v1);
	auto r1 = (v0 * v2);
	auto r2 = (v0 / v3);
	auto r3 = (v0 - v4);

	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r0)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r1)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r2)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r3)>::value, "Wrong result type.");

	CHECK((v0 + v1) == libv::vec2i(2, 3));
	CHECK((v0 * v2) == libv::vec2i(4, 8));
	CHECK((v0 / v3) == libv::vec2i(0, 1));
	CHECK((v0 - v4) == libv::vec2i(-2, 2));
}

TEST_CASE("operator*(vec, vec) with different types") {
	libv::vec_t<2, float> v0(1, 2);

	libv::vec_t<2, int> v1(1, 1);
	libv::vec_t<2, int> v2(4, 4);
	libv::vec_t<2, int> v3(2, 2);
	libv::vec_t<2, int> v4(3, 0);

	auto r0 = (v0 + v1);
	auto r1 = (v0 * v2);
	auto r2 = (v0 / v3);
	auto r3 = (v0 - v4);

	static_assert(std::is_same<libv::vec_t<2, float>, decltype(r0)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, float>, decltype(r1)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, float>, decltype(r2)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, float>, decltype(r3)>::value, "Wrong result type.");

	CHECK(r0 == libv::vec2f(2, 3));
	CHECK(r1 == libv::vec2f(4, 8));
	CHECK(r2 == libv::vec2f(0.5f, 1));
	CHECK(r3 == libv::vec2f(-2, 2));
}

TEST_CASE("operator*(vec, skalar)") {
	libv::vec_t<2, int> v0(4, 8);

	int v1 = 2;
	int v2 = 8;

	auto r0 = (v0 * v1);
	auto r1 = (v0 / v2);
	auto r2 = (v1 * v0);
	auto r3 = (v2 / v0);

	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r0)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r1)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r2)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r3)>::value, "Wrong result type.");

	CHECK(r0 == libv::vec2i(8, 16));
	CHECK(r1 == libv::vec2i(0, 1));
	CHECK(r2 == libv::vec2i(8, 16));
	CHECK(r3 == libv::vec2i(2, 1));
}

TEST_CASE("operator*(vec, skalar) with different types") {
	libv::vec_t<2, int> v0(4, 8);

	float v1 = 2;
	float v2 = 8;

	auto r0 = (v0 * v1);
	auto r1 = (v0 / v2);
	auto r2 = (v1 * v0);
	auto r3 = (v2 / v0);

	static_assert(std::is_same<libv::vec_t<2, float>, decltype(r0)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, float>, decltype(r1)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, float>, decltype(r2)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, float>, decltype(r3)>::value, "Wrong result type.");

	CHECK(r0 == libv::vec2f(8, 16));
	CHECK(r1 == libv::vec2f(0.5f, 1));
	CHECK(r2 == libv::vec2f(8, 16));
	CHECK(r3 == libv::vec2f(2, 1));
}

TEST_CASE("operator+") {
	libv::vec_t<2, int> v0(+4, +8);
	libv::vec_t<2, int> v1(-4, +8);
	libv::vec_t<2, int> v2(-4, -8);

	auto r00 = +v0;
	auto r01 = -v0;
	auto r10 = +v1;
	auto r11 = -v1;
	auto r20 = +v2;
	auto r21 = -v2;

	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r00)>::value, "Wrong result type.");
	static_assert(std::is_same<libv::vec_t<2, int>, decltype(r01)>::value, "Wrong result type.");

	CHECK(r00 == libv::vec2i(+4, +8));
	CHECK(r01 == libv::vec2i(-4, -8));
	CHECK(r10 == libv::vec2i(-4, +8));
	CHECK(r11 == libv::vec2i(+4, -8));
	CHECK(r20 == libv::vec2i(-4, -8));
	CHECK(r21 == libv::vec2i(+4, +8));
}
