// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/meta/identity.hpp>
// std
#include <array>
// pro
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

template <typename T> using array_of_3 = std::array<T, 3>;
template <typename T> using array_of_3_vec2 = std::array<libv::vec_t<2, T>, 3>;
template <typename T> using array_of_3_vec3 = std::array<libv::vec_t<3, T>, 3>;
template <typename T> using array_of_3_vec4 = std::array<libv::vec_t<4, T>, 3>;

const auto uniformblock_test = libv::glr::UniformBlockBinding{0, "Test"};

template <template <typename> typename T>
struct UniformScalars {
	libv::glr::Uniform_t<T<bool>> _00_bool__;
	libv::glr::Uniform_t<T<int32_t>> _01_int32_;
	libv::glr::Uniform_t<T<uint32_t>> _02_uint32;
	libv::glr::Uniform_t<T<int64_t>> _03_int64_;
	libv::glr::Uniform_t<T<uint64_t>> _04_uint64;
	libv::glr::Uniform_t<T<float>> _05_float_;
	libv::glr::Uniform_t<T<double>> _06_double;
	libv::glr::Uniform_t<bool> _07_tail__;

	LIBV_REFLECTION_ACCESS(_00_bool__);
	LIBV_REFLECTION_ACCESS(_01_int32_);
	LIBV_REFLECTION_ACCESS(_02_uint32);
	LIBV_REFLECTION_ACCESS(_03_int64_);
	LIBV_REFLECTION_ACCESS(_04_uint64);
	LIBV_REFLECTION_ACCESS(_05_float_);
	LIBV_REFLECTION_ACCESS(_06_double);
	LIBV_REFLECTION_ACCESS(_07_tail__);
};

template <template <typename> typename T>
struct UniformMatScalars {
	libv::glr::Uniform_t<T<float>> _00_float_;
	libv::glr::Uniform_t<T<double>> _01_double;
	libv::glr::Uniform_t<bool> _02_tail__;

	LIBV_REFLECTION_ACCESS(_00_float_);
	LIBV_REFLECTION_ACCESS(_01_double);
	LIBV_REFLECTION_ACCESS(_02_tail__);
};

struct UniformsSimple {
	libv::glr::Uniform_t<libv::mat4f> _00_mat4f;
	libv::glr::Uniform_t<libv::mat4f> _01_mat4f;
	libv::glr::Uniform_t<libv::vec4f> _02_vec4f;

	LIBV_REFLECTION_ACCESS(_00_mat4f);
	LIBV_REFLECTION_ACCESS(_01_mat4f);
	LIBV_REFLECTION_ACCESS(_02_vec4f);
};

struct UniformsBasic {
	libv::glr::Uniform_t<float> _00_float_;
	libv::glr::Uniform_t<libv::vec3f> _01_vec3f_;
	libv::glr::Uniform_t<libv::mat4f> _02_mat4f_;
	libv::glr::Uniform_t<std::array<float, 3>> _03_3float;
	libv::glr::Uniform_t<bool> _04_bool__;
	libv::glr::Uniform_t<int32_t> _05_int32_;

	LIBV_REFLECTION_ACCESS(_00_float_);
	LIBV_REFLECTION_ACCESS(_01_vec3f_);
	LIBV_REFLECTION_ACCESS(_02_mat4f_);
	LIBV_REFLECTION_ACCESS(_03_3float);
	LIBV_REFLECTION_ACCESS(_04_bool__);
	LIBV_REFLECTION_ACCESS(_05_int32_);
};

struct UniformsNested {
	libv::glr::Uniform_t<float> _00_float_;
	libv::glr::Uniform_t<libv::vec3f> _01_vec3f_;
	UniformsSimple _02_nested;
	libv::glr::Uniform_t<int32_t> _03_int32_;
	libv::glr::Uniform_t<bool> _04_tail__;

	LIBV_REFLECTION_ACCESS(_00_float_);
	LIBV_REFLECTION_ACCESS(_01_vec3f_);
	LIBV_REFLECTION_ACCESS(_02_nested);
	LIBV_REFLECTION_ACCESS(_03_int32_);
	LIBV_REFLECTION_ACCESS(_04_tail__);
};

struct UniformsNestedNested {
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
	libv::glr::Uniform_t<int32_t> _00_int32_;
	libv::glr::Uniform_t<libv::vec2b> _01_vec2f_;

	LIBV_REFLECTION_ACCESS(_00_int32_);
	LIBV_REFLECTION_ACCESS(_01_vec2f_);
};

struct UniformsComplexNestedB {
	libv::glr::Uniform_t<libv::vec3ui> _00_vec3ui;
	libv::glr::Uniform_t<libv::vec2f> _01_vec2f_;
	libv::glr::Uniform_t<std::array<float, 2>> _02_2float;
	libv::glr::Uniform_t<libv::vec2f> _03_vec2f_;
	libv::glr::Uniform_t<std::array<libv::mat3f, 2>> _04_2mat2f;

	LIBV_REFLECTION_ACCESS(_00_vec3ui);
	LIBV_REFLECTION_ACCESS(_01_vec2f_);
	LIBV_REFLECTION_ACCESS(_02_2float);
	LIBV_REFLECTION_ACCESS(_03_vec2f_);
	LIBV_REFLECTION_ACCESS(_04_2mat2f);
};

struct UniformsComplex {
	libv::glr::Uniform_t<float> _00_float_;
	libv::glr::Uniform_t<libv::vec2f> _01_vec2f_;
	libv::glr::Uniform_t<libv::vec3f> _02_vec3f_;
	UniformsComplexNestedA _03_nestA_;
	libv::glr::Uniform_t<float> _04_float_;
	libv::glr::Uniform_t<std::array<float, 2>> _05_2float;
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

template <typename T>
libv::vec3u test_info() {
	return {libv::glr::layout_std140_align<T>(), libv::glr::layout_std140_size<T>(), libv::glr::layout_std140_stride<T>()};
}

libv::vec3u info(uint32_t align, uint32_t size, uint32_t stride) {
	return {align, size, stride};
}

} // namespace

TEST_CASE("Alignments for scalar types", "[libv.glr.std140]") {
	CHECK(test_info<bool>() == info(4, 4, 4));
	CHECK(test_info<int32_t>() == info(4, 4, 4));
	CHECK(test_info<uint32_t>() == info(4, 4, 4));
	CHECK(test_info<int64_t>() == info(8, 8, 8));
	CHECK(test_info<uint64_t>() == info(8, 8, 8));
	CHECK(test_info<float>() == info(4, 4, 4));
	CHECK(test_info<double>() == info(8, 8, 8));
}

TEST_CASE("Alignments for vector types", "[libv.glr.std140]") {
	CHECK(test_info<libv::vec2b>() == info(8, 8, 8));
	CHECK(test_info<libv::vec3b>() == info(16, 12, 16));
	CHECK(test_info<libv::vec4b>() == info(16, 16, 16));

	CHECK(test_info<libv::vec2i>() == info(8, 8, 8));
	CHECK(test_info<libv::vec3i>() == info(16, 12, 16));
	CHECK(test_info<libv::vec4i>() == info(16, 16, 16));

	CHECK(test_info<libv::vec2u>() == info(8, 8, 8));
	CHECK(test_info<libv::vec3u>() == info(16, 12, 16));
	CHECK(test_info<libv::vec4u>() == info(16, 16, 16));

	CHECK(test_info<libv::vec2l>() == info(16, 16, 16));
	CHECK(test_info<libv::vec3l>() == info(32, 24, 32));
	CHECK(test_info<libv::vec4l>() == info(32, 32, 32));

	CHECK(test_info<libv::vec2ul>() == info(16, 16, 16));
	CHECK(test_info<libv::vec3ul>() == info(32, 24, 32));
	CHECK(test_info<libv::vec4ul>() == info(32, 32, 32));

	CHECK(test_info<libv::vec2f>() == info(8, 8, 8));
	CHECK(test_info<libv::vec3f>() == info(16, 12, 16));
	CHECK(test_info<libv::vec4f>() == info(16, 16, 16));

	CHECK(test_info<libv::vec2d>() == info(16, 16, 16));
	CHECK(test_info<libv::vec3d>() == info(32, 24, 32));
	CHECK(test_info<libv::vec4d>() == info(32, 32, 32));
}

