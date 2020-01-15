// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
//#include <libv/glr/mesh.hpp>
//#include <libv/glr/program.hpp>
//#include <libv/glr/queue_fwd.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/math/vec.hpp>
// std
//#include <optional>
// pro
#include <vm4_viewer/scene/node.hpp>
//#include <vm4_viewer/camera.hpp>
//#include <vm4_viewer/scene/object.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

enum class LightType : int32_t {
	disable = 0,

	point = 1,
	directional = 2,
	spot = 3,
};

struct Light : Node {
	LightType type = LightType::point;

	libv::vec3f position = {0, 0, 0};
	libv::vec3f direction = {1, 0, 0};

	libv::vec3f ambient = {0.1f, 0.1f, 0.1f};
	libv::vec3f diffuse = {1, 1, 1};
	libv::vec3f specular = {1, 1, 1};

//	float range = 75.0;
	float innerCosAngle = 0.8f; // Angles closer to 1 produce tighter cones
	float outerCosAngle = 0.6f; // Angles of -1 will emulate point lights.

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

	libv::glr::Uniform_vec3f position;
	libv::glr::Uniform_vec3f direction;

	libv::glr::Uniform_vec3f ambient;
	libv::glr::Uniform_vec3f diffuse;
	libv::glr::Uniform_vec3f specular;

//	libv::glr::Uniform_float range;
	libv::glr::Uniform_float innerCosAngle;
	libv::glr::Uniform_float outerCosAngle;

//	libv::glr::Uniform_bool shadowCast;
//	libv::glr::Uniform_int32 shadowMapSampler;
//	libv::glr::Uniform_mat4f shadowMVPTmat;

//	UniformLight(const std::string& name);

	template <typename Access>
	void update_uniforms(Access& access) {
		access(type, "sun.type", libv::to_value(LightType::point));

		access(position, "sun.position", libv::vec3f(0, 0, 0));
		access(direction, "sun.direction", libv::vec3f(0, 0, -1));

		access(ambient, "sun.ambient", libv::vec3f(0.1f, 0.1f, 0.1f));
		access(diffuse, "sun.diffuse", libv::vec3f(1, 1, 1));
		access(specular, "sun.specular", libv::vec3f(1, 1, 1));

//		access(range, "sun.range", 100);
		access(innerCosAngle, "sun.innerCosAngle", 0.8f);
		access(outerCosAngle, "sun.outerCosAngle", 0.6f);

//		access(shadowCast, "sun.shadowCast", false);
//		access(shadowMapSampler, "sun.shadowMapSampler");
//		access(shadowMVPTmat, "sun.shadowMVPTmat");
	}

	void set(libv::glr::Queue& gl, const Light& v) {
		// TODO P3: gl::uniform and glr::uniform template overloads to accept any enum type and call to_value by itself
		gl.uniform(type, libv::to_value(v.type));

		gl.uniform(position, v.position);
		gl.uniform(direction, v.direction);

		gl.uniform(ambient, v.ambient);
		gl.uniform(diffuse, v.diffuse);
		gl.uniform(specular, v.specular);

		gl.uniform(innerCosAngle, v.innerCosAngle);
		gl.uniform(outerCosAngle, v.outerCosAngle);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
