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

// Shader --------------------------------------------------------------------------------------

class Shader {
private:
	GLuint shaderID = 0;

public:
	inline Shader() = default;
	inline Shader(const Shader&) = delete;
	inline Shader(Shader&& orig) noexcept : shaderID(orig.shaderID) {
		orig.shaderID = 0;
	}
	inline ~Shader() {
		LIBV_GL_DEBUG_ASSERT(shaderID == 0);
	}

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

	inline void createCompile(ShaderType type, const char* sourceStr, const GLint size) {
		create(type);
		compile(sourceStr, size);
	}

	inline void createCompile(ShaderType type, const std::string& sourceStr) {
		createCompile(type, sourceStr.c_str(), sourceStr.size());
	}

public:
	inline auto id() const {
		return shaderID;
	}
	inline operator GLuint() const {
		return shaderID;
	}
};

// GuardedShader ----------------------------------------------------------------------------------------

class GuardedShader : public Shader {
public:
	inline GuardedShader() {
	}
	inline GuardedShader(ShaderType type) {
		create(type);
	}
	inline GuardedShader(ShaderType type, const char* sourceStr, const size_t size) {
		createCompile(type, sourceStr, size);
	}
	inline GuardedShader(ShaderType type, const std::string& sourceStr) :
		GuardedShader(type, sourceStr.c_str(), sourceStr.size()) { }
	inline GuardedShader(const GuardedShader&) = delete;
	inline GuardedShader(Shader&& orig) : Shader(std::move(orig)) {}
	inline GuardedShader(GuardedShader&& orig) : Shader(std::move(orig)) {}
	inline ~GuardedShader() {
		if (id())
			destroy();
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv