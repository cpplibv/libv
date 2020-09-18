// Project: libv.gl, File: src/libv/gl/uniform.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <GL/glew.h>
// std
#include <cstdint>
// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
// pro
#include <libv/gl/enum.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

// Special
inline void uniform(const GLint location, const libv::gl::TextureChannel value) noexcept {
	glUniform1i(location, to_value(value));
}

// Scalar
inline void uniform(const GLint location, const bool value) noexcept {
	glUniform1i(location, value);
}
inline void uniform(const GLint location, const int32_t value) noexcept {
	glUniform1i(location, value);
}
inline void uniform(const GLint location, const uint32_t value) noexcept {
	glUniform1ui(location, value);
}
inline void uniform(const GLint location, const int64_t value) noexcept {
	glUniform1i64ARB(location, value);
}
inline void uniform(const GLint location, const uint64_t value) noexcept {
	glUniform1ui64ARB(location, value);
}
inline void uniform(const GLint location, const float value) noexcept {
	glUniform1f(location, value);
}
inline void uniform(const GLint location, const double value) noexcept {
	glUniform1d(location, value);
}

// Vector 2D
inline void uniform(const GLint location, const libv::vec2b value) noexcept {
	glUniform2i(location, value.x, value.y);
}
inline void uniform(const GLint location, const libv::vec2i value) noexcept {
	glUniform2i(location, value.x, value.y);
}
inline void uniform(const GLint location, const libv::vec2ui value) noexcept {
	glUniform2ui(location, value.x, value.y);
}
inline void uniform(const GLint location, const libv::vec2l value) noexcept {
	glUniform2i64ARB(location, value.x, value.y);
}
inline void uniform(const GLint location, const libv::vec2ul value) noexcept {
	glUniform2ui64ARB(location, value.x, value.y);
}
inline void uniform(const GLint location, const libv::vec2f value) noexcept {
	glUniform2f(location, value.x, value.y);
}
inline void uniform(const GLint location, const libv::vec2d value) noexcept {
	glUniform2d(location, value.x, value.y);
}

// Vector 3D
inline void uniform(const GLint location, const libv::vec3b value) noexcept {
	glUniform3i(location, value.x, value.y, value.z);
}
inline void uniform(const GLint location, const libv::vec3i value) noexcept {
	glUniform3i(location, value.x, value.y, value.z);
}
inline void uniform(const GLint location, const libv::vec3ui value) noexcept {
	glUniform3ui(location, value.x, value.y, value.z);
}
inline void uniform(const GLint location, const libv::vec3l value) noexcept {
	glUniform3i64ARB(location, value.x, value.y, value.z);
}
inline void uniform(const GLint location, const libv::vec3ul value) noexcept {
	glUniform3ui64ARB(location, value.x, value.y, value.z);
}
inline void uniform(const GLint location, const libv::vec3f value) noexcept {
	glUniform3f(location, value.x, value.y, value.z);
}
inline void uniform(const GLint location, const libv::vec3d value) noexcept {
	glUniform3d(location, value.x, value.y, value.z);
}

// Vector 4D
inline void uniform(const GLint location, const libv::vec4b value) noexcept {
	glUniform4i(location, value.x, value.y, value.z, value.w);
}
inline void uniform(const GLint location, const libv::vec4i value) noexcept {
	glUniform4i(location, value.x, value.y, value.z, value.w);
}
inline void uniform(const GLint location, const libv::vec4ui value) noexcept {
	glUniform4ui(location, value.x, value.y, value.z, value.w);
}
inline void uniform(const GLint location, const libv::vec4l value) noexcept {
	glUniform4i64ARB(location, value.x, value.y, value.z, value.w);
}
inline void uniform(const GLint location, const libv::vec4ul value) noexcept {
	glUniform4ui64ARB(location, value.x, value.y, value.z, value.w);
}
inline void uniform(const GLint location, const libv::vec4f value) noexcept {
	glUniform4f(location, value.x, value.y, value.z, value.w);
}
inline void uniform(const GLint location, const libv::vec4d value) noexcept {
	glUniform4d(location, value.x, value.y, value.z, value.w);
}

// Matrix 2x2
inline void uniform(const GLint location, const libv::mat2f& value) noexcept {
	glUniformMatrix2fv(location, 1, GL_FALSE, value.data());
}
inline void uniform(const GLint location, const libv::mat2d& value) noexcept {
	glUniformMatrix2dv(location, 1, GL_FALSE, value.data());
}
// Matrix 3x3
inline void uniform(const GLint location, const libv::mat3f& value) noexcept {
	glUniformMatrix3fv(location, 1, GL_FALSE, value.data());
}
inline void uniform(const GLint location, const libv::mat3d& value) noexcept {
	glUniformMatrix3dv(location, 1, GL_FALSE, value.data());
}
// Matrix 4x4
inline void uniform(const GLint location, const libv::mat4f& value) noexcept {
	glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
}
inline void uniform(const GLint location, const libv::mat4d& value) noexcept {
	glUniformMatrix4dv(location, 1, GL_FALSE, value.data());
}

// Matrix 2x3
inline void uniform(const GLint location, const libv::mat2x3f& value) noexcept {
	glUniformMatrix2x3fv(location, 1, GL_FALSE, value.data());
}
inline void uniform(const GLint location, const libv::mat2x3d& value) noexcept {
	glUniformMatrix2x3dv(location, 1, GL_FALSE, value.data());
}
// Matrix 2x4
inline void uniform(const GLint location, const libv::mat2x4f& value) noexcept {
	glUniformMatrix2x4fv(location, 1, GL_FALSE, value.data());
}
inline void uniform(const GLint location, const libv::mat2x4d& value) noexcept {
	glUniformMatrix2x4dv(location, 1, GL_FALSE, value.data());
}
// Matrix 3x2
inline void uniform(const GLint location, const libv::mat3x2f& value) noexcept {
	glUniformMatrix3x2fv(location, 1, GL_FALSE, value.data());
}
inline void uniform(const GLint location, const libv::mat3x2d& value) noexcept {
	glUniformMatrix3x2dv(location, 1, GL_FALSE, value.data());
}
// Matrix 3x4
inline void uniform(const GLint location, const libv::mat3x4f& value) noexcept {
	glUniformMatrix3x4fv(location, 1, GL_FALSE, value.data());
}
inline void uniform(const GLint location, const libv::mat3x4d& value) noexcept {
	glUniformMatrix3x4dv(location, 1, GL_FALSE, value.data());
}
// Matrix 4x2
inline void uniform(const GLint location, const libv::mat4x2f& value) noexcept {
	glUniformMatrix4x2fv(location, 1, GL_FALSE, value.data());
}
inline void uniform(const GLint location, const libv::mat4x2d& value) noexcept {
	glUniformMatrix4x2dv(location, 1, GL_FALSE, value.data());
}
// Matrix 4x3
inline void uniform(const GLint location, const libv::mat4x3f& value) noexcept {
	glUniformMatrix4x3fv(location, 1, GL_FALSE, value.data());
}
inline void uniform(const GLint location, const libv::mat4x3d& value) noexcept {
	glUniformMatrix4x3dv(location, 1, GL_FALSE, value.data());
}

// -------------------------------------------------------------------------------------------------

// TODO P3: glr has a similar Uniform class, would be nice to only keep one
struct Uniform {
	GLint location = -1;
};

template <typename T>
struct Uniform_t : Uniform {
	inline void operator=(const T& val) & noexcept {
		libv::gl::uniform(location, val);
	}
};

using Uniform_texture = Uniform_t<libv::gl::TextureChannel>;

using Uniform_bool = Uniform_t<bool>;
using Uniform_int32 = Uniform_t<int32_t>;
using Uniform_uint32 = Uniform_t<uint32_t>;
using Uniform_int64 = Uniform_t<int64_t>;
using Uniform_uint64 = Uniform_t<uint64_t>;
using Uniform_float = Uniform_t<float>;
using Uniform_double = Uniform_t<double>;

using Uniform_vec2b = Uniform_t<libv::vec2b>;
using Uniform_vec3b = Uniform_t<libv::vec3b>;
using Uniform_vec4b = Uniform_t<libv::vec4b>;
using Uniform_vec2i = Uniform_t<libv::vec2i>;
using Uniform_vec3i = Uniform_t<libv::vec3i>;
using Uniform_vec4i = Uniform_t<libv::vec4i>;
using Uniform_vec2ui = Uniform_t<libv::vec2ui>;
using Uniform_vec3ui = Uniform_t<libv::vec3ui>;
using Uniform_vec4ui = Uniform_t<libv::vec4ui>;
using Uniform_vec2l = Uniform_t<libv::vec2l>;
using Uniform_vec3l = Uniform_t<libv::vec3l>;
using Uniform_vec4l = Uniform_t<libv::vec4l>;
using Uniform_vec2ul = Uniform_t<libv::vec2ul>;
using Uniform_vec3ul = Uniform_t<libv::vec3ul>;
using Uniform_vec4ul = Uniform_t<libv::vec4ul>;
using Uniform_vec2f = Uniform_t<libv::vec2f>;
using Uniform_vec3f = Uniform_t<libv::vec3f>;
using Uniform_vec4f = Uniform_t<libv::vec4f>;
using Uniform_vec2d = Uniform_t<libv::vec2d>;
using Uniform_vec3d = Uniform_t<libv::vec3d>;
using Uniform_vec4d = Uniform_t<libv::vec4d>;

