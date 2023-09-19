// Project: libv.re, File: test/libv/re/test_std140_layout.cpp

#include <catch2/catch_test_macros.hpp>

#include <libv/meta/identity.hpp>
#include <libv/re/uniform/std140_layout.hpp>
#include <libv/re/uniform/uniform.hpp>


// // hpp
// #include <catch2/catch_test_macros.hpp>
// // libv
// #include <libv/math/mat.hpp>
// #include <libv/math/vec.hpp>
// #include <libv/meta/reflection_access.hpp>
// // std
// #include <array>
// // pro
// #include <libv/glr/layout_std140.hpp>
// #include <libv/glr/uniform.hpp>
// #include <libv/glr/uniform_block_binding.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

template <typename T> using array_of_3 = std::array<T, 3>;
template <typename T> using array_of_3_vec2 = std::array<libv::vec_t<2, T>, 3>;
template <typename T> using array_of_3_vec3 = std::array<libv::vec_t<3, T>, 3>;
template <typename T> using array_of_3_vec4 = std::array<libv::vec_t<4, T>, 3>;

template <template <typename> typename T>
struct UniformScalars {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformScalars";

	libv::re::Uniform<T<bool>> _00_bool__;
	libv::re::Uniform<T<int32_t>> _01_int32_;
	libv::re::Uniform<T<uint32_t>> _02_uint32;
	libv::re::Uniform<T<int64_t>> _03_int64_;
	libv::re::Uniform<T<uint64_t>> _04_uint64;
	libv::re::Uniform<T<float>> _05_float_;
	libv::re::Uniform<T<double>> _06_double;
	libv::re::Uniform<bool> _07_tail__;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_bool__, "_00_bool__");
		access(_01_int32_, "_01_int32_");
		access(_02_uint32, "_02_uint32");
		access(_03_int64_, "_03_int64_");
		access(_04_uint64, "_04_uint64");
		access(_05_float_, "_05_float_");
		access(_06_double, "_06_double");
		access(_07_tail__, "_07_tail__");
	}
};

template <template <typename> typename T>
struct UniformMatScalars {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformMatScalars";

	libv::re::Uniform<T<float>> _00_float_;
	libv::re::Uniform<T<double>> _01_double;
	libv::re::Uniform<bool> _02_tail__;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_float_, "_00_float_");
		access(_01_double, "_01_double");
		access(_02_tail__, "_02_tail__");
	}
};

struct UniformsSimple {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformsSimple";

	libv::re::Uniform<libv::mat4f> _00_mat4f;
	libv::re::Uniform<libv::mat4f> _01_mat4f;
	libv::re::Uniform<libv::vec4f> _02_vec4f;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_mat4f, "_00_mat4f");
		access(_01_mat4f, "_01_mat4f");
		access(_02_vec4f, "_02_vec4f");
	}
};

struct UniformsBasic {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformsBasic";

	libv::re::Uniform<float> _00_float_;
	libv::re::Uniform<libv::vec3f> _01_vec3f_;
	libv::re::Uniform<libv::mat4f> _02_mat4f_;
	libv::re::Uniform<std::array<float, 3>> _03_3float;
	libv::re::Uniform<bool> _04_bool__;
	libv::re::Uniform<int32_t> _05_int32_;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_float_, "_00_float_");
		access(_01_vec3f_, "_01_vec3f_");
		access(_02_mat4f_, "_02_mat4f_");
		access(_03_3float, "_03_3float");
		access(_04_bool__, "_04_bool__");
		access(_05_int32_, "_05_int32_");
	}
};

struct UniformsNested {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformsNested";

	libv::re::Uniform<float> _00_float_;
	libv::re::Uniform<libv::vec3f> _01_vec3f_;
	UniformsSimple _02_nested;
	libv::re::Uniform<int32_t> _03_int32_;
	libv::re::Uniform<bool> _04_tail__;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_float_, "_00_float_");
		access(_01_vec3f_, "_01_vec3f_");
		access(_02_nested, "_02_nested");
		access(_03_int32_, "_03_int32_");
		access(_04_tail__, "_04_tail__");
	}
};

struct UniformsNestedNested {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformsNestedNested";

	libv::re::Uniform<float> _00_float_;
	libv::re::Uniform<libv::vec3f> _01_vec3f_;
	UniformsNested _02_nested;
	libv::re::Uniform<int32_t> _03_int32_;
	libv::re::Uniform<bool> _04_tail__;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_float_, "_00_float_");
		access(_01_vec3f_, "_01_vec3f_");
		access(_02_nested, "_02_nested");
		access(_03_int32_, "_03_int32_");
		access(_04_tail__, "_04_tail__");
	}
};

struct UniformsComplexNestedA {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformsComplexNestedA";

	libv::re::Uniform<int32_t> _00_int32_;
	libv::re::Uniform<libv::vec2b> _01_vec2f_;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_int32_, "_00_int32_");
		access(_01_vec2f_, "_01_vec2f_");
	}
};

struct UniformsComplexNestedB {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformsComplexNestedB";

	libv::re::Uniform<libv::vec3ui> _00_vec3ui;
	libv::re::Uniform<libv::vec2f> _01_vec2f_;
	libv::re::Uniform<std::array<float, 2>> _02_2float;
	libv::re::Uniform<libv::vec2f> _03_vec2f_;
	libv::re::Uniform<std::array<libv::mat3f, 2>> _04_2mat2f;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_vec3ui, "_00_vec3ui");
		access(_01_vec2f_, "_01_vec2f_");
		access(_02_2float, "_02_2float");
		access(_03_vec2f_, "_03_vec2f_");
		access(_04_2mat2f, "_04_2mat2f");
	}
};

struct UniformsComplex {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformsComplex";

	libv::re::Uniform<float> _00_float_;
	libv::re::Uniform<libv::vec2f> _01_vec2f_;
	libv::re::Uniform<libv::vec3f> _02_vec3f_;
	UniformsComplexNestedA _03_nestA_;
	libv::re::Uniform<float> _04_float_;
	libv::re::Uniform<std::array<float, 2>> _05_2float;
	libv::re::Uniform<libv::mat2x3f> _06_mat2f_;
	std::array<UniformsComplexNestedB, 2> _07_2nestB;
	libv::re::Uniform<bool> _08_tail__;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_float_, "_00_float_");
		access(_01_vec2f_, "_01_vec2f_");
		access(_02_vec3f_, "_02_vec3f_");
		access(_03_nestA_, "_03_nestA_");
		access(_04_float_, "_04_float_");
		access(_05_2float, "_05_2float");
		access(_06_mat2f_, "_06_mat2f_");
		access(_07_2nestB, "_07_2nestB");
		access(_08_tail__, "_08_tail__");
	}
};

struct UniformsVec3AndFloat {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformsVec3AndFloat";

	libv::re::Uniform<libv::vec3f> _00_vec3f_;
	libv::re::Uniform<float> _01_float_;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_vec3f_, "_00_vec3f_");
		access(_01_float_, "_01_float_");
	}
};

struct UniformsArrayArrayNested {
	static constexpr libv::re::BlockBinding binding{0};
	static constexpr std::string_view name = "UniformsArrayArrayNested";

	std::array<std::array<UniformsComplexNestedA, 2>, 2> _00_array;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(_00_array, "_00_array");
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
libv::vec3ui test_info() {
	return {
			libv::re::layout_std140_align<T>(),
	        libv::re::layout_std140_size<T>(),
			libv::re::layout_std140_stride<T>()
	};
}

libv::vec3ui info(uint32_t align, uint32_t size, uint32_t stride) {
	return {align, size, stride};
}

} // namespace -------------------------------------------------------------------------------------

TEST_CASE("libv.re: STD140 info of scalar types", "[libv.re.std140]") {
	CHECK(test_info<bool>() == info(4, 4, 4));
	CHECK(test_info<int32_t>() == info(4, 4, 4));
	CHECK(test_info<uint32_t>() == info(4, 4, 4));
	CHECK(test_info<int64_t>() == info(8, 8, 8));
	CHECK(test_info<uint64_t>() == info(8, 8, 8));
	CHECK(test_info<float>() == info(4, 4, 4));
	CHECK(test_info<double>() == info(8, 8, 8));
}

TEST_CASE("libv.re: STD140 info of vector types", "[libv.re.std140]") {
	CHECK(test_info<libv::vec2b>() == info(8, 8, 8));
	CHECK(test_info<libv::vec3b>() == info(16, 12, 16));
	CHECK(test_info<libv::vec4b>() == info(16, 16, 16));

	CHECK(test_info<libv::vec2i>() == info(8, 8, 8));
	CHECK(test_info<libv::vec3i>() == info(16, 12, 16));
	CHECK(test_info<libv::vec4i>() == info(16, 16, 16));

	CHECK(test_info<libv::vec2ui>() == info(8, 8, 8));
	CHECK(test_info<libv::vec3ui>() == info(16, 12, 16));
	CHECK(test_info<libv::vec4ui>() == info(16, 16, 16));

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

TEST_CASE("libv.re: STD140 info of array types", "[libv.re.std140]") {
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

TEST_CASE("libv.re: STD140 info of matrix types", "[libv.re.std140]") {
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

TEST_CASE("libv.re: STD140 info of struct types", "[libv.re.std140]") {
	CHECK(test_info<UniformScalars<libv::meta::identity_t>>() == info(16, 64, 64));

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
	CHECK(test_info<UniformsVec3AndFloat>() == info(16, 16, 16));
	CHECK(test_info<UniformsArrayArrayNested>() == info(16, 64, 64));
}

TEST_CASE("libv.re: STD140 Layout scalar types", "[libv.re.std140]") {
	UniformScalars<libv::meta::identity_t> block;
	libv::re::layout_std140(block);

	CHECK(block._00_bool__.location.block.offset == 0);
	CHECK(block._01_int32_.location.block.offset == 4);
	CHECK(block._02_uint32.location.block.offset == 8);
	CHECK(block._03_int64_.location.block.offset == 16);
	CHECK(block._04_uint64.location.block.offset == 24);
	CHECK(block._05_float_.location.block.offset == 32);
	CHECK(block._06_double.location.block.offset == 40);
	CHECK(block._07_tail__.location.block.offset == 48);
}

TEST_CASE("libv.re: STD140 Layout vec2 types", "[libv.re.std140]") {
	UniformScalars<libv::vec2_t> block;
	libv::re::layout_std140(block);

	CHECK(block._00_bool__.location.block.offset == 0);
	CHECK(block._01_int32_.location.block.offset == 8);
	CHECK(block._02_uint32.location.block.offset == 16);
	CHECK(block._03_int64_.location.block.offset == 32);
	CHECK(block._04_uint64.location.block.offset == 48);
	CHECK(block._05_float_.location.block.offset == 64);
	CHECK(block._06_double.location.block.offset == 80);
	CHECK(block._07_tail__.location.block.offset == 96);
}

TEST_CASE("libv.re: STD140 Layout vec3 types", "[libv.re.std140]") {
	UniformScalars<libv::vec3_t> block;
	libv::re::layout_std140(block);

	CHECK(block._00_bool__.location.block.offset == 0);
	CHECK(block._01_int32_.location.block.offset == 16);
	CHECK(block._02_uint32.location.block.offset == 32);
	CHECK(block._03_int64_.location.block.offset == 64);
	CHECK(block._04_uint64.location.block.offset == 96);
	CHECK(block._05_float_.location.block.offset == 128);
	CHECK(block._06_double.location.block.offset == 160);
	CHECK(block._07_tail__.location.block.offset == 184);
}

TEST_CASE("libv.re: STD140 Layout vec4 types", "[libv.re.std140]") {
	UniformScalars<libv::vec4_t> block;
	libv::re::layout_std140(block);

	CHECK(block._00_bool__.location.block.offset == 0);
	CHECK(block._01_int32_.location.block.offset == 16);
	CHECK(block._02_uint32.location.block.offset == 32);
	CHECK(block._03_int64_.location.block.offset == 64);
	CHECK(block._04_uint64.location.block.offset == 96);
	CHECK(block._05_float_.location.block.offset == 128);
	CHECK(block._06_double.location.block.offset == 160);
	CHECK(block._07_tail__.location.block.offset == 192);
}

TEST_CASE("libv.re: STD140 Layout array 3 of scalar types", "[libv.re.std140]") {
	UniformScalars<array_of_3> block;
	libv::re::layout_std140(block);

	CHECK(block._00_bool__.location.block.offset == 0);
	CHECK(block._01_int32_.location.block.offset == 48);
	CHECK(block._02_uint32.location.block.offset == 96);
	CHECK(block._03_int64_.location.block.offset == 144);
	CHECK(block._04_uint64.location.block.offset == 192);
	CHECK(block._05_float_.location.block.offset == 240);
	CHECK(block._06_double.location.block.offset == 288);
	CHECK(block._07_tail__.location.block.offset == 336);
}

TEST_CASE("libv.re: STD140 Layout array 3 of vec2 types", "[libv.re.std140]") {
	UniformScalars<array_of_3_vec2> block;
	libv::re::layout_std140(block);

	CHECK(block._00_bool__.location.block.offset == 0);
	CHECK(block._01_int32_.location.block.offset == 48);
	CHECK(block._02_uint32.location.block.offset == 96);
	CHECK(block._03_int64_.location.block.offset == 144);
	CHECK(block._04_uint64.location.block.offset == 192);
	CHECK(block._05_float_.location.block.offset == 240);
	CHECK(block._06_double.location.block.offset == 288);
	CHECK(block._07_tail__.location.block.offset == 336);
}

TEST_CASE("libv.re: STD140 Layout array 3 of vec3 types", "[libv.re.std140]") {
	UniformScalars<array_of_3_vec3> block;
	libv::re::layout_std140(block);

	CHECK(block._00_bool__.location.block.offset == 0);
	CHECK(block._01_int32_.location.block.offset == 48);
	CHECK(block._02_uint32.location.block.offset == 96);
	CHECK(block._03_int64_.location.block.offset == 160);
	CHECK(block._04_uint64.location.block.offset == 256);
	CHECK(block._05_float_.location.block.offset == 352);
	CHECK(block._06_double.location.block.offset == 416);
	CHECK(block._07_tail__.location.block.offset == 512);
}

TEST_CASE("libv.re: STD140 Layout array 3 of vec4 types", "[libv.re.std140]") {
	UniformScalars<array_of_3_vec4> block;
	libv::re::layout_std140(block);

	CHECK(block._00_bool__.location.block.offset == 0);
	CHECK(block._01_int32_.location.block.offset == 48);
	CHECK(block._02_uint32.location.block.offset == 96);
	CHECK(block._03_int64_.location.block.offset == 160);
	CHECK(block._04_uint64.location.block.offset == 256);
	CHECK(block._05_float_.location.block.offset == 352);
	CHECK(block._06_double.location.block.offset == 416);
	CHECK(block._07_tail__.location.block.offset == 512);
}

TEST_CASE("libv.re: STD140 Layout mat2f types", "[libv.re.std140]") {
	UniformMatScalars<libv::mat2_t> block;
	libv::re::layout_std140(block);

	CHECK(block._00_float_.location.block.offset == 0);
	CHECK(block._01_double.location.block.offset == 32);
	CHECK(block._02_tail__.location.block.offset == 64);
}

TEST_CASE("libv.re: STD140 Layout mat3f types", "[libv.re.std140]") {
	UniformMatScalars<libv::mat3_t> block;
	libv::re::layout_std140(block);

	CHECK(block._00_float_.location.block.offset == 0);
	CHECK(block._01_double.location.block.offset == 64);
	CHECK(block._02_tail__.location.block.offset == 160);
}

TEST_CASE("libv.re: STD140 Layout mat4f types", "[libv.re.std140]") {
	UniformMatScalars<libv::mat4_t> block;
	libv::re::layout_std140(block);

	CHECK(block._00_float_.location.block.offset == 0);
	CHECK(block._01_double.location.block.offset == 64);
	CHECK(block._02_tail__.location.block.offset == 192);
}

TEST_CASE("libv.re: STD140 Layout UniformsSimple", "[libv.re.std140]") {
	UniformsSimple block;
	libv::re::layout_std140(block);

	CHECK(block._00_mat4f.location.block.offset == 0);
	CHECK(block._01_mat4f.location.block.offset == 64);
	CHECK(block._02_vec4f.location.block.offset == 128);
}

TEST_CASE("libv.re: STD140 Layout UniformsBasic", "[libv.re.std140]") {
	UniformsBasic block;
	libv::re::layout_std140(block);

	CHECK(block._00_float_.location.block.offset == 0);
	CHECK(block._01_vec3f_.location.block.offset == 16);
	CHECK(block._02_mat4f_.location.block.offset == 32);
	CHECK(block._03_3float.location.block.offset == 96);
	CHECK(block._04_bool__.location.block.offset == 144);
	CHECK(block._05_int32_.location.block.offset == 148);
}

TEST_CASE("libv.re: STD140 Layout UniformsNested", "[libv.re.std140]") {
	UniformsNested block;
	libv::re::layout_std140(block);

	CHECK(block._00_float_.location.block.offset == 0);
	CHECK(block._01_vec3f_.location.block.offset == 16);
	CHECK(block._02_nested._00_mat4f.location.block.offset == 32);
	CHECK(block._02_nested._01_mat4f.location.block.offset == 96);
	CHECK(block._02_nested._02_vec4f.location.block.offset == 160);
	CHECK(block._03_int32_.location.block.offset == 176);
	CHECK(block._04_tail__.location.block.offset == 180);
}

TEST_CASE("libv.re: STD140 Layout UniformsNestedNested", "[libv.re.std140]") {
	UniformsNestedNested block;
	libv::re::layout_std140(block);

	CHECK(block._00_float_.location.block.offset == 0);
	CHECK(block._01_vec3f_.location.block.offset == 16);
	CHECK(block._02_nested._00_float_.location.block.offset == 32);
	CHECK(block._02_nested._01_vec3f_.location.block.offset == 48);
	CHECK(block._02_nested._02_nested._00_mat4f.location.block.offset == 64);
	CHECK(block._02_nested._02_nested._01_mat4f.location.block.offset == 128);
	CHECK(block._02_nested._02_nested._02_vec4f.location.block.offset == 192);
	CHECK(block._02_nested._03_int32_.location.block.offset == 208);
	CHECK(block._02_nested._04_tail__.location.block.offset == 212);
	CHECK(block._03_int32_.location.block.offset == 224);
	CHECK(block._04_tail__.location.block.offset == 228);
}

TEST_CASE("libv.re: STD140 Layout UniformsComplex", "[libv.re.std140]") {
	UniformsComplex block;
	libv::re::layout_std140(block);

	CHECK(block._00_float_.location.block.offset == 0);
	CHECK(block._01_vec2f_.location.block.offset == 8);
	CHECK(block._02_vec3f_.location.block.offset == 16);
	CHECK(block._03_nestA_._00_int32_.location.block.offset == 32);
	CHECK(block._03_nestA_._01_vec2f_.location.block.offset == 40);
	CHECK(block._04_float_.location.block.offset == 48);
	CHECK(block._05_2float.location.block.offset == 64);
	CHECK(block._06_mat2f_.location.block.offset == 96);
	CHECK(block._07_2nestB[0]._00_vec3ui.location.block.offset == 128);
	CHECK(block._07_2nestB[0]._01_vec2f_.location.block.offset == 144);
	CHECK(block._07_2nestB[0]._02_2float.location.block.offset == 160);
	CHECK(block._07_2nestB[0]._03_vec2f_.location.block.offset == 192);
	CHECK(block._07_2nestB[0]._04_2mat2f.location.block.offset == 208);
	CHECK(block._07_2nestB[1]._00_vec3ui.location.block.offset == 304);
	CHECK(block._07_2nestB[1]._01_vec2f_.location.block.offset == 320);
	CHECK(block._07_2nestB[1]._02_2float.location.block.offset == 336);
	CHECK(block._07_2nestB[1]._03_vec2f_.location.block.offset == 368);
	CHECK(block._07_2nestB[1]._04_2mat2f.location.block.offset == 384);
	CHECK(block._08_tail__.location.block.offset == 480);
}

TEST_CASE("libv.re: STD140 Layout UniformsVec3AndFloat", "[libv.re.std140]") {
	UniformsVec3AndFloat block;
	libv::re::layout_std140(block);

	CHECK(block._00_vec3f_.location.block.offset == 0);
	CHECK(block._01_float_.location.block.offset == 12);
}

TEST_CASE("libv.re: STD140 Layout UniformsArrayArrayNested", "[libv.re.std140]") {
	UniformsArrayArrayNested block;
	libv::re::layout_std140(block);

	CHECK(block._00_array[0][0]._00_int32_.location.block.offset == 0);
	CHECK(block._00_array[0][0]._01_vec2f_.location.block.offset == 8);
	CHECK(block._00_array[0][1]._00_int32_.location.block.offset == 16);
	CHECK(block._00_array[0][1]._01_vec2f_.location.block.offset == 24);
	CHECK(block._00_array[1][0]._00_int32_.location.block.offset == 32);
	CHECK(block._00_array[1][0]._01_vec2f_.location.block.offset == 40);
	CHECK(block._00_array[1][1]._00_int32_.location.block.offset == 48);
	CHECK(block._00_array[1][1]._01_vec2f_.location.block.offset == 56);
}
