// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

// hpp
#include <catch/catch.hpp>
// ext
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
// std
#include <string>
// pro
#include <libv/math/vec.hpp>
#include <libv/math/vec_concept.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Vec concepts should reject non vector types") {
	CHECK((not libv::Vec2<float>::value));
	CHECK((not libv::Vec2<float, float>::value));
	CHECK((not libv::Vec2<std::string>::value));
	CHECK((not libv::Vec2<std::string, float>::value));
	CHECK((not libv::Vec2<std::string, std::string>::value));

	CHECK((not libv::Vec3<float>::value));
	CHECK((not libv::Vec3<float, float>::value));
	CHECK((not libv::Vec3<std::string>::value));
	CHECK((not libv::Vec3<std::string, float>::value));
	CHECK((not libv::Vec3<std::string, std::string>::value));

	CHECK((not libv::Vec4<float>::value));
	CHECK((not libv::Vec4<float, float>::value));
	CHECK((not libv::Vec4<std::string>::value));
	CHECK((not libv::Vec4<std::string, float>::value));
	CHECK((not libv::Vec4<std::string, std::string>::value));
}

TEST_CASE("Vec concepts should reject non packed types") {
	struct TestVec2 {
		float x, y, a;
	};
	struct TestVec3 {
		float x, y, z, a;
	};
	struct TestVec4 {
		float x, y, z, w, a;
	};

	CHECK((not libv::Vec2<TestVec2>::value));
	CHECK((not libv::Vec2<TestVec2, float>::value));
	CHECK((not libv::Vec3<TestVec2>::value));
	CHECK((not libv::Vec3<TestVec2, float>::value));
	CHECK((not libv::Vec4<TestVec2>::value));
	CHECK((not libv::Vec4<TestVec2, float>::value));

	CHECK((not libv::Vec2<TestVec3>::value));
	CHECK((not libv::Vec2<TestVec3, float>::value));
	CHECK((not libv::Vec3<TestVec3>::value));
	CHECK((not libv::Vec3<TestVec3, float>::value));
	CHECK((not libv::Vec4<TestVec3>::value));
	CHECK((not libv::Vec4<TestVec3, float>::value));

	CHECK((not libv::Vec2<TestVec4>::value));
	CHECK((not libv::Vec2<TestVec4, float>::value));
	CHECK((not libv::Vec3<TestVec4>::value));
	CHECK((not libv::Vec3<TestVec4, float>::value));
	CHECK((not libv::Vec4<TestVec4>::value));
	CHECK((not libv::Vec4<TestVec4, float>::value));
}

TEST_CASE("Vec concepts should reject out of order types") {
	struct TestVec2 {
		float y, x;
	};
	struct TestVec3 {
		float x, z, y;
	};
	struct TestVec4 {
		float x, z, y, w;
	};

	CHECK((not libv::Vec2<TestVec2>::value));
	CHECK((not libv::Vec2<TestVec2, float>::value));
	CHECK((not libv::Vec3<TestVec2>::value));
	CHECK((not libv::Vec3<TestVec2, float>::value));
	CHECK((not libv::Vec4<TestVec2>::value));
	CHECK((not libv::Vec4<TestVec2, float>::value));

	CHECK((not libv::Vec2<TestVec3>::value));
	CHECK((not libv::Vec2<TestVec3, float>::value));
	CHECK((not libv::Vec3<TestVec3>::value));
	CHECK((not libv::Vec3<TestVec3, float>::value));
	CHECK((not libv::Vec4<TestVec3>::value));
	CHECK((not libv::Vec4<TestVec3, float>::value));

	CHECK((not libv::Vec2<TestVec4>::value));
	CHECK((not libv::Vec2<TestVec4, float>::value));
	CHECK((not libv::Vec3<TestVec4>::value));
	CHECK((not libv::Vec3<TestVec4, float>::value));
	CHECK((not libv::Vec4<TestVec4>::value));
	CHECK((not libv::Vec4<TestVec4, float>::value));
}

TEST_CASE("Vec2 concepts accepts vec2_t but rejects non 2 dimensional or non matching element types") {
	CHECK((libv::Vec2<libv::vec2f>::value));
	CHECK((libv::Vec2<libv::vec2f, float>::value));
	CHECK((not libv::Vec2<libv::vec2f, double>::value));

	CHECK((not libv::Vec2<libv::vec3f>::value));
	CHECK((not libv::Vec2<libv::vec3f, float>::value));
	CHECK((not libv::Vec2<libv::vec3f, double>::value));

	CHECK((not libv::Vec2<libv::vec4f>::value));
	CHECK((not libv::Vec2<libv::vec4f, float>::value));
	CHECK((not libv::Vec2<libv::vec4f, double>::value));
}

TEST_CASE("Vec3 concepts accepts vec3_t but rejects non 3 dimensional or non matching element types") {
	CHECK((not libv::Vec3<libv::vec2f>::value));
	CHECK((not libv::Vec3<libv::vec2f, float>::value));
	CHECK((not libv::Vec3<libv::vec2f, double>::value));

	CHECK((libv::Vec3<libv::vec3f>::value));
	CHECK((libv::Vec3<libv::vec3f, float>::value));
	CHECK((not libv::Vec3<libv::vec3f, double>::value));

	CHECK((not libv::Vec3<libv::vec4f>::value));
	CHECK((not libv::Vec3<libv::vec4f, float>::value));
	CHECK((not libv::Vec3<libv::vec4f, double>::value));
}

TEST_CASE("Vec4 concepts accepts vec4_t but rejects non 4 dimensional or non matching element types") {
	CHECK((not libv::Vec4<libv::vec2f>::value));
	CHECK((not libv::Vec4<libv::vec2f, float>::value));
	CHECK((not libv::Vec4<libv::vec2f, double>::value));

	CHECK((not libv::Vec4<libv::vec3f>::value));
	CHECK((not libv::Vec4<libv::vec3f, float>::value));
	CHECK((not libv::Vec4<libv::vec3f, double>::value));

	CHECK((libv::Vec4<libv::vec4f>::value));
	CHECK((libv::Vec4<libv::vec4f, float>::value));
	CHECK((not libv::Vec4<libv::vec4f, double>::value));
}

TEST_CASE("Vec2 concepts accepts glm but rejects non 2 dimensional or non matching element types") {
	CHECK((libv::Vec2<glm::vec2>::value));
	CHECK((libv::Vec2<glm::vec2, float>::value));
	CHECK((not libv::Vec2<glm::vec2, double>::value));

	CHECK((not libv::Vec2<glm::vec3>::value));
	CHECK((not libv::Vec2<glm::vec3, float>::value));
	CHECK((not libv::Vec2<glm::vec3, double>::value));

	CHECK((not libv::Vec2<glm::vec4>::value));
	CHECK((not libv::Vec2<glm::vec4, float>::value));
	CHECK((not libv::Vec2<glm::vec4, double>::value));
}

TEST_CASE("Vec3 concepts accepts glm but rejects non 3 dimensional or non matching element types") {
	CHECK((not libv::Vec3<glm::vec2>::value));
	CHECK((not libv::Vec3<glm::vec2, float>::value));
	CHECK((not libv::Vec3<glm::vec2, double>::value));

	CHECK((libv::Vec3<glm::vec3>::value));
	CHECK((libv::Vec3<glm::vec3, float>::value));
	CHECK((not libv::Vec3<glm::vec3, double>::value));

	CHECK((not libv::Vec3<glm::vec4>::value));
	CHECK((not libv::Vec3<glm::vec4, float>::value));
	CHECK((not libv::Vec3<glm::vec4, double>::value));
}

TEST_CASE("Vec4 concepts accepts glm but rejects non 4 dimensional or non matching element types") {
	CHECK((not libv::Vec4<glm::vec2>::value));
	CHECK((not libv::Vec4<glm::vec2, float>::value));
	CHECK((not libv::Vec4<glm::vec2, double>::value));

	CHECK((not libv::Vec4<glm::vec3>::value));
	CHECK((not libv::Vec4<glm::vec3, float>::value));
	CHECK((not libv::Vec4<glm::vec3, double>::value));

	CHECK((libv::Vec4<glm::vec4>::value));
	CHECK((libv::Vec4<glm::vec4, float>::value));
	CHECK((not libv::Vec4<glm::vec4, double>::value));
}

// -------------------------------------------------------------------------------------------------

