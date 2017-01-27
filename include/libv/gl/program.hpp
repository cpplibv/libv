// File:   program.hpp Author: Vader Created on 2016. július 8., 12:02

#pragma once

// ext
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
// libv
#include <libv/vec.hpp>
// std
#include <string>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/shader.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class Shader;

template <typename T>
class Uniform;

template <typename T>
class BaseAttribute;

// Program -------------------------------------------------------------------------------------

class Program {
private:
	GLuint programID = 0;

public:
	inline Program() { }
	inline Program(const Program&) = delete;
	inline Program(Program&& orig) noexcept : programID(orig.programID) {
		orig.programID = 0;
	}
	inline ~Program() {
		LIBV_GL_DEBUG_ASSERT(programID == 0);
	}

public:
	inline bool status() const {
		LIBV_GL_DEBUG_ASSERT(programID != 0);
		GLint result;
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		LIBV_GL_DEBUG_CHECK();
		return result;
	}
	inline std::string info() const {
		LIBV_GL_DEBUG_ASSERT(programID != 0);
		int infoLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);

		std::string result;
		result.resize(infoLength);
		glGetProgramInfoLog(programID, infoLength, nullptr, &result[0]);
		LIBV_GL_DEBUG_CHECK();
		return result;
	}
	inline void create() {
		LIBV_GL_DEBUG_ASSERT(programID == 0);
		programID = glCreateProgram();
		LIBV_GL_DEBUG_CHECK();
		if (programID == 0)
			LIBV_LOG_GL_ERROR("Failed to create program");
	}
	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(programID != 0);
		glDeleteProgram(programID);
		programID = 0;
		LIBV_GL_DEBUG_CHECK();
	}
	inline void link(const Shader& vertex, const Shader& fragment) {
		LIBV_GL_DEBUG_ASSERT(programID != 0);
		glAttachShader(programID, vertex);
		glAttachShader(programID, fragment);
		LIBV_GL_DEBUG_CHECK();
		glLinkProgram(programID);
		LIBV_GL_DEBUG_CHECK();
		if (!status())
			LIBV_LOG_GL_ERROR("Failed to link program:\n%s", info());
	}
	inline void link(const Shader& vertex, const Shader& fragment, const Shader& geometry) {
		LIBV_GL_DEBUG_ASSERT(programID != 0);
		glAttachShader(programID, vertex);
		glAttachShader(programID, fragment);
		glAttachShader(programID, geometry);
		LIBV_GL_DEBUG_CHECK();
		glLinkProgram(programID);
		LIBV_GL_DEBUG_CHECK();
		if (!status())
			LIBV_LOG_GL_ERROR("Failed to link program:\n%s", info());
	}
	inline void createLink(const Shader& vertex, const Shader& fragment) {
		create();
		link(vertex, fragment);
	}
	inline void createLink(const Shader& vertex, const Shader& fragment, const Shader& geometry) {
		create();
		link(vertex, fragment, geometry);
	}
	inline void use() const {
		LIBV_GL_DEBUG_ASSERT(programID != 0);
		glUseProgram(programID);
		LIBV_GL_DEBUG_CHECK();
	}

public:
	template <typename T> inline void assign(BaseAttribute<T>& attribute, const char* name) {
		attribute.assign(*this, name);
	}
	template <typename T> inline void assign(BaseAttribute<T>& attribute, const std::string& name) {
		attribute.assign(*this, name.c_str());
	}
	template <typename T> inline void assign(Uniform<T>& uniform, const char* name) {
		uniform.assign(*this, name);
	}
	template <typename T> inline void assign(Uniform<T>& uniform, const std::string& name) {
		uniform.assign(*this, name.c_str());
	}

public:
	inline auto id() const {
		return programID;
	}
	inline operator GLuint() const {
		return programID;
	}
};

// Guarded -----------------------------------------------------------------------------------------

struct GuardedProgram : Program {
	inline GuardedProgram() {
		create();
	}
	inline GuardedProgram(const Shader& vertex, const Shader& fragment) {
		createLink(vertex, fragment);
	}
	inline GuardedProgram(const Shader& vertex, const Shader& fragment, const Shader& geometry) {
		createLink(vertex, fragment, geometry);
	}
	inline GuardedProgram(const Program&) = delete;
	inline GuardedProgram(const GuardedProgram&) = delete;
	inline GuardedProgram(Program&& orig) : Program(std::move(orig)) {}
	inline GuardedProgram(GuardedProgram&& orig) : Program(std::move(orig)) {}
	~GuardedProgram() {
		if (id())
			destroy();
	}
};

//
//
// =================================================================================================
//
//

// glUniform-------------------------------------------------------------------------------------

// Special
inline void uniform(GLint location, const libv::gl::TextureChannel& value) {
	glUniform1i(location, to_value(value));
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
	inline void assign(const Program& program, const char* name) {
		LIBV_GL_DEBUG_ASSERT(program.id() != 0);
		location = glGetUniformLocation(program, name);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void assign(const Program& program, const std::string& name) {
		assign(program, name.c_str());
	}
	Uniform() = default;
	inline Uniform(const Program& program, const char* name) {
		assign(program, name);
	}
	inline Uniform(const Program& program, const std::string& name) {
		assign(program, name.c_str());
	}
	inline void operator=(const T& val) {
		libv::gl::uniform(location, val);
	}
public:
	inline auto id() const {
		return location;
	}
	inline operator GLint() const {
		return location;
	}
};

// -------------------------------------------------------------------------------------------------

using Uniform_texture = Uniform<libv::gl::TextureChannel>;

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

// BaseAttribute -----------------------------------------------------------------------------------

struct BaseAttributeCore {
protected:
	GLint location = 0;
public:
	inline auto id() const {
		return location;
	}
	inline operator GLint() const {
		return location;
	}
};

template <typename T>
struct BaseAttribute;

template <> struct BaseAttribute<bool> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 1;
};

template <> struct BaseAttribute<int> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 1;
};

template <> struct BaseAttribute<float> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::FLOAT);
	static constexpr GLint attributeSize = 1;
};

template <> struct BaseAttribute<double> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::DOUBLE);
	static constexpr GLint attributeSize = 1;
};

template <> struct BaseAttribute<libv::ivec2> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 2;
};

template <> struct BaseAttribute<libv::fvec2> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::FLOAT);
	static constexpr GLint attributeSize = 2;
};

template <> struct BaseAttribute<libv::dvec2> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::DOUBLE);
	static constexpr GLint attributeSize = 2;
};

template <> struct BaseAttribute<libv::ivec3> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 3;
};

template <> struct BaseAttribute<libv::fvec3> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::FLOAT);
	static constexpr GLint attributeSize = 3;
};

template <> struct BaseAttribute<libv::dvec3> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::DOUBLE);
	static constexpr GLint attributeSize = 3;
};

template <> struct BaseAttribute<libv::ivec4> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 3;
};

template <> struct BaseAttribute<libv::fvec4> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::FLOAT);
	static constexpr GLint attributeSize = 4;
};

template <> struct BaseAttribute<libv::dvec4> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::DOUBLE);
	static constexpr GLint attributeSize = 4;
};

template <> struct BaseAttribute<glm::ivec2> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 2;
};

template <> struct BaseAttribute<glm::fvec2> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::FLOAT);
	static constexpr GLint attributeSize = 2;
};

template <> struct BaseAttribute<glm::dvec2> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::DOUBLE);
	static constexpr GLint attributeSize = 2;
};

template <> struct BaseAttribute<glm::ivec3> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 3;
};

template <> struct BaseAttribute<glm::fvec3> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::FLOAT);
	static constexpr GLint attributeSize = 3;
};

template <> struct BaseAttribute<glm::dvec3> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::DOUBLE);
	static constexpr GLint attributeSize = 3;
};

template <> struct BaseAttribute<glm::ivec4> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::INT);
	static constexpr GLint attributeSize = 3;
};

template <> struct BaseAttribute<glm::fvec4> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::FLOAT);
	static constexpr GLint attributeSize = 4;
};

template <> struct BaseAttribute<glm::dvec4> : public BaseAttributeCore {
	static constexpr GLenum attributeType = to_value(AttributeType::DOUBLE);
	static constexpr GLint attributeSize = 4;
};

// AttributeLocation -------------------------------------------------------------------------------

template <typename T>
class AttributeLocation : public BaseAttribute<T> {
public:
	inline void assign(const Program& program, const char* name) {
		LIBV_GL_DEBUG_ASSERT(program.id() != 0);
		this->location = glGetAttribLocation(program, name);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void assign(const Program& program, const std::string& name) {
		assign(program, name.c_str());
	}
	AttributeLocation() = default;
	inline AttributeLocation(const Program& program, const char* name) {
		assign(program, name);
	}
	inline AttributeLocation(const Program& program, const std::string& name) {
		assign(program, name.c_str());
	}
};

// Attribute ---------------------------------------------------------------------------------------

template <typename T>
class AttributeFixLocation : public BaseAttribute<T> {
public:
	inline void assign(GLint location) {
		this->location = location;
	}
	inline AttributeFixLocation<T>& operator=(GLint location) {
		this->location = location;
		return *this;
	}
	AttributeFixLocation() {}
	inline AttributeFixLocation(GLint location) {
		this->location = location;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
using Attribute = AttributeFixLocation<T>;

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv


//Returned Symbolic Contant | Shader Uniform Type
//
//GL_FLOAT, "float"
//GL_FLOAT_VEC2, "vec2"
//GL_FLOAT_VEC3, "vec3"
//GL_FLOAT_VEC4, "vec4"
//GL_DOUBLE, "double"
//GL_DOUBLE_VEC2, "dvec2"
//GL_DOUBLE_VEC3, "dvec3"
//GL_DOUBLE_VEC4, "dvec4"
//GL_INT, "int"
//GL_INT_VEC2, "ivec2"
//GL_INT_VEC3, "ivec3"
//GL_INT_VEC4, "ivec4"
//GL_UNSIGNED_INT, "unsigned, "int"
//GL_UNSIGNED_INT_VEC2, "uvec2"
//GL_UNSIGNED_INT_VEC3, "uvec3"
//GL_UNSIGNED_INT_VEC4, "uvec4"
//GL_BOOL, "bool"
//GL_BOOL_VEC2, "bvec2"
//GL_BOOL_VEC3, "bvec3"
//GL_BOOL_VEC4, "bvec4"
//GL_FLOAT_MAT2, "mat2"
//GL_FLOAT_MAT3, "mat3"
//GL_FLOAT_MAT4, "mat4"
//GL_FLOAT_MAT2x3, "mat2x3"
//GL_FLOAT_MAT2x4, "mat2x4"
//GL_FLOAT_MAT3x2, "mat3x2"
//GL_FLOAT_MAT3x4, "mat3x4"
//GL_FLOAT_MAT4x2, "mat4x2"
//GL_FLOAT_MAT4x3, "mat4x3"
//GL_DOUBLE_MAT2, "dmat2"
//GL_DOUBLE_MAT3, "dmat3"
//GL_DOUBLE_MAT4, "dmat4"
//GL_DOUBLE_MAT2x3, "dmat2x3"
//GL_DOUBLE_MAT2x4, "dmat2x4"
//GL_DOUBLE_MAT3x2, "dmat3x2"
//GL_DOUBLE_MAT3x4, "dmat3x4"
//GL_DOUBLE_MAT4x2, "dmat4x2"
//GL_DOUBLE_MAT4x3, "dmat4x3"
//GL_SAMPLER_1D, "sampler1D"
//GL_SAMPLER_2D, "sampler2D"
//GL_SAMPLER_3D, "sampler3D"
//GL_SAMPLER_CUBE, "samplerCube"
//GL_SAMPLER_1D_SHADOW, "sampler1DShadow"
//GL_SAMPLER_2D_SHADOW, "sampler2DShadow"
//GL_SAMPLER_1D_ARRAY, "sampler1DArray"
//GL_SAMPLER_2D_ARRAY, "sampler2DArray"
//GL_SAMPLER_1D_ARRAY_SHADOW, "sampler1DArrayShadow"
//GL_SAMPLER_2D_ARRAY_SHADOW, "sampler2DArrayShadow"
//GL_SAMPLER_2D_MULTISAMPLE, "sampler2DMS"
//GL_SAMPLER_2D_MULTISAMPLE_ARRAY, "sampler2DMSArray"
//GL_SAMPLER_CUBE_SHADOW, "samplerCubeShadow"
//GL_SAMPLER_BUFFER, "samplerBuffer"
//GL_SAMPLER_2D_RECT, "sampler2DRect"
//GL_SAMPLER_2D_RECT_SHADOW, "sampler2DRectShadow"
//GL_INT_SAMPLER_1D, "isampler1D"
//GL_INT_SAMPLER_2D, "isampler2D"
//GL_INT_SAMPLER_3D, "isampler3D"
//GL_INT_SAMPLER_CUBE, "isamplerCube"
//GL_INT_SAMPLER_1D_ARRAY, "isampler1DArray"
//GL_INT_SAMPLER_2D_ARRAY, "isampler2DArray"
//GL_INT_SAMPLER_2D_MULTISAMPLE, "isampler2DMS"
//GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, "isampler2DMSArray"
//GL_INT_SAMPLER_BUFFER, "isamplerBuffer"
//GL_INT_SAMPLER_2D_RECT, "isampler2DRect"
//GL_UNSIGNED_INT_SAMPLER_1D, "usampler1D"
//GL_UNSIGNED_INT_SAMPLER_2D, "usampler2D"
//GL_UNSIGNED_INT_SAMPLER_3D, "usampler3D"
//GL_UNSIGNED_INT_SAMPLER_CUBE, "usamplerCube"
//GL_UNSIGNED_INT_SAMPLER_1D_ARRAY, "usampler2DArray"
//GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, "usampler2DArray"
//GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, "usampler2DMS"
//GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, "usampler2DMSArray"
//GL_UNSIGNED_INT_SAMPLER_BUFFER, "usamplerBuffer"
//GL_UNSIGNED_INT_SAMPLER_2D_RECT, "usampler2DRect"