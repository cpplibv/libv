// File: shader.hpp, Created on 2014. december 7. 12:56, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
// std
#include <string_view>
// libv
#include <libv/utility/enum.hpp>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/shader_object.hpp>


namespace libv {
namespace gl {

// Shader ------------------------------------------------------------------------------------------

class AccessShader {
private:
	Shader& object;

public:
	AccessShader(Shader& object) :
		object(object) { }

public:
	inline void create(ShaderType type) {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		object.id = glCreateShader(to_value(type));
		checkGL();
		if (object.id == 0)
			log_gl.error("Failed to create {} shader", to_string(type));
	}

	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteShader(object.id);
		object.id = 0;
		checkGL();
	}

public:
	inline bool status() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		GLint result;
		glGetShaderiv(object.id, GL_COMPILE_STATUS, &result);
		checkGL();
		return result;
	}

	std::string info() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		int infoLength;
		glGetShaderiv(object.id, GL_INFO_LOG_LENGTH, &infoLength);

		std::string result;
		result.resize(infoLength);
		glGetShaderInfoLog(object.id, infoLength, nullptr, &result[0]);
		checkGL();
		return result;
	}

public:
	inline void compile(const char* sourceStr, const GLint size) {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glShaderSource(object.id, 1, &sourceStr, &size);
		checkGL();
		glCompileShader(object.id);
		checkGL();
		if (!status())
			log_gl.error("Failed to compile shader:\n{}", info());
	}

	inline void compile(std::string_view source) {
		compile(source.data(), static_cast<GLint>(source.size()));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
