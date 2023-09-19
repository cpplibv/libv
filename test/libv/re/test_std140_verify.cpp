// Project: libv.re, File: test/libv/re/test_std140_verify.cpp

#include <catch2/catch_test_macros.hpp>

#include <libv/re/uniform/std140_verify.hpp>
#include <libv/re/uniform/padding.hpp>

#include "std140_structs.hpp"


// -------------------------------------------------------------------------------------------------

namespace {

struct Test0Bad {
	template <typename Access> constexpr void access_uniforms(Access& access) {
		(void) access;
	}
};

struct Test0 {
	libv::re::Padding<16> p0;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(p0, "p0");
	}
};

struct Test1Bad {
	float m0;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
	}
};

struct Test1 {
	float m0;
	libv::re::Padding<12> p0;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(p0, "p0");
	}
};

struct Test2 {
	libv::vec3f m0;
	float m1;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(m1, "m1");
	}
};

struct Test3 {
	float m0;
	libv::vec3f m1;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(m1, "m1");
	}
};

struct Test4 {
	libv::vec3f m0;
	float m1;
	libv::mat4f m2;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(m1, "m1");
		access(m2, "m2");
	}
};

struct Test5Bad {
	libv::vec3f m0;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
	}
};

struct Test5 {
	libv::vec3f m0;
	libv::re::Padding<4> p0;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(p0, "p0");
	}
};

struct Test6Bad {
	libv::vec3f m0;
	libv::vec3f m1;
	float m2;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(m1, "m1");
		access(m2, "m2");
	}
};

struct Test6 {
	libv::vec3f m0;
	libv::re::Padding<4> p0;
	libv::vec3f m1;
	float m2;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(p0, "p0");
		access(m1, "m1");
		access(m2, "m2");
	}
};

struct Test7Bad {
	Test5 m0;
	float m1;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(m1, "m1");
	}
};

struct Test7 {
	Test5 m0;
	float m1;
	libv::re::Padding<12> p0;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(m1, "m1");
		access(p0, "p0");
	}
};

struct Test8Bad {
	float m0;
	libv::re::Padding<4> p0;
	float m1;
	float m2;
	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(m0, "m0");
		access(p0, "p0");
		access(m1, "m1");
		access(m2, "m2");
	}
};


} // namespace -------------------------------------------------------------------------------------

// TEST_CASE("libv.re: test", "[libv.re.std140]") {
// 	Test8Bad test;
// 	CHECK(!libv::re::verify_layout_std140_debug(test, 0, 0));
// }

static_assert(!libv::re::is_std140_packed<Test0Bad>);
static_assert(libv::re::is_std140_packed<Test0>);
static_assert(!libv::re::is_std140_packed<Test1Bad>);
static_assert(libv::re::is_std140_packed<Test1>);
static_assert(libv::re::is_std140_packed<Test2>);
static_assert(!libv::re::is_std140_packed<Test3>);
static_assert(libv::re::is_std140_packed<Test4>);
static_assert(!libv::re::is_std140_packed<Test5Bad>);
static_assert(libv::re::is_std140_packed<Test5>);
static_assert(!libv::re::is_std140_packed<Test6Bad>);
static_assert(libv::re::is_std140_packed<Test6>);
static_assert(!libv::re::is_std140_packed<Test7Bad>);
static_assert(libv::re::is_std140_packed<Test7>);
static_assert(!libv::re::is_std140_packed<Test8Bad>);



static_assert(libv::re::is_std140_packed<BlockLights>);
static_assert(libv::re::is_std140_packed<BlockArrays>);
static_assert(libv::re::is_std140_packed<BlockNested>);

// array<int, 4> is missing a padding between elements, need a custom array type for this to work properly
// static_assert(libv::re::is_std140_packed<BlockArraysInts>);
