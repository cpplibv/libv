// Project: libv.math, File: test/libv/math/test_vec_concept.cpp

// hpp
#include <catch/catch.hpp>
// ext
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
// std
#include <string>
// pro
#include <libv/math/vec_concept.hpp>
#include <libv/math/vec.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Vec concepts should reject non vector types", "[libv.math.vec]") {
	CHECK(not libv::Vec2<float>);
	CHECK(not libv::Vec2<float, float>);
	CHECK(not libv::Vec2<std::string>);
	CHECK(not libv::Vec2<std::string, float>);
	CHECK(not libv::Vec2<std::string, std::string>);

	CHECK(not libv::Vec3<float>);
	CHECK(not libv::Vec3<float, float>);
	CHECK(not libv::Vec3<std::string>);
	CHECK(not libv::Vec3<std::string, float>);
	CHECK(not libv::Vec3<std::string, std::string>);

	CHECK(not libv::Vec4<float>);
	CHECK(not libv::Vec4<float, float>);
	CHECK(not libv::Vec4<std::string>);
	CHECK(not libv::Vec4<std::string, float>);
	CHECK(not libv::Vec4<std::string, std::string>);
}

TEST_CASE("Vec concepts should reject non packed types", "[libv.math.vec]") {
	struct TestVec2 {
		float x, y, a;
	};
	struct TestVec3 {
		float x, y, z, a;
	};
	struct TestVec4 {
		float x, y, z, w, a;
	};

	CHECK(not libv::Vec2<TestVec2>);
	CHECK(not libv::Vec2<TestVec2, float>);
	CHECK(not libv::Vec3<TestVec2>);
	CHECK(not libv::Vec3<TestVec2, float>);
	CHECK(not libv::Vec4<TestVec2>);
	CHECK(not libv::Vec4<TestVec2, float>);

	CHECK(not libv::Vec2<TestVec3>);
	CHECK(not libv::Vec2<TestVec3, float>);
	CHECK(not libv::Vec3<TestVec3>);
	CHECK(not libv::Vec3<TestVec3, float>);
	CHECK(not libv::Vec4<TestVec3>);
	CHECK(not libv::Vec4<TestVec3, float>);

	CHECK(not libv::Vec2<TestVec4>);
	CHECK(not libv::Vec2<TestVec4, float>);
	CHECK(not libv::Vec3<TestVec4>);
	CHECK(not libv::Vec3<TestVec4, float>);
	CHECK(not libv::Vec4<TestVec4>);
	CHECK(not libv::Vec4<TestVec4, float>);
}

TEST_CASE("Vec concepts should reject out of order types", "[libv.math.vec]") {
	struct TestVec2 {
		float y, x;
	};
	struct TestVec3 {
		float x, z, y;
	};
	struct TestVec4 {
		float x, z, y, w;
	};

	CHECK(not libv::Vec2<TestVec2>);
	CHECK(not libv::Vec2<TestVec2, float>);
	CHECK(not libv::Vec3<TestVec2>);
	CHECK(not libv::Vec3<TestVec2, float>);
	CHECK(not libv::Vec4<TestVec2>);
	CHECK(not libv::Vec4<TestVec2, float>);

	CHECK(not libv::Vec2<TestVec3>);
	CHECK(not libv::Vec2<TestVec3, float>);
	CHECK(not libv::Vec3<TestVec3>);
	CHECK(not libv::Vec3<TestVec3, float>);
	CHECK(not libv::Vec4<TestVec3>);
	CHECK(not libv::Vec4<TestVec3, float>);

	CHECK(not libv::Vec2<TestVec4>);
	CHECK(not libv::Vec2<TestVec4, float>);
	CHECK(not libv::Vec3<TestVec4>);
	CHECK(not libv::Vec3<TestVec4, float>);
	CHECK(not libv::Vec4<TestVec4>);
	CHECK(not libv::Vec4<TestVec4, float>);
}

TEST_CASE("Vec2 concepts accepts vec2_t but rejects non 2 dimensional or non matching element types", "[libv.math.vec]") {
	CHECK(libv::Vec2<libv::vec2f>);
	CHECK(libv::Vec2<libv::vec2f, float>);
	CHECK(not libv::Vec2<libv::vec2f, double>);

	CHECK(not libv::Vec2<libv::vec3f>);
	CHECK(not libv::Vec2<libv::vec3f, float>);
	CHECK(not libv::Vec2<libv::vec3f, double>);

	CHECK(not libv::Vec2<libv::vec4f>);
	CHECK(not libv::Vec2<libv::vec4f, float>);
	CHECK(not libv::Vec2<libv::vec4f, double>);
}

TEST_CASE("Vec3 concepts accepts vec3_t but rejects non 3 dimensional or non matching element types", "[libv.math.vec]") {
	CHECK(not libv::Vec3<libv::vec2f>);
	CHECK(not libv::Vec3<libv::vec2f, float>);
	CHECK(not libv::Vec3<libv::vec2f, double>);

	CHECK(libv::Vec3<libv::vec3f>);
	CHECK(libv::Vec3<libv::vec3f, float>);
	CHECK(not libv::Vec3<libv::vec3f, double>);

	CHECK(not libv::Vec3<libv::vec4f>);
	CHECK(not libv::Vec3<libv::vec4f, float>);
	CHECK(not libv::Vec3<libv::vec4f, double>);
}

TEST_CASE("Vec4 concepts accepts vec4_t but rejects non 4 dimensional or non matching element types", "[libv.math.vec]") {
	CHECK(not libv::Vec4<libv::vec2f>);
	CHECK(not libv::Vec4<libv::vec2f, float>);
	CHECK(not libv::Vec4<libv::vec2f, double>);

	CHECK(not libv::Vec4<libv::vec3f>);
	CHECK(not libv::Vec4<libv::vec3f, float>);
	CHECK(not libv::Vec4<libv::vec3f, double>);

	CHECK(libv::Vec4<libv::vec4f>);
	CHECK(libv::Vec4<libv::vec4f, float>);
	CHECK(not libv::Vec4<libv::vec4f, double>);
}

TEST_CASE("Vec2 concepts accepts glm but rejects non 2 dimensional or non matching element types", "[libv.math.vec]") {
	CHECK(libv::Vec2<glm::vec2>);
	CHECK(libv::Vec2<glm::vec2, float>);
	CHECK(not libv::Vec2<glm::vec2, double>);

	CHECK(not libv::Vec2<glm::vec3>);
	CHECK(not libv::Vec2<glm::vec3, float>);
	CHECK(not libv::Vec2<glm::vec3, double>);

	CHECK(not libv::Vec2<glm::vec4>);
	CHECK(not libv::Vec2<glm::vec4, float>);
	CHECK(not libv::Vec2<glm::vec4, double>);
}

TEST_CASE("Vec3 concepts accepts glm but rejects non 3 dimensional or non matching element types", "[libv.math.vec]") {
	CHECK(not libv::Vec3<glm::vec2>);
	CHECK(not libv::Vec3<glm::vec2, float>);
	CHECK(not libv::Vec3<glm::vec2, double>);

	CHECK(libv::Vec3<glm::vec3>);
	CHECK(libv::Vec3<glm::vec3, float>);
	CHECK(not libv::Vec3<glm::vec3, double>);

	CHECK(not libv::Vec3<glm::vec4>);
	CHECK(not libv::Vec3<glm::vec4, float>);
	CHECK(not libv::Vec3<glm::vec4, double>);
}

TEST_CASE("Vec4 concepts accepts glm but rejects non 4 dimensional or non matching element types", "[libv.math.vec]") {
	CHECK(not libv::Vec4<glm::vec2>);
	CHECK(not libv::Vec4<glm::vec2, float>);
	CHECK(not libv::Vec4<glm::vec2, double>);

	CHECK(not libv::Vec4<glm::vec3>);
	CHECK(not libv::Vec4<glm::vec3, float>);
	CHECK(not libv::Vec4<glm::vec3, double>);

	CHECK(libv::Vec4<glm::vec4>);
	CHECK(libv::Vec4<glm::vec4, float>);
	CHECK(not libv::Vec4<glm::vec4, double>);
}

// -------------------------------------------------------------------------------------------------
