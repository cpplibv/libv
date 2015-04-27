// File: Uniform.cpp, Created on 2014. december 19. 13:45, Author: Vader

#include "vl/gl/uniform.hpp"

namespace vl {
	namespace gl {

		// UniformMaterial -----------------------------------------------------
		// UniformLight --------------------------------------------------------

		UniformLight::UniformLight(const std::string & name) :
			type(name + ".type"),
			enabled(name + ".enabled"),
			position(name + ".position"),
			direction(name + ".direction"),
			diffuse(name + ".diffuse"),
			specular(name + ".specular"),
			range(name + ".range"),
			intensity(name + ".intensity"),
			innerCosAngle(name + ".innerCosAngle"),
			outerCosAngle(name + ".outerCosAngle"),
			shadowCast(name + ".shadowCast"),
			shadowMapSampler(name + ".shadowMapSampler"),
			shadowMVPTmat(name + ".shadowMVPTmat") { }

		void UniformLight::operator=(const gl::Light & v) {
			type = v.type;
			enabled = v.enabled;
			position = v.position;
			direction = glm::normalize(v.direction);
			diffuse = v.diffuse;
			specular = v.specular;
			range = v.range;
			intensity = v.intensity;
			outerCosAngle = v.outerCosAngle;
			innerCosAngle = v.innerCosAngle;
			shadowCast = v.shadowCast;
			shadowMapSampler = v.shadowMapSampler; //It can be commented?
			shadowMVPTmat = v.shadowMVPTmat;
		}
	} //namespace gl

	namespace glsl {
		gl::Uniform<bool> debugB0("debugB0");
		gl::Uniform<bool> debugB1("debugB1");
		gl::Uniform<int> debugI0("debugI0");
		gl::Uniform<int> debugI1("debugI1");
		gl::Uniform<float> debugF0("debugF0");
		gl::Uniform<float> debugF1("debugF1");
		gl::Uniform<double> debugD0("debugD0");
		gl::Uniform<double> debugD1("debugD1");
		gl::Uniform<glm::vec3> debugVec30("debugVec30");
		gl::Uniform<glm::vec3> debugVec31("debugVec31");
		gl::Uniform<glm::vec4> debugVec40("debugVec40");
		gl::Uniform<glm::vec4> debugVec41("debugVec41");
		gl::Uniform<glm::dvec3> debugDVec30("debugDVec30");
		gl::Uniform<glm::dvec3> debugDVec31("debugDVec31");
		gl::Uniform<glm::dvec4> debugDVec40("debugDVec40");
		gl::Uniform<glm::dvec4> debugDVec41("debugDVec41");

		gl::Uniform<glm::mat4> MVPmat("MVPmat");
		gl::Uniform<glm::mat4> Mmat("Mmat");
		gl::Uniform<glm::vec3> eyePosW("eyePosW");

		gl::Uniform<int> textureDiffuseSampler("textureDiffuseSampler");
		gl::Uniform<int> textureNormalSampler("textureNormalSampler");
		gl::Uniform<int> textureSpecularSampler("textureSpecularSampler");
		gl::Uniform<int> textureEmissionSampler("textureEmissionSampler");
		gl::Uniform<int> textureEnvironmentSampler("textureEnvironmentSampler");
		gl::Uniform<int> texturePassSampler("texturePassSampler");
		gl::Uniform<int> textureAmbientSampler("textureAmbientSampler");

		gl::Uniform<glm::vec4> blend0("blend0");
		gl::Uniform<glm::vec4> globalAmbient("globalAmbient");
		gl::Uniform<glm::vec4> clip("clip");
		gl::Uniform<int> numActiveLight("numActiveLight");
		gl::UniformLight light[8]{
			{ "light[0]"},
			{ "light[1]"},
			{ "light[2]"},
			{ "light[3]"},
			{ "light[4]"},
			{ "light[5]"},
			{ "light[6]"},
			{ "light[7]"}
		};
	} //namespace glsl
} //namespace vl