TEST_CASE("Alignments for array types", "[libv.glr.std140]") {
	CHECK(test_info<std::array<float,       1>>() == info(16, 16 * 1, 16 * 1));
	CHECK(test_info<std::array<double,      1>>() == info(16, 16 * 1, 16 * 1));
	CHECK(test_info<std::array<libv::vec2f, 1>>() == info(16, 16 * 1, 16 * 1));
	CHECK(test_info<std::array<libv::vec2d, 1>>() == info(16, 16 * 1, 16 * 1));
	CHECK(test_info<std::array<libv::vec3f, 1>>() == info(16, 16 * 1, 16 * 1));
	CHECK(test_info<std::array<libv::vec3d, 1>>() == info(32, 32 * 1, 32 * 1));
	CHECK(test_info<std::array<libv::vec4f, 1>>() == info(16, 16 * 1, 16 * 1));
	CHECK(test_info<std::array<libv::vec4d, 1>>() == info(32, 32 * 1, 32 * 1));

	CHECK(test_info<std::array<float,       2>>() == info(16, 16 * 2, 16 * 2));
	CHECK(test_info<std::array<double,      2>>() == info(16, 16 * 2, 16 * 2));
	CHECK(test_info<std::array<libv::vec2f, 2>>() == info(16, 16 * 2, 16 * 2));
	CHECK(test_info<std::array<libv::vec2d, 2>>() == info(16, 16 * 2, 16 * 2));
	CHECK(test_info<std::array<libv::vec3f, 2>>() == info(16, 16 * 2, 16 * 2));
	CHECK(test_info<std::array<libv::vec3d, 2>>() == info(32, 32 * 2, 32 * 2));
	CHECK(test_info<std::array<libv::vec4f, 2>>() == info(16, 16 * 2, 16 * 2));
	CHECK(test_info<std::array<libv::vec4d, 2>>() == info(32, 32 * 2, 32 * 2));

	CHECK(test_info<std::array<float,       3>>() == info(16, 16 * 3, 16 * 3));
	CHECK(test_info<std::array<double,      3>>() == info(16, 16 * 3, 16 * 3));
	CHECK(test_info<std::array<libv::vec2f, 3>>() == info(16, 16 * 3, 16 * 3));
	CHECK(test_info<std::array<libv::vec2d, 3>>() == info(16, 16 * 3, 16 * 3));
	CHECK(test_info<std::array<libv::vec3f, 3>>() == info(16, 16 * 3, 16 * 3));
	CHECK(test_info<std::array<libv::vec3d, 3>>() == info(32, 32 * 3, 32 * 3));
	CHECK(test_info<std::array<libv::vec4f, 3>>() == info(16, 16 * 3, 16 * 3));
	CHECK(test_info<std::array<libv::vec4d, 3>>() == info(32, 32 * 3, 32 * 3));

	CHECK(test_info<std::array<float,       4>>() == info(16, 16 * 4, 16 * 4));
	CHECK(test_info<std::array<double,      4>>() == info(16, 16 * 4, 16 * 4));
	CHECK(test_info<std::array<libv::vec2f, 4>>() == info(16, 16 * 4, 16 * 4));
	CHECK(test_info<std::array<libv::vec2d, 4>>() == info(16, 16 * 4, 16 * 4));
	CHECK(test_info<std::array<libv::vec3f, 4>>() == info(16, 16 * 4, 16 * 4));
	CHECK(test_info<std::array<libv::vec3d, 4>>() == info(32, 32 * 4, 32 * 4));
	CHECK(test_info<std::array<libv::vec4f, 4>>() == info(16, 16 * 4, 16 * 4));
	CHECK(test_info<std::array<libv::vec4d, 4>>() == info(32, 32 * 4, 32 * 4));

	CHECK(test_info<std::array<float,       5>>() == info(16, 16 * 5, 16 * 5));
	CHECK(test_info<std::array<double,      5>>() == info(16, 16 * 5, 16 * 5));
	CHECK(test_info<std::array<libv::vec2f, 5>>() == info(16, 16 * 5, 16 * 5));
	CHECK(test_info<std::array<libv::vec2d, 5>>() == info(16, 16 * 5, 16 * 5));
	CHECK(test_info<std::array<libv::vec3f, 5>>() == info(16, 16 * 5, 16 * 5));
	CHECK(test_info<std::array<libv::vec3d, 5>>() == info(32, 32 * 5, 32 * 5));
	CHECK(test_info<std::array<libv::vec4f, 5>>() == info(16, 16 * 5, 16 * 5));
	CHECK(test_info<std::array<libv::vec4d, 5>>() == info(32, 32 * 5, 32 * 5));
}

TEST_CASE("Alignments for matrix types", "[libv.glr.std140]") {
	CHECK(test_info<libv::mat2f>() == test_info<std::array<libv::vec2f, 2>>());
	CHECK(test_info<libv::mat3f>() == test_info<std::array<libv::vec3f, 3>>());
	CHECK(test_info<libv::mat4f>() == test_info<std::array<libv::vec4f, 4>>());
	CHECK(test_info<libv::mat2d>() == test_info<std::array<libv::vec2f, 2>>());
	CHECK(test_info<libv::mat3d>() == test_info<std::array<libv::vec3d, 3>>());
	CHECK(test_info<libv::mat4d>() == test_info<std::array<libv::vec4d, 4>>());
	CHECK(test_info<libv::mat2x3f>() == test_info<std::array<libv::vec3f, 2>>());
	CHECK(test_info<libv::mat2x4f>() == test_info<std::array<libv::vec4f, 2>>());
	CHECK(test_info<libv::mat3x2f>() == test_info<std::array<libv::vec2f, 3>>());
	CHECK(test_info<libv::mat3x4f>() == test_info<std::array<libv::vec4f, 3>>());
	CHECK(test_info<libv::mat4x2f>() == test_info<std::array<libv::vec2f, 4>>());
	CHECK(test_info<libv::mat4x3f>() == test_info<std::array<libv::vec3f, 4>>());
	CHECK(test_info<libv::mat2x3d>() == test_info<std::array<libv::vec3d, 2>>());
	CHECK(test_info<libv::mat2x4d>() == test_info<std::array<libv::vec4d, 2>>());
	CHECK(test_info<libv::mat3x2d>() == test_info<std::array<libv::vec2d, 3>>());
	CHECK(test_info<libv::mat3x4d>() == test_info<std::array<libv::vec4d, 3>>());
	CHECK(test_info<libv::mat4x2d>() == test_info<std::array<libv::vec2d, 4>>());
	CHECK(test_info<libv::mat4x3d>() == test_info<std::array<libv::vec3d, 4>>());
}

TEST_CASE("Alignments for struct types", "[libv.glr.std140]") {
	CHECK(test_info<UniformScalars<libv::meta::identity>>() == info(16, 64, 64));

	CHECK(test_info<UniformScalars<libv::vec2_t>>() == info(16, 112, 112));
	CHECK(test_info<UniformScalars<libv::vec3_t>>() == info(32, 192, 192));
	CHECK(test_info<UniformScalars<libv::vec4_t>>() == info(32, 224, 224));

	CHECK(test_info<UniformScalars<array_of_3>>() == info(16, 352, 352));
	CHECK(test_info<UniformScalars<array_of_3_vec2>>() == info(16, 352, 352));
	CHECK(test_info<UniformScalars<array_of_3_vec3>>() == info(32, 544, 544));
	CHECK(test_info<UniformScalars<array_of_3_vec4>>() == info(32, 544, 544));

	CHECK(test_info<UniformMatScalars<libv::mat2_t>>() == info(16, 80, 80));
	CHECK(test_info<UniformMatScalars<libv::mat3_t>>() == info(32, 192, 192));
	CHECK(test_info<UniformMatScalars<libv::mat4_t>>() == info(32, 224, 224));

	CHECK(test_info<UniformsSimple>() == info(16, 144, 144));
	CHECK(test_info<UniformsBasic>() == info(16, 160, 160));
	CHECK(test_info<UniformsNested>() == info(16, 192, 192));
	CHECK(test_info<UniformsNestedNested>() == info(16, 240, 240));
	CHECK(test_info<UniformsComplexNestedA>() == info(16, 16, 16));
	CHECK(test_info<UniformsComplexNestedB>() == info(16, 176, 176));
	CHECK(test_info<UniformsComplex>() == info(16, 496, 496));
}

TEST_CASE("Layout scalar types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformScalars<libv::meta::identity>>(uniformblock_test);

	CHECK(layout._00_bool__.location == 0);
	CHECK(layout._01_int32_.location == 4);
	CHECK(layout._02_uint32.location == 8);
	CHECK(layout._03_int64_.location == 16);
	CHECK(layout._04_uint64.location == 24);
	CHECK(layout._05_float_.location == 32);
	CHECK(layout._06_double.location == 40);
	CHECK(layout._07_tail__.location == 48);
}

TEST_CASE("Layout vec2 types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformScalars<libv::vec2_t>>(uniformblock_test);

	CHECK(layout._00_bool__.location == 0);
	CHECK(layout._01_int32_.location == 8);
	CHECK(layout._02_uint32.location == 16);
	CHECK(layout._03_int64_.location == 32);
	CHECK(layout._04_uint64.location == 48);
	CHECK(layout._05_float_.location == 64);
	CHECK(layout._06_double.location == 80);
	CHECK(layout._07_tail__.location == 96);
}

TEST_CASE("Layout vec3 types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformScalars<libv::vec3_t>>(uniformblock_test);

	CHECK(layout._00_bool__.location == 0);
	CHECK(layout._01_int32_.location == 16);
	CHECK(layout._02_uint32.location == 32);
	CHECK(layout._03_int64_.location == 64);
	CHECK(layout._04_uint64.location == 96);
	CHECK(layout._05_float_.location == 128);
	CHECK(layout._06_double.location == 160);
	CHECK(layout._07_tail__.location == 184);
}

TEST_CASE("Layout vec4 types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformScalars<libv::vec4_t>>(uniformblock_test);

	CHECK(layout._00_bool__.location == 0);
	CHECK(layout._01_int32_.location == 16);
	CHECK(layout._02_uint32.location == 32);
	CHECK(layout._03_int64_.location == 64);
	CHECK(layout._04_uint64.location == 96);
	CHECK(layout._05_float_.location == 128);
	CHECK(layout._06_double.location == 160);
	CHECK(layout._07_tail__.location == 192);
}

TEST_CASE("Layout array 3 of scalar types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformScalars<array_of_3>>(uniformblock_test);

	CHECK(layout._00_bool__.location == 0);
	CHECK(layout._01_int32_.location == 48);
	CHECK(layout._02_uint32.location == 96);
	CHECK(layout._03_int64_.location == 144);
	CHECK(layout._04_uint64.location == 192);
	CHECK(layout._05_float_.location == 240);
	CHECK(layout._06_double.location == 288);
	CHECK(layout._07_tail__.location == 336);
}

TEST_CASE("Layout array 3 of vec2 types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformScalars<array_of_3_vec2>>(uniformblock_test);

	CHECK(layout._00_bool__.location == 0);
	CHECK(layout._01_int32_.location == 48);
	CHECK(layout._02_uint32.location == 96);
	CHECK(layout._03_int64_.location == 144);
	CHECK(layout._04_uint64.location == 192);
	CHECK(layout._05_float_.location == 240);
	CHECK(layout._06_double.location == 288);
	CHECK(layout._07_tail__.location == 336);
}

TEST_CASE("Layout array 3 of vec3 types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformScalars<array_of_3_vec3>>(uniformblock_test);

	CHECK(layout._00_bool__.location == 0);
	CHECK(layout._01_int32_.location == 48);
	CHECK(layout._02_uint32.location == 96);
	CHECK(layout._03_int64_.location == 160);
	CHECK(layout._04_uint64.location == 256);
	CHECK(layout._05_float_.location == 352);
	CHECK(layout._06_double.location == 416);
	CHECK(layout._07_tail__.location == 512);
}

TEST_CASE("Layout array 3 of vec4 types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformScalars<array_of_3_vec4>>(uniformblock_test);

	CHECK(layout._00_bool__.location == 0);
	CHECK(layout._01_int32_.location == 48);
	CHECK(layout._02_uint32.location == 96);
	CHECK(layout._03_int64_.location == 160);
	CHECK(layout._04_uint64.location == 256);
	CHECK(layout._05_float_.location == 352);
	CHECK(layout._06_double.location == 416);
	CHECK(layout._07_tail__.location == 512);
}

TEST_CASE("Layout mat2f types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformMatScalars<libv::mat2_t>>(uniformblock_test);

	CHECK(layout._00_float_.location == 0);
	CHECK(layout._01_double.location == 32);
	CHECK(layout._02_tail__.location == 64);
}

TEST_CASE("Layout mat3f types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformMatScalars<libv::mat3_t>>(uniformblock_test);

	CHECK(layout._00_float_.location == 0);
	CHECK(layout._01_double.location == 64);
	CHECK(layout._02_tail__.location == 160);
}

TEST_CASE("Layout mat4f types", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformMatScalars<libv::mat4_t>>(uniformblock_test);

	CHECK(layout._00_float_.location == 0);
	CHECK(layout._01_double.location == 64);
	CHECK(layout._02_tail__.location == 192);
}

TEST_CASE("Layout UniformsSimple", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformsSimple>(uniformblock_test);

	CHECK(layout._00_mat4f.location == 0);
	CHECK(layout._01_mat4f.location == 64);
	CHECK(layout._02_vec4f.location == 128);
}

TEST_CASE("Layout UniformsBasic", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformsBasic>(uniformblock_test);

	CHECK(layout._00_float_.location == 0);
	CHECK(layout._01_vec3f_.location == 16);
	CHECK(layout._02_mat4f_.location == 32);
	CHECK(layout._03_3float.location == 96);
	CHECK(layout._04_bool__.location == 144);
	CHECK(layout._05_int32_.location == 148);
}

TEST_CASE("Layout UniformsNested", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformsNested>(uniformblock_test);
	CHECK(layout._00_float_.location == 0);
	CHECK(layout._01_vec3f_.location == 16);
	CHECK(layout._02_nested._00_mat4f.location == 32);
	CHECK(layout._02_nested._01_mat4f.location == 96);
	CHECK(layout._02_nested._02_vec4f.location == 160);
	CHECK(layout._03_int32_.location == 176);
	CHECK(layout._04_tail__.location == 180);
}

TEST_CASE("Layout UniformsNestedNested", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformsNestedNested>(uniformblock_test);
	CHECK(layout._00_float_.location == 0);
	CHECK(layout._01_vec3f_.location == 16);
	CHECK(layout._02_nested._00_float_.location == 32);
	CHECK(layout._02_nested._01_vec3f_.location == 48);
	CHECK(layout._02_nested._02_nested._00_mat4f.location == 64);
	CHECK(layout._02_nested._02_nested._01_mat4f.location == 128);
	CHECK(layout._02_nested._02_nested._02_vec4f.location == 192);
	CHECK(layout._02_nested._03_int32_.location == 208);
	CHECK(layout._02_nested._04_tail__.location == 212);
	CHECK(layout._03_int32_.location == 224);
	CHECK(layout._04_tail__.location == 228);
}

TEST_CASE("Layout UniformsComplex", "[libv.glr.std140]") {
	const auto layout = libv::glr::layout_std140<UniformsComplex>(uniformblock_test);

	CHECK(layout._00_float_.location == 0);
	CHECK(layout._01_vec2f_.location == 8);
	CHECK(layout._02_vec3f_.location == 16);
	CHECK(layout._03_nestA_._00_int32_.location == 32);
	CHECK(layout._03_nestA_._01_vec2f_.location == 40);
	CHECK(layout._04_float_.location == 48);
	CHECK(layout._05_2float.location == 64);
	CHECK(layout._06_mat2f_.location == 96);
	CHECK(layout._07_2nestB[0]._00_vec3ui.location == 128);
	CHECK(layout._07_2nestB[0]._01_vec2f_.location == 144);
	CHECK(layout._07_2nestB[0]._02_2float.location == 160);
	CHECK(layout._07_2nestB[0]._03_vec2f_.location == 192);
	CHECK(layout._07_2nestB[0]._04_2mat2f.location == 208);
	CHECK(layout._07_2nestB[1]._00_vec3ui.location == 304);
	CHECK(layout._07_2nestB[1]._01_vec2f_.location == 320);
	CHECK(layout._07_2nestB[1]._02_2float.location == 336);
	CHECK(layout._07_2nestB[1]._03_vec2f_.location == 368);
	CHECK(layout._07_2nestB[1]._04_2mat2f.location == 384);
	CHECK(layout._08_tail__.location == 480);
}
