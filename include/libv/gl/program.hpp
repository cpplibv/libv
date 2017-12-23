// File: program.hpp Author: Vader Created on 2016. július 8., 12:02

#pragma once

// ext
#include <GL/glew.h>
// std
#include <string>
// pro
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/program_object.hpp>
#include <libv/gl/shader.hpp>
#include <libv/gl/uniform.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class AccessProgram {
private:
	Program& object;

public:
	AccessProgram(Program& object) :
		object(object) { }

public:
	inline void create() {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		object.id = glCreateProgram();
		checkGL();
	}

	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteProgram(object.id);
		object.id = 0;
		checkGL();
	}

public:
	inline bool status() const {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		GLint result;
		glGetProgramiv(object.id, GL_LINK_STATUS, &result);
		checkGL();
		return result;
	}

	inline std::string info() const {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		int infoLength;
		glGetProgramiv(object.id, GL_INFO_LOG_LENGTH, &infoLength);

		std::string result;
		result.resize(infoLength);
		glGetProgramInfoLog(object.id, infoLength, nullptr, &result[0]);
		checkGL();
		return result;
	}

public:
	inline void attach(const Shader& shader) {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glAttachShader(object.id, shader.id);
		checkGL();
	}

	inline void link() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glLinkProgram(object.id);
		checkGL();
		if (!status())
			log_gl.error("Failed to link program:\n{}", info());
	}

	inline void link(const Shader& vertex, const Shader& fragment) {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glAttachShader(object.id, vertex.id);
		glAttachShader(object.id, fragment.id);
		checkGL();
		glLinkProgram(object.id);
		checkGL();
		if (!status())
			log_gl.error("Failed to link program:\n{}", info());
	}

	inline void link(const Shader& vertex, const Shader& fragment, const Shader& geometry) {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glAttachShader(object.id, vertex.id);
		glAttachShader(object.id, fragment.id);
		glAttachShader(object.id, geometry.id);
		checkGL();
		glLinkProgram(object.id);
		checkGL();
		if (!status())
			log_gl.error("Failed to link program:\n{}", info());
	}

public:
	inline void use() const {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glUseProgram(object.id);
		checkGL();
	}

public:
	inline void uniformBlockBinding(const char* identifier, GLuint index) const {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		const auto loc = glGetUniformBlockIndex(object.id, identifier);
		glUniformBlockBinding(object.id, loc, index);
		checkGL();
	}

public:
	inline void assign(Uniform& uni, const char* identifier) {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		uni.location = glGetUniformLocation(object.id, identifier);
		checkGL();
	}

	inline GLint uniformLocation(const char* identifier) {
		GLint location;
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		location = glGetUniformLocation(object.id, identifier);
		checkGL();
		return location;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
