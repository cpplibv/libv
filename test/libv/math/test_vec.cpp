// Project: libv.math, File: test/libv/math/test_vec.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/math/constants.hpp>
// std
#include <string>
// pro
#include <libv/math/vec.hpp>
#include <libv/utility/approx.hpp>


// -------------------------------------------------------------------------------------------------

template <typename Tag>
struct CtorCounter {
	static inline std::size_t default_ctor = 0;
	static inline std::size_t copy_ctor = 0;
	static inline std::size_t move_ctor = 0;
	static inline std::size_t copy_assign = 0;
	static inline std::size_t move_assign = 0;
	static inline std::size_t dtor = 0;

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
	CtorCounter& operator=(const CtorCounter&) & { ++copy_assign; return *this; }
	CtorCounter& operator=(CtorCounter&&) & { ++move_assign; return *this; }
	~CtorCounter() { ++dtor; }
};

//template <std::size_t N, typename T>
//libv::vec_t<N, libv::approx<T>> approx(libv::vec_t<N, T> vec) {
//	return vec.template cast<libv::approx<T>>();
//}

// -------------------------------------------------------------------------------------------------

TEST_CASE("unionValue", "[libv.math.vec]") {
	libv::vec4f vec(0, 1, 2, 3);

	CHECK(0.f == vec.x);
	CHECK(1.f == vec.y);
	CHECK(2.f == vec.z);
	CHECK(3.f == vec.w);

	vec.x = 42;

	CHECK(42.f == vec.x);
//	CHECK(42.f == vec.r);
//	CHECK(42.f == vec.s);
	CHECK(42.f == vec.data()[0]);
	CHECK(1.f == vec.y);
//	CHECK(1.f == vec.g);
//	CHECK(1.f == vec.t);
	CHECK(1.f == vec.data()[1]);
	CHECK(2.f == vec.z);
//	CHECK(2.f == vec.b);
//	CHECK(2.f == vec.p);
	CHECK(2.f == vec.data()[2]);
	CHECK(3.f == vec.w);
//	CHECK(3.f == vec.a);
//	CHECK(3.f == vec.q);
	CHECK(3.f == vec.data()[3]);

	CHECK((4 * sizeof (float)) == sizeof (vec));
}

TEST_CASE("unionAddress", "[libv.math.vec]") {
	libv::vec4_t<double> vec(0, 1, 2, 3);

	CHECK(&vec.x == &vec.data()[0]);
	CHECK(&vec.y == &vec.data()[1]);
	CHECK(&vec.z == &vec.data()[2]);
	CHECK(&vec.w == &vec.data()[3]);

	CHECK(&vec.x == &vec.data()[0]);
//	CHECK(&vec.x == &vec.r);
//	CHECK(&vec.x == &vec.s);

	CHECK(&vec.y == &vec.data()[1]);
//	CHECK(&vec.y == &vec.g);
//	CHECK(&vec.y == &vec.t);

	CHECK(&vec.z == &vec.data()[2]);
//	CHECK(&vec.z == &vec.b);
//	CHECK(&vec.z == &vec.p);

	CHECK(&vec.w == &vec.data()[3]);
//	CHECK(&vec.w == &vec.a);
//	CHECK(&vec.w == &vec.q);

	CHECK((4 * sizeof (double)) == sizeof (vec));
}

TEST_CASE("dimMacro", "[libv.math.vec]") {
	libv::vec2ui veca2(1, 2);
	libv::vec3ui veca3(1, 2, 3);
	libv::vec4ui veca4(1, 2, 3, 4);
	libv::vec2ui vecb2(1, 2);
	libv::vec3ui vecb3(1, 2, 3);
	libv::vec4ui vecb4(1, 2, 3, 4);

	veca2 += vecb2;
	veca3 += vecb3;
	veca4 += vecb4;

	CHECK(veca2 == libv::vec2ul(2, 4));
	CHECK(veca3 == libv::vec3ul(2, 4, 6));
	CHECK(veca4 == libv::vec4ul(2, 4, 6, 8));
}

TEST_CASE("ctor", "[libv.math.vec]") {
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
}

TEST_CASE("nonMemberOperator", "[libv.math.vec]") {
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

TEST_CASE("normalize", "[libv.math.vec]") {
	libv::vec2d vec0(15, 15);

	CHECK(vec0 > 2);
	CHECK(vec0.normalize_copy() < 2);
	CHECK(2 < vec0);
	CHECK(2 > vec0.normalize_copy());

	CHECK(1 == libv::approx(vec0.normalize_copy().length()));
}

TEST_CASE("Custom getter functions", "[libv.math.vec]") {
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

TEST_CASE("Freestanding one dim getter functions", "[libv.math.vec]") {
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

TEST_CASE("Copy ctor from different type", "[libv.math.vec]") {
	struct Source {
		int value;
	};
	struct Target {
		int value;
		explicit Target(Source s) : value(s.value) {}
		bool operator==(Source s) const { return value == s.value; }
	};

	libv::vec4_t<Source> v0({1}, {2}, {3}, {4});
	libv::vec4_t<Target> v1(v0);

	CHECK(v1 == v0);
}

TEST_CASE("Non trivially destructible type", "[libv.math.vec]") {
	libv::vec_t<2, std::string> v0("x", "y");

	CHECK(v0[0] == "x");
	CHECK(v0[1] == "y");
}

TEST_CASE("operator=", "[libv.math.vec]") {
	struct Source {
		int value;
	};
	struct Target {
		int value;
		Target& operator=(Source s) { value = s.value; return *this; }
		bool operator==(Source s) const { return value == s.value; }
	};

	libv::vec_t<2, Source> v0({1}, {2});
	libv::vec_t<2, Source> v1({4}, {5});
	libv::vec_t<2, Target> v2({1}, {2});

	v0 = v1;
	v2 = v0;

	CHECK(v2 == libv::vec_t<2, Source>({4}, {5}));
}

TEST_CASE("copy / move assignment", "[libv.math.vec]") {
	using A = CtorCounter<class Tag>;

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

TEST_CASE("copy / move ctor", "[libv.math.vec]") {
	using A = CtorCounter<class Tag>;

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

TEST_CASE("copy / move assignment with non trivial payload", "[libv.math.vec]") {
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

TEST_CASE("copy / move ctor with non trivial payload", "[libv.math.vec]") {
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

TEST_CASE("operator*=", "[libv.math.vec]") {
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

TEST_CASE("length", "[libv.math.vec]") {
	libv::vec_t<2, float> v0(0, 10);
	libv::vec_t<2, float> v1(10, 10);

	CHECK(10.0f == libv::approx(v0.length()));
	CHECK(100.0f == libv::approx(v0.length_sq()));
	CHECK(14.14214f == libv::approx(v1.length()));
	CHECK(200.0f == libv::approx(v1.length_sq()));
}

TEST_CASE("operator*(vec, vec)", "[libv.math.vec]") {
	libv::vec_t<2, int> v0(1, 2);

	libv::vec_t<2, int> v1(1, 1);
	libv::vec_t<2, int> v2(4, 4);
	libv::vec_t<2, int> v3(2, 2);
	libv::vec_t<2, int> v4(3, 0);

	auto r0 = (v0 + v1);
	auto r1 = (v0 * v2);
	auto r2 = (v0 / v3);
	auto r3 = (v0 - v4);

	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r0)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r1)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r2)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r3)>, "Wrong result type.");

	CHECK((v0 + v1) == libv::vec2i(2, 3));
	CHECK((v0 * v2) == libv::vec2i(4, 8));
	CHECK((v0 / v3) == libv::vec2i(0, 1));
	CHECK((v0 - v4) == libv::vec2i(-2, 2));
}

TEST_CASE("operator*(vec, vec) with different types", "[libv.math.vec]") {
	libv::vec_t<2, int64_t> v0(1, 2);

	libv::vec_t<2, int32_t> v1(1, 1);
	libv::vec_t<2, int32_t> v2(4, 4);
	libv::vec_t<2, int32_t> v3(2, 2);
	libv::vec_t<2, int32_t> v4(3, 0);

	auto r0 = v0 + v1;
	auto r1 = v0 * v2;
	auto r2 = v0 / v3;
	auto r3 = v0 - v4;

	static_assert(std::is_same_v<libv::vec_t<2, int64_t>, decltype(r0)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int64_t>, decltype(r1)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int64_t>, decltype(r2)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int64_t>, decltype(r3)>, "Wrong result type.");

	CHECK(r0 == libv::vec2l(2, 3));
	CHECK(r1 == libv::vec2l(4, 8));
	CHECK(r2 == libv::vec2l(0, 1));
	CHECK(r3 == libv::vec2l(-2, 2));
}

TEST_CASE("operator*(vec, skalar)", "[libv.math.vec]") {
	libv::vec_t<2, int> v0(4, 8);

	int v1 = 2;
	int v2 = 8;

	auto r0 = (v0 * v1);
	auto r1 = (v0 / v2);
	auto r2 = (v1 * v0);
	auto r3 = (v2 / v0);

	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r0)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r1)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r2)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r3)>, "Wrong result type.");

	CHECK(r0 == libv::vec2i(8, 16));
	CHECK(r1 == libv::vec2i(0, 1));
	CHECK(r2 == libv::vec2i(8, 16));
	CHECK(r3 == libv::vec2i(2, 1));
}

TEST_CASE("operator*(vec, skalar) with different types", "[libv.math.vec]") {
	libv::vec_t<2, int32_t> v0(4, 8);

	int64_t v1 = 2;
	int64_t v2 = 8;

	auto r0 = (v0 * v1);
	auto r1 = (v0 / v2);
	auto r2 = (v1 * v0);
	auto r3 = (v2 / v0);

	static_assert(std::is_same_v<libv::vec_t<2, int64_t>, decltype(r0)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int64_t>, decltype(r1)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int64_t>, decltype(r2)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int64_t>, decltype(r3)>, "Wrong result type.");

	CHECK(r0 == libv::vec2l(8, 16));
	CHECK(r1 == libv::vec2l(0, 1));
	CHECK(r2 == libv::vec2l(8, 16));
	CHECK(r3 == libv::vec2l(2, 1));
}

TEST_CASE("operator+", "[libv.math.vec]") {
	libv::vec_t<2, int> v0(+4, +8);
	libv::vec_t<2, int> v1(-4, +8);
	libv::vec_t<2, int> v2(-4, -8);

	auto r00 = +v0;
	auto r01 = -v0;
	auto r10 = +v1;
	auto r11 = -v1;
	auto r20 = +v2;
	auto r21 = -v2;

	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r00)>, "Wrong result type.");
	static_assert(std::is_same_v<libv::vec_t<2, int>, decltype(r01)>, "Wrong result type.");

	CHECK(r00 == libv::vec2i(+4, +8));
	CHECK(r01 == libv::vec2i(-4, -8));
	CHECK(r10 == libv::vec2i(-4, +8));
	CHECK(r11 == libv::vec2i(+4, -8));
	CHECK(r20 == libv::vec2i(-4, -8));
	CHECK(r21 == libv::vec2i(+4, +8));
}

// =================================================================================================

// TODO P2: libv.math: Move formatter to the vec.hpp header
// TODO P2: libv.math: Implement vec_t top / nested level formatting, support recursion
// TODO P2: libv.math: Implement quat_t formatter
template <std::size_t N, typename T>
struct fmt::formatter<libv::vec_t<N, T>> : fmt::formatter<T> {
	template <typename FormatContext>
	auto format(const libv::vec_t<N, T>& vec, FormatContext& ctx) {
		auto out = ctx.out();

		for (std::size_t i = 0; i < N; ++i) {
			if (i != 0)
				out = fmt::format_to(out, " ");
			out = formatter<T>::format(vec[i], ctx);
			ctx.advance_to(out);
		}

		return out;
	}
};

TEST_CASE("Test math.vec formatter", "[libv.math.vec]") {
	libv::vec3f v0(42.0f, libv::pi, -0.3f);
	libv::vec3f v1(0, 1, 2);

	CHECK(fmt::format("{}", v0) == "42 3.1415927 -0.3");
	CHECK(fmt::format("{:07.3f}", v0) == "042.000 003.142 -00.300");
	CHECK(fmt::format("{:<6}", v0) == "42     3.1415927 -0.3  ");
	CHECK(fmt::format("{:^6}", v0) == "  42   3.1415927  -0.3 ");
	CHECK(fmt::format("{:>6}", v0) == "    42 3.1415927   -0.3");

	CHECK(fmt::format("{}", v1) == "0 1 2");

//	CHECK(fmt::format("{:,}", v0) == "42, 3.1415927, -0.3");
//	CHECK(fmt::format("{:,:07.3f}", v0) == "042.000, 003.142, -00.300");
//	CHECK(fmt::format("{:,:<6}", v0) == "42    , 3.1415927, -0.3  ");
//	CHECK(fmt::format("{:,:^6}", v0) == "  42  , 3.1415927,  -0.3 ");
//	CHECK(fmt::format("{:,:>6}", v0) == "    42, 3.1415927,   -0.3");
//
//	CHECK(fmt::format("{:,}", v1) == "0, 1, 2");
}
