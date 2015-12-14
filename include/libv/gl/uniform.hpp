// File: Uniform.hpp, Created on 2014. december 19. 13:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
// pro
#include <libv/gl/shader.hpp>
#include <libv/gl/gl.hpp>

namespace libv {
namespace gl {

// glUniform-------------------------------------------------------------------------------------

// Special
inline void glUniform(GLint location, const libv::gl::TextureType& value) {
	glUniform1i(location, static_cast<std::underlying_type<libv::gl::TextureType>::type>(value));
}

// Single
inline void glUniform(GLint location, const bool& value) {
	glUniform1i(location, value);
}
inline void glUniform(GLint location, const int& value) {
	glUniform1i(location, value);
}
inline void glUniform(GLint location, const float& value) {
	glUniform1f(location, value);
}
inline void glUniform(GLint location, const double& value) {
	glUniform1d(location, value);
}
// 2D Vector
inline void glUniform(GLint location, const glm::ivec2& value) {
	glUniform2i(location, value.x, value.y);
}
inline void glUniform(GLint location, const glm::vec2& value) {
	glUniform2f(location, value.x, value.y);
}
inline void glUniform(GLint location, const glm::dvec2& value) {
	glUniform2d(location, value.x, value.y);
}
// 3D Vector
inline void glUniform(GLint location, const glm::ivec3& value) {
	glUniform3i(location, value.x, value.y, value.z);
}
inline void glUniform(GLint location, const glm::vec3& value) {
	glUniform3f(location, value.x, value.y, value.z);
}
inline void glUniform(GLint location, const glm::dvec3& value) {
	glUniform3d(location, value.x, value.y, value.z);
}
// 4D Vector
inline void glUniform(GLint location, const glm::ivec4& value) {
	glUniform4i(location, value.x, value.y, value.z, value.w);
}
inline void glUniform(GLint location, const glm::vec4& value) {
	glUniform4f(location, value.x, value.y, value.z, value.w);
}
inline void glUniform(GLint location, const glm::dvec4& value) {
	glUniform4d(location, value.x, value.y, value.z, value.w);
}
// 3x3 Matrix
inline void glUniform(GLint location, const glm::mat3& value) {
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
inline void glUniform(GLint location, const glm::dmat3& value) {
	glUniformMatrix3dv(location, 1, GL_FALSE, glm::value_ptr(value));
}
// 4x4 Matrix
inline void glUniform(GLint location, const glm::mat4& value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
inline void glUniform(GLint location, const glm::dmat4& value) {
	glUniformMatrix4dv(location, 1, GL_FALSE, glm::value_ptr(value));
}

// Uniform -----------------------------------------------------------------------------------------

template <typename T>
class Uniform {
private:
	GLint location;
public:
	Uniform(const ShaderProgram& shaderProgram, const std::string& name) :
		location(shaderProgram.getActiveUniformLocation(name)) { }
	inline void operator=(const T& val) {
		libv::gl::glUniform(location, val);
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv
