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

TEST_CASE("Vec2 concepts with vec_t") {
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

TEST_CASE("Vec3 concepts with vec_t") {
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

TEST_CASE("Vec4 concepts with vec_t") {
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

TEST_CASE("Vec2 concepts with glm") {
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

TEST_CASE("Vec3 concepts with glm") {
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

TEST_CASE("Vec4 concepts with glm") {
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

