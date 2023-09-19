// Project: libv.gl, File: src/libv/gl/shader.hpp

#pragma once

// ext
#include <GL/glew.h>
// std
#include <string_view>
// libv
#include <libv/utility/to_underlying.hpp>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/shader_object.hpp>


namespace libv {
namespace gl {

// Shader ------------------------------------------------------------------------------------------

class AccessShader {
private:
	Shader& object;

public:
	inline AccessShader(Shader& object) noexcept :
		object(object) { }

public:
	inline void create(ShaderType type) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		object.id = glCreateShader(to_value(type));
		checkGL();
	}

	inline void destroy() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteShader(object.id);
		object.id = 0;
		checkGL();
	}

public:
	[[nodiscard]] inline bool status() const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		GLint result;
		glGetShaderiv(object.id, GL_COMPILE_STATUS, &result);
		checkGL();
		return result;
	}

	[[nodiscard]] std::string info() const noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		int infoLength;
		glGetShaderiv(object.id, GL_INFO_LOG_LENGTH, &infoLength);

		std::string result;

		if (infoLength != 0) {
			result.resize(infoLength);
			glGetShaderInfoLog(object.id, infoLength, nullptr, &result[0]);

			// OpenGL Includes the null terminator in GL_INFO_LOG_LENGTH and forces a null terminator into the output
			result.pop_back();

			// Discard the last new line character to not break logs and UIs
			if (result.ends_with("\n"))
				result.pop_back();
		}

		checkGL();
		return result;
	}

public:
	inline void compile(const char* sourceStr, const GLint size, bool logError = true) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glShaderSource(object.id, 1, &sourceStr, &size);
		checkGL();
		glCompileShader(object.id);
		checkGL();
		if (logError && !status())
			log_gl.error("Failed to compile shader:\n{}", info());
	}

	inline void compile(std::string_view source, bool logError = true) noexcept {
		compile(source.data(), static_cast<GLint>(source.size()), logError);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