using Uniform_mat2f = Uniform_t<libv::mat2f>;
using Uniform_mat3f = Uniform_t<libv::mat3f>;
using Uniform_mat4f = Uniform_t<libv::mat4f>;
using Uniform_mat2d = Uniform_t<libv::mat2d>;
using Uniform_mat3d = Uniform_t<libv::mat3d>;
using Uniform_mat4d = Uniform_t<libv::mat4d>;

using Uniform_mat2x2f = Uniform_t<libv::mat2x2f>;
using Uniform_mat2x3f = Uniform_t<libv::mat2x3f>;
using Uniform_mat2x4f = Uniform_t<libv::mat2x4f>;
using Uniform_mat3x2f = Uniform_t<libv::mat3x2f>;
using Uniform_mat3x3f = Uniform_t<libv::mat3x3f>;
using Uniform_mat3x4f = Uniform_t<libv::mat3x4f>;
using Uniform_mat4x2f = Uniform_t<libv::mat4x2f>;
using Uniform_mat4x3f = Uniform_t<libv::mat4x3f>;
using Uniform_mat4x4f = Uniform_t<libv::mat4x4f>;
using Uniform_mat2x2d = Uniform_t<libv::mat2x2d>;
using Uniform_mat2x3d = Uniform_t<libv::mat2x3d>;
using Uniform_mat2x4d = Uniform_t<libv::mat2x4d>;
using Uniform_mat3x2d = Uniform_t<libv::mat3x2d>;
using Uniform_mat3x3d = Uniform_t<libv::mat3x3d>;
using Uniform_mat3x4d = Uniform_t<libv::mat3x4d>;
using Uniform_mat4x2d = Uniform_t<libv::mat4x2d>;
using Uniform_mat4x3d = Uniform_t<libv::mat4x3d>;
using Uniform_mat4x4d = Uniform_t<libv::mat4x4d>;

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
//GL_DOUBLE_VEC2, "vec2d"
//GL_DOUBLE_VEC3, "vec3d"
//GL_DOUBLE_VEC4, "vec4d"
//GL_INT, "int"
//GL_INT_VEC2, "vec2i"
//GL_INT_VEC3, "vec3i"
//GL_INT_VEC4, "vec4i"
//GL_UNSIGNED_INT, "unsigned, "int"
//GL_UNSIGNED_INT_VEC2, "vec2u"
//GL_UNSIGNED_INT_VEC3, "vec3u"
//GL_UNSIGNED_INT_VEC4, "vec4u"
//GL_BOOL, "bool"
//GL_BOOL_VEC2, "vec2b"
//GL_BOOL_VEC3, "vec3b"
//GL_BOOL_VEC4, "vec4b"
//GL_FLOAT_MAT2, "mat2f"
//GL_FLOAT_MAT3, "mat3f"
//GL_FLOAT_MAT4, "mat4f"
//GL_FLOAT_MAT2x3, "mat2x3f"
//GL_FLOAT_MAT2x4, "mat2x4f"
//GL_FLOAT_MAT3x2, "mat3x2f"
//GL_FLOAT_MAT3x4, "mat3x4f"
//GL_FLOAT_MAT4x2, "mat4x2f"
//GL_FLOAT_MAT4x3, "mat4x3f"
//GL_DOUBLE_MAT2, "mat2d"
//GL_DOUBLE_MAT3, "mat3d"
//GL_DOUBLE_MAT4, "mat4d"
//GL_DOUBLE_MAT2x3, "mat2x3d"
//GL_DOUBLE_MAT2x4, "mat2x4d"
//GL_DOUBLE_MAT3x2, "mat3x2d"
//GL_DOUBLE_MAT3x4, "mat3x4d"
//GL_DOUBLE_MAT4x2, "mat4x2d"
//GL_DOUBLE_MAT4x3, "mat4x3d"
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
