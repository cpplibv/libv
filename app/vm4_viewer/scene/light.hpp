// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
//#include <libv/glr/mesh.hpp>
//#include <libv/glr/program.hpp>
//#include <libv/glr/queue_fwd.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/math/vec.hpp>
// std
//#include <optional>
// pro
//#include <vm4_viewer/camera.hpp>
#include <vm4_viewer/scene/object.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

enum class LightType : int32_t {
	point = 0,
	dir = 1,
	spot = 2,
};

struct Light : Node {
	LightType type = LightType::point;
//	bool enabled = true;

//	libv::vec3f position = {0, 0, 0};
	libv::vec3f direction = {1, 0, 0};
	libv::vec3f diffuse = {1, 1, 1};
	libv::vec3f specular = {1, 1, 1};

//	double range = 75.0;
//	double intensity = 1.0;
//	double innerCosAngle = 0.8; // Angles closer to 1 produce tighter cones
//	double outerCosAngle = 0.6; // Angles of -1 will emulate point lights.

//	bool shadowCast = false;
//	uint32_t frameBuffer;
//	uint32_t shadowDepthTexture; //Texture sampler for shadow map. The textureSamplers layout is 10+i where 'i' is the index of the light!
//	uint32_t shadowMapSampler;
//	libv::mat4f shadowMVPTmat; //MVPT mat

//	libv::mat4f getVmat();
//	libv::mat4f getPmat();
};

//libv::mat4f Light::getPmat() {
//	if (type == LightType::spot)
//		return libv::mat4f::perspective(std::acos(outerCosAngle) * 180.0f / libv::PI<float> * 2.0f, 1.0f, range / 15.0f, range);
//	else if (type == LightType::dir)
//		return libv::mat4f::ortho(-90, 90, -90, 90, -90, 90);
//	else if (type == LightType::point)
//		return libv::mat4f::ortho(-30, 30, -30, 30, -10, 150);
//	else
//		assert(false);
//}
//
//libv::mat4f Light::getVmat() {
//	if (type == LightType::spot)
//		return libv::mat4f::lookAt(position, position + direction, libv::vec3f(0, 1, 0));
//	else if (type == LightType::dir)
//		return libv::mat4f::lookAt(libv::vec3f(0, 0, 0), direction, libv::vec3f(0, 1, 0));
//	else if (type == LightType::point)
//		return libv::mat4f::ortho(-30, 30, -30, 30, -10, 150);
//	else
//		assert(false);
//}

struct UniformLight {
	libv::glr::Uniform_int32 type;
//	libv::glr::Uniform_bool enabled;

//	libv::glr::Uniform_vec3f position;
	libv::glr::Uniform_vec3f direction;
	libv::glr::Uniform_vec3f diffuse;
	libv::glr::Uniform_vec3f specular;

//	libv::glr::Uniform_double range;
//	libv::glr::Uniform_double intensity;
//	libv::glr::Uniform_double innerCosAngle;
//	libv::glr::Uniform_double outerCosAngle;

//	libv::glr::Uniform_bool shadowCast;
//	libv::glr::Uniform_int32 shadowMapSampler;
//	libv::glr::Uniform_mat4f shadowMVPTmat;

//	UniformLight(const std::string& name);
	void set(const Light& v) {

	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
