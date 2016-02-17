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
inline void uniform(GLint location, const libv::gl::TextureType& value) {
	glUniform1i(location, static_cast<std::underlying_type<libv::gl::TextureType>::type> (value));
}

// Single
inline void uniform(GLint location, const bool& value) {
	glUniform1i(location, value);
}
inline void uniform(GLint location, const int& value) {
	glUniform1i(location, value);
}
inline void uniform(GLint location, const float& value) {
	glUniform1f(location, value);
}
inline void uniform(GLint location, const double& value) {
	glUniform1d(location, value);
}
// 2D Vector
inline void uniform(GLint location, const glm::ivec2& value) {
	glUniform2i(location, value.x, value.y);
}
inline void uniform(GLint location, const glm::fvec2& value) {
	glUniform2f(location, value.x, value.y);
}
inline void uniform(GLint location, const glm::dvec2& value) {
	glUniform2d(location, value.x, value.y);
}
// 3D Vector
inline void uniform(GLint location, const glm::ivec3& value) {
	glUniform3i(location, value.x, value.y, value.z);
}
inline void uniform(GLint location, const glm::fvec3& value) {
	glUniform3f(location, value.x, value.y, value.z);
}
inline void uniform(GLint location, const glm::dvec3& value) {
	glUniform3d(location, value.x, value.y, value.z);
}
// 4D Vector
inline void uniform(GLint location, const glm::ivec4& value) {
	glUniform4i(location, value.x, value.y, value.z, value.w);
}
inline void uniform(GLint location, const glm::fvec4& value) {
	glUniform4f(location, value.x, value.y, value.z, value.w);
}
inline void uniform(GLint location, const glm::dvec4& value) {
	glUniform4d(location, value.x, value.y, value.z, value.w);
}
// 3x3 Matrix
inline void uniform(GLint location, const glm::fmat3& value) {
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
inline void uniform(GLint location, const glm::dmat3& value) {
	glUniformMatrix3dv(location, 1, GL_FALSE, glm::value_ptr(value));
}
// 4x4 Matrix
inline void uniform(GLint location, const glm::fmat4& value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
inline void uniform(GLint location, const glm::dmat4& value) {
	glUniformMatrix4dv(location, 1, GL_FALSE, glm::value_ptr(value));
}

// Uniform -----------------------------------------------------------------------------------------

template <typename T>
class Uniform {
private:
	GLint location;
public:
	Uniform() = default;
	Uniform(const ShaderProgram& shaderProgram, const std::string& name) :
		location(shaderProgram.getActiveUniformLocation(name)) { }
	void assign(const ShaderProgram& shaderProgram, const std::string& name) {
		location = shaderProgram.getActiveUniformLocation(name);
	}
	inline void operator=(const T& val) {
		libv::gl::uniform(location, val);
	}
};

// -------------------------------------------------------------------------------------------------

using Uniform_texture = Uniform<libv::gl::TextureType>;

using Uniform_bool = Uniform<bool>;
using Uniform_int = Uniform<int>;
using Uniform_float = Uniform<float>;
using Uniform_double = Uniform<double>;

using Uniform_ivec2 = Uniform<glm::ivec2>;
using Uniform_fvec2 = Uniform<glm::fvec2>;
using Uniform_dvec2 = Uniform<glm::dvec2>;

using Uniform_ivec3 = Uniform<glm::ivec3>;
using Uniform_fvec3 = Uniform<glm::fvec3>;
using Uniform_dvec3 = Uniform<glm::dvec3>;

using Uniform_ivec4 = Uniform<glm::ivec4>;
using Uniform_fvec4 = Uniform<glm::fvec4>;
using Uniform_dvec4 = Uniform<glm::dvec4>;

using Uniform_fmat3 = Uniform<glm::fmat3>;
using Uniform_dmat3 = Uniform<glm::dmat3>;

using Uniform_fmat4 = Uniform<glm::fmat4>;
using Uniform_dmat4 = Uniform<glm::dmat4>;

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv
