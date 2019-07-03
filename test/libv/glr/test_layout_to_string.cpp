// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/reflection_access.hpp>
// std
#include <array>
// pro
#include <libv/glr/layout_to_string.hpp>
#include <libv/glr/uniform.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

struct UniformsSimple {
	static inline auto name() { return "UniformsSimple"; }

	libv::glr::Uniform_t<libv::mat4f> _00_mat4f;
	libv::glr::Uniform_t<libv::mat4f> _01_mat4f;
	libv::glr::Uniform_t<libv::vec4f> _02_vec4f;

	LIBV_REFLECTION_ACCESS(_00_mat4f);
	LIBV_REFLECTION_ACCESS(_01_mat4f);
	LIBV_REFLECTION_ACCESS(_02_vec4f);
};

struct UniformsNested {
	static inline auto name() { return "UniformsNested"; }

	libv::glr::Uniform_t<float> _00_float_;
	libv::glr::Uniform_t<libv::vec3f> _01_vec3f_;
	UniformsSimple _02_nested;
	UniformsSimple _03_nested;
	libv::glr::Uniform_t<bool> _04_tail__;

	LIBV_REFLECTION_ACCESS(_00_float_);
	LIBV_REFLECTION_ACCESS(_01_vec3f_);
	LIBV_REFLECTION_ACCESS(_02_nested);
	LIBV_REFLECTION_ACCESS(_03_nested);
	LIBV_REFLECTION_ACCESS(_04_tail__);
};

struct UniformsNestedNested {
	static inline auto name() { return "UniformsNestedNested"; }

	libv::glr::Uniform_t<float> _00_float_;
	libv::glr::Uniform_t<libv::vec3f> _01_vec3f_;
	UniformsNested _02_nested;
	libv::glr::Uniform_t<int32_t> _03_int32_;
	libv::glr::Uniform_t<bool> _04_tail__;

	LIBV_REFLECTION_ACCESS(_00_float_);
	LIBV_REFLECTION_ACCESS(_01_vec3f_);
	LIBV_REFLECTION_ACCESS(_02_nested);
	LIBV_REFLECTION_ACCESS(_03_int32_);
	LIBV_REFLECTION_ACCESS(_04_tail__);
};

struct UniformsComplexNestedA {
	static inline auto name() { return "UniformsComplexNestedA"; }

	libv::glr::Uniform_t<int32_t> _00_int32_;
	libv::glr::Uniform_t<libv::vec2b> _01_vec2f_;

	LIBV_REFLECTION_ACCESS(_00_int32_);
	LIBV_REFLECTION_ACCESS(_01_vec2f_);
};

struct UniformsComplexNestedB {
	static inline auto name() { return "UniformsComplexNestedB"; }

	libv::glr::Uniform_t<libv::vec3ui> _00_vec3ui;
	libv::glr::Uniform_t<libv::vec2f> _01_vec2f_;
	libv::glr::Uniform_t<std::array<float, 2 >> _02_2float;
	libv::glr::Uniform_t<libv::vec2f> _03_vec2f_;
	libv::glr::Uniform_t<std::array<libv::mat3f, 2 >> _04_2mat2f;

	LIBV_REFLECTION_ACCESS(_00_vec3ui);
	LIBV_REFLECTION_ACCESS(_01_vec2f_);
	LIBV_REFLECTION_ACCESS(_02_2float);
	LIBV_REFLECTION_ACCESS(_03_vec2f_);
	LIBV_REFLECTION_ACCESS(_04_2mat2f);
};

struct UniformsComplex {
	static inline auto name() { return "UniformsComplex"; }

	libv::glr::Uniform_t<float> _00_float_;
	libv::glr::Uniform_t<libv::vec2f> _01_vec2f_;
	libv::glr::Uniform_t<libv::vec3f> _02_vec3f_;
	UniformsComplexNestedA _03_nestA_;
	libv::glr::Uniform_t<float> _04_float_;
	libv::glr::Uniform_t<std::array<float, 2 >> _05_2float;
	libv::glr::Uniform_t<libv::mat2x3f> _06_mat2f_;
	std::array<UniformsComplexNestedB, 2> _07_2nestB;
	libv::glr::Uniform_t<bool> _08_tail__;

	LIBV_REFLECTION_ACCESS(_00_float_);
	LIBV_REFLECTION_ACCESS(_01_vec2f_);
	LIBV_REFLECTION_ACCESS(_02_vec3f_);
	LIBV_REFLECTION_ACCESS(_03_nestA_);
	LIBV_REFLECTION_ACCESS(_04_float_);
	LIBV_REFLECTION_ACCESS(_05_2float);
	LIBV_REFLECTION_ACCESS(_06_mat2f_);
	LIBV_REFLECTION_ACCESS(_07_2nestB);
	LIBV_REFLECTION_ACCESS(_08_tail__);
};

} // namespace

TEST_CASE("Layout to string UniformsSimple", "[libv.glr.to_string]") {
	const auto layout = libv::glr::layout_to_string<UniformsSimple>("TestBlock");

	const auto expected =
R"(layout(std140) uniform TestBlock {
	mat4 _00_mat4f;
	mat4 _01_mat4f;
	vec4 _02_vec4f;
};
)";

	CHECK(layout == expected);
}

TEST_CASE("Layout to string UniformsNestedNested", "[libv.glr.to_string]") {
	const auto layout = libv::glr::layout_to_string<UniformsNestedNested>("TestBlock");

	const auto expected =
R"(struct UniformsSimple {
	mat4 _00_mat4f;
	mat4 _01_mat4f;
	vec4 _02_vec4f;
};

struct UniformsNested {
	float _00_float_;
	vec3 _01_vec3f_;
	UniformsSimple _02_nested;
	UniformsSimple _03_nested;
	bool _04_tail__;
};

layout(std140) uniform TestBlock {
	float _00_float_;
	vec3 _01_vec3f_;
	UniformsNested _02_nested;
	int _03_int32_;
	bool _04_tail__;
};
)";

	CHECK(layout == expected);
}

TEST_CASE("Layout to string UniformsComplex", "[libv.glr.to_string]") {
	const auto layout = libv::glr::layout_to_string<UniformsComplex>("TestBlock");

	const auto expected =
R"(struct UniformsComplexNestedA {
	int _00_int32_;
	vec2b _01_vec2f_;
};

struct UniformsComplexNestedB {
	vec3u _00_vec3ui;
	vec2 _01_vec2f_;
	float _02_2float[2];
	vec2 _03_vec2f_;
	mat3 _04_2mat2f[2];
};

layout(std140) uniform TestBlock {
	float _00_float_;
	vec2 _01_vec2f_;
	vec3 _02_vec3f_;
	UniformsComplexNestedA _03_nestA_;
	float _04_float_;
	float _05_2float[2];
	mat2x3 _06_mat2f_;
	UniformsComplexNestedB _07_2nestB[2];
	bool _08_tail__;
};
)";

	CHECK(layout == expected);
}
