// File: shader.hpp, Created on 2014. december 7. 12:56, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
// std
#include <string>
// libv
#include <libv/utility.hpp>
// pro
//#include <libv/gl/gl.hpp>
#include <libv/gl/enum.hpp>

namespace libv {
namespace gl {

// BaseShader --------------------------------------------------------------------------------------

class BaseShader {
private:
	GLuint shaderID = 0;

protected:
	inline BaseShader() = default;
	inline BaseShader(const BaseShader&) = delete;
	inline BaseShader(BaseShader&&) = delete;
	inline ~BaseShader() = default;

public:
	inline bool status() {
		LIBV_GL_DEBUG_ASSERT(shaderID != 0);
		GLint result;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		LIBV_GL_DEBUG_CHECK_GL();
		return result;
	}

	std::string info() {
		LIBV_GL_DEBUG_ASSERT(shaderID != 0);
		int infoLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLength);

		std::string result;
		result.resize(infoLength);
		glGetShaderInfoLog(shaderID, infoLength, nullptr, &result[0]);
		LIBV_GL_DEBUG_CHECK_GL();
		return result;
	}

	inline void create(ShaderType type) {
		LIBV_GL_DEBUG_ASSERT(shaderID == 0);
		shaderID = glCreateShader(to_value(type));
		LIBV_GL_DEBUG_CHECK_GL();
		if (shaderID == 0)
			LIBV_LOG_GL_ERROR("Failed to create %s shader", to_string(type));
	}

	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(shaderID != 0);
		glDeleteShader(shaderID);
		shaderID = 0;
		LIBV_GL_DEBUG_CHECK_GL();
	}

	inline void compile(const char* sourceStr, const GLint size) {
		LIBV_GL_DEBUG_ASSERT(shaderID != 0);
		glShaderSource(shaderID, 1, &sourceStr, &size);
		LIBV_GL_DEBUG_CHECK_GL();
		glCompileShader(shaderID);
		LIBV_GL_DEBUG_CHECK_GL();
		if (!status())
			LIBV_LOG_GL_ERROR("Failed to compile shader:\n%s", info());
	}

	inline void compile(const std::string& sourceStr) {
		compile(sourceStr.c_str(), sourceStr.size());
	}

	inline void createCompile(const char* sourceStr, const GLint size, ShaderType type) {
		create(type);
		compile(sourceStr, size);
	}

	inline void createCompile(const std::string& sourceStr, ShaderType type) {
		createCompile(sourceStr.c_str(), sourceStr.size(), type);
	}

public:
	inline auto id() const {
		return shaderID;
	}
	inline operator GLuint() const {
		return shaderID;
	}
};

// ShaderAC ----------------------------------------------------------------------------------------

class ShaderAC : public BaseShader {
public:
	inline ShaderAC() {
	}
	inline ShaderAC(ShaderType type) {
		create(type);
	}
	inline ShaderAC(const char* sourceStr, const size_t size, ShaderType type) {
		createCompile(sourceStr, size, type);
	}
	inline ShaderAC(const std::string& sourceStr, ShaderType type) :
		ShaderAC(sourceStr.c_str(), sourceStr.size(), type) { }
	inline ShaderAC(const ShaderAC&) = delete;
	inline ShaderAC(ShaderAC&&) = delete;
	inline ~ShaderAC() {
		if (id())
			destroy();
	}
};

using Shader = ShaderAC;

// ShaderNC ----------------------------------------------------------------------------------------

class ShaderNC : public BaseShader {
public:
	inline ShaderNC() = default;
	inline ~ShaderNC() {
		LIBV_GL_DEBUG_ASSERT(id() == 0);
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv