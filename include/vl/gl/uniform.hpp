// File: Uniform.hpp, Created on 2014. december 19. 13:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
// pro
#include "vl/gl/light.hpp"
#include "vl/gl/shader.hpp"

namespace vl {
namespace gl {
// Uniform -------------------------------------------------------------

template <typename T>
class Uniform {
private:
	int index;
	std::string name;
public:
	Uniform(const std::string& name) :
		index(Shader::registerUniform(name)),
		name(name) { }
	void operator=(const T& val);
};

// Single
template <> inline void Uniform<bool>::operator=(const bool& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform1i(Shader::getUniformLocation(index), value);
}
template <> inline void Uniform<int>::operator=(const int& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform1i(Shader::getUniformLocation(index), value);
}
template <> inline void Uniform<float>::operator=(const float& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform1f(Shader::getUniformLocation(index), value);
}
template <> inline void Uniform<double>::operator=(const double& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform1d(Shader::getUniformLocation(index), value);
}
// 2D Vector
template <> inline void Uniform<glm::ivec2>::operator=(const glm::ivec2& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform2i(Shader::getUniformLocation(index), value.x, value.y);
}
template <> inline void Uniform<glm::vec2>::operator=(const glm::vec2& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform2f(Shader::getUniformLocation(index), value.x, value.y);
}
template <> inline void Uniform<glm::dvec2>::operator=(const glm::dvec2& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform2d(Shader::getUniformLocation(index), value.x, value.y);
}
// 3D Vector
template <> inline void Uniform<glm::ivec3>::operator=(const glm::ivec3& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform3i(Shader::getUniformLocation(index), value.x, value.y, value.z);
}
template <> inline void Uniform<glm::vec3>::operator=(const glm::vec3& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform3f(Shader::getUniformLocation(index), value.x, value.y, value.z);
}
template <> inline void Uniform<glm::dvec3>::operator=(const glm::dvec3& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform3d(Shader::getUniformLocation(index), value.x, value.y, value.z);
}
// 4D Vector
template <> inline void Uniform<glm::ivec4>::operator=(const glm::ivec4& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform4i(Shader::getUniformLocation(index), value.x, value.y, value.z, value.w);
}
template <> inline void Uniform<glm::vec4>::operator=(const glm::vec4& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform4f(Shader::getUniformLocation(index), value.x, value.y, value.z, value.w);
}
template <> inline void Uniform<glm::dvec4>::operator=(const glm::dvec4& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniform4d(Shader::getUniformLocation(index), value.x, value.y, value.z, value.w);
}
// 3x3 Matrix
template <> inline void Uniform<glm::mat3>::operator=(const glm::mat3& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniformMatrix3fv(Shader::getUniformLocation(index), 1, GL_FALSE, glm::value_ptr(value));
}
template <> inline void Uniform<glm::dmat3>::operator=(const glm::dmat3& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniformMatrix3dv(Shader::getUniformLocation(index), 1, GL_FALSE, glm::value_ptr(value));
}
// 4x4 Matrix
template <> inline void Uniform<glm::mat4>::operator=(const glm::mat4& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniformMatrix4fv(Shader::getUniformLocation(index), 1, GL_FALSE, glm::value_ptr(value));
}
template <> inline void Uniform<glm::dmat4>::operator=(const glm::dmat4& value) {
	//			if (Shader::getUniformLocation(index) >= 0)
	glUniformMatrix4dv(Shader::getUniformLocation(index), 1, GL_FALSE, glm::value_ptr(value));
}

struct UniformLight {
	Uniform<int> type;
	Uniform<bool> enabled;

	Uniform<glm::vec3> position;
	Uniform<glm::vec3> direction;
	Uniform<glm::vec4> diffuse;
	Uniform<glm::vec4> specular;

	Uniform<double> range;
	Uniform<double> intensity;
	Uniform<double> innerCosAngle;
	Uniform<double> outerCosAngle;

	Uniform<bool> shadowCast;
	Uniform<int> shadowMapSampler;
	Uniform<glm::mat4> shadowMVPTmat;

	UniformLight(const std::string& name);
	void operator=(const Light &v);
};
} //namespace gl

// GLSL --------------------------------------------------------------------

namespace glsl {

extern gl::Uniform<bool> debugB0;
extern gl::Uniform<bool> debugB1;
extern gl::Uniform<int> debugI0;
extern gl::Uniform<int> debugI1;
extern gl::Uniform<float> debugF0;
extern gl::Uniform<float> debugF1;
extern gl::Uniform<double> debugD0;
extern gl::Uniform<double> debugD1;
extern gl::Uniform<glm::vec3> debugVec30;
extern gl::Uniform<glm::vec3> debugVec31;
extern gl::Uniform<glm::vec4> debugVec40;
extern gl::Uniform<glm::vec4> debugVec41;
extern gl::Uniform<glm::dvec3> debugDVec30;
extern gl::Uniform<glm::dvec3> debugDVec31;
extern gl::Uniform<glm::dvec4> debugDVec40;
extern gl::Uniform<glm::dvec4> debugDVec41;

extern gl::Uniform<glm::mat4> MVPmat;
extern gl::Uniform<glm::mat4> Mmat;
extern gl::Uniform<glm::vec3> eyePosW;

extern gl::Uniform<int> textureDiffuseSampler;
extern gl::Uniform<int> textureNormalSampler;
extern gl::Uniform<int> textureSpecularSampler;
extern gl::Uniform<int> textureEmissionSampler;
extern gl::Uniform<int> textureEnvironmentSampler;
extern gl::Uniform<int> texturePassSampler;
extern gl::Uniform<int> textureAmbientSampler;

extern gl::Uniform<glm::vec4> blend0;
extern gl::Uniform<glm::vec4> globalAmbient;
extern gl::Uniform<glm::vec4> clip;
extern gl::Uniform<bool> opt;
extern gl::Uniform<int> numActiveLight;
extern gl::UniformLight light[8];

} //namespace glsl
} //namespace vl