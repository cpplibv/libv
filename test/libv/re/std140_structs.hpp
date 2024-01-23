// Project: libv.re, File: test/libv/re/std140_structs.hpp

#include <libv/math/vec.hpp>
#include <libv/re/common/block_binding.hpp>
#include <libv/re/uniform/padding.hpp>

#include <string_view>


namespace {

using libv::re::Padding;
using libv::re::BlockBinding;


struct UniformLightDirectional {
	// Packed std140
	static constexpr std::string_view name = "LightDirectional300";

	libv::vec3f directionW;
	Padding<4> _p0;

	libv::vec3f radiance;
	Padding<4> _p1;

	// === 32 byte

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(directionW, "directionW");
		access(_p0, "_p0");
		access(radiance, "radiance");
		access(_p1, "_p1");
	}
};

struct BlockLights {
	// Packed std140
	static constexpr BlockBinding binding{0};
	static constexpr std::string_view name = "TestBlock";

	uint32_t numLightDirectionals = 0;
	uint32_t numLightPoints = 0;
	uint32_t numLightSpots = 0;
	Padding<4> _p0;
	std::array<UniformLightDirectional, 4> lightDirectionals;

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(numLightDirectionals, "numLightDirectionals");
		access(numLightPoints, "numLightPoints");
		access(numLightSpots, "numLightSpots");
		access(_p0, "_p0");
		access(lightDirectionals, "lightDirectionals");
	}
};

struct IntArrays {
	// Packed std140
	static constexpr std::string_view name = "IntArrays";

	uint32_t numInts = 0;
	Padding<12> _p0;
	std::array<int, 4> ints1D;
	std::array<std::array<int, 4>, 4> ints2D;
	std::array<std::array<std::array<int, 4>, 4>, 4> ints3D;

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(numInts, "numInts");
		access(_p0, "_p0");
		access(ints1D, "ints1D");
		access(ints2D, "ints2D");
		access(ints3D, "lights3D");
	}
};

struct BlockArrays {
	// Packed std140
	static constexpr BlockBinding binding{0};
	static constexpr std::string_view name = "TestBlock";

	uint32_t numLights = 0;
	Padding<12> _p0;
	std::array<UniformLightDirectional, 4> lights1D;
	std::array<std::array<UniformLightDirectional, 4>, 4> lights2D;
	std::array<std::array<std::array<UniformLightDirectional, 4>, 4>, 4> lights3D;

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(numLights, "numLights");
		access(_p0, "_p0");
		access(lights1D, "lights1D");
		access(lights2D, "lights2D");
		access(lights3D, "lights3D");
	}
};

struct BlockArraysInts {
	// Packed std140
	static constexpr BlockBinding binding{0};
	static constexpr std::string_view name = "TestBlock";

	uint32_t numIntStructs = 0;
	Padding<12> _p0;
	std::array<IntArrays, 4> intsStruct1D;
	std::array<std::array<IntArrays, 4>, 4> intsStruct2D;
	std::array<std::array<std::array<IntArrays, 4>, 4>, 4> intsStruct3D;

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(numIntStructs, "numIntStructs");
		access(_p0, "_p0");
		access(intsStruct1D, "intsStruct1D");
		access(intsStruct2D, "intsStruct2D");
		access(intsStruct3D, "lightsStruct3D");
	}
};

struct NestedA {
	// Packed std140
	static constexpr std::string_view name = "NestedA";

	int32_t value;
	Padding<12> _p0;

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(value, "value");
		access(_p0, "_p0");
	}
};

struct NestedB {
	// Packed std140
	static constexpr std::string_view name = "NestedB";

	NestedA nestedA0;
	NestedA nestedA1;

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(nestedA0, "nestedA0");
		access(nestedA1, "nestedA1");
	}
};

struct NestedC {
	// Packed std140
	static constexpr std::string_view name = "NestedC";

	NestedB nestedB0;
	NestedB nestedB1;

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(nestedB0, "nestedB0");
		access(nestedB1, "nestedB1");
	}
};

struct BlockNested {
	// Packed std140
	static constexpr BlockBinding binding{0};
	static constexpr std::string_view name = "TestBlock";

	NestedC nestedC0;
	NestedC nestedC1;

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(nestedC0, "nestedC0");
		access(nestedC1, "nestedC1");
	}
};

}
