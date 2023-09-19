// Project: libv.re, File: test/libv/re/test_std140_verify.cpp

#include <catch2/catch_test_macros.hpp>

#include <libv/re/uniform/padding.hpp>
#include <libv/re/uniform/std140_string.hpp>

#include "std140_structs.hpp"

// -------------------------------------------------------------------------------------------------

TEST_CASE("Layout to string", "[libv.re.string_std140]") {
	CHECK(libv::re::string_std140<BlockLights>("TestBlock") ==
R"(struct LightDirectional300 {
	vec3 directionW;
	vec3 radiance;
};

layout(std140) uniform TestBlock {
	uint numLightDirectionals;
	uint numLightPoints;
	uint numLightSpots;
	LightDirectional300 lightDirectionals[4];
};
)");

	CHECK(libv::re::string_std140<BlockArrays>("TestBlock") ==
R"(struct LightDirectional300 {
	vec3 directionW;
	vec3 radiance;
};

layout(std140) uniform TestBlock {
	uint numLights;
	LightDirectional300 lights1D[4];
	LightDirectional300 lights2D[4][4];
	LightDirectional300 lights3D[4][4][4];
};
)");

	CHECK(libv::re::string_std140<BlockArraysInts>("TestBlock") ==
R"(struct IntArrays {
	uint numInts;
	int ints1D[4];
	int ints2D[4][4];
	int lights3D[4][4][4];
};

layout(std140) uniform TestBlock {
	uint numIntStructs;
	IntArrays intsStruct1D[4];
	IntArrays intsStruct2D[4][4];
	IntArrays lightsStruct3D[4][4][4];
};
)");

	CHECK(libv::re::string_std140<BlockNested>("TestBlock") ==
R"(struct NestedA {
	int value;
};

struct NestedB {
	NestedA nestedA0;
	NestedA nestedA1;
};

struct NestedC {
	NestedB nestedB0;
	NestedB nestedB1;
};

layout(std140) uniform TestBlock {
	NestedC nestedC0;
	NestedC nestedC1;
};
)");
}
