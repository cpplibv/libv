// File:   error.hpp

#pragma once

#include <GL/glew.h>
#include <string>

namespace vl {
namespace gl {

// -------------------------------------------------------------------------------------------------

inline bool glGetShaderCompileStatus(GLuint shaderID) {
	GLint result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	return result;
}

inline std::string glGetShaderCompileMessage(GLuint shaderID) {
	std::string result;
	int infoLength;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLength);

	std::string errorMessage;
	errorMessage.resize(infoLength);
	glGetShaderInfoLog(shaderID, infoLength, nullptr, &result[0]);
	return result;
}

// -------------------------------------------------------------------------------------------------

inline bool glGetProgamLinkStatus(GLuint programID) {
	GLint result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	return result;
}

inline std::string glGetProgamLinkMessage(GLuint programID) {
	std::string result;
	int infoLength;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);

	std::string errorMessage;
	errorMessage.resize(infoLength);
	glGetProgramInfoLog(programID, infoLength, nullptr, &result[0]);
	return result;
}

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl
