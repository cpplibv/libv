// File: Shader.cpp, Created on 2014. december 7. 13:03, Author: Vader

// hpp
#include "vl/gl/shader.hpp"
// std
#include <fstream>
// pro
#include "vl/gl/log.hpp"

namespace vl {
namespace gl {
namespace detail {

std::string readShaderFile(const std::string& file) {
	std::ifstream shaderFile(file, std::ios::in);
	std::string result;
	if (shaderFile.is_open()) {
		std::string line;
		while (std::getline(shaderFile, line))
			result += "\n" + line;
		shaderFile.close();
	} else
		VLOG_ERROR(vl::gl::log(), std::string("Failed to open shader file: ") + file);
	return result;
}

GLint compileShader(GLuint id, const std::string& file) {
	GLint result = GL_FALSE;
	int infoLength;

	std::string source = readShaderFile(file);
	char const* sourcePtr = source.c_str();
	glShaderSource(id, 1, &sourcePtr, nullptr);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLength);

	if (infoLength > 1) {
		std::string errorMessage(infoLength, ' ');
		glGetShaderInfoLog(id, infoLength, nullptr, &errorMessage[0]);
		VLOG_ERROR(vl::gl::log(), std::string("Failed to compile ") + file + ":" + errorMessage);
	}

	return result;
}

GLuint loadShaderProgram(const std::string& program, const std::string& vs, const std::string& gs, const std::string& fs) {
	if (!gs.empty())
		VLOG_ERROR(vl::gl::log(), "Geometry shader not implemented yet!");

	GLint programStatus = GL_FALSE;
	GLuint id = glCreateProgram();

	GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	compileShader(vertexID, vs);
	compileShader(fragmentID, fs);

	glAttachShader(id, vertexID);
	glAttachShader(id, fragmentID);

	glLinkProgram(id);

	int infoLength;
	glGetProgramiv(id, GL_LINK_STATUS, &programStatus);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLength);

	if (infoLength > 1) {
		std::string errorMessage(infoLength, ' ');
		glGetProgramInfoLog(id, infoLength, nullptr, &errorMessage[0]);
		glDeleteProgram(id);
		id = 0;
		VLOG_ERROR(vl::gl::log(), std::string("In shader program [") + program + "]:\n" + errorMessage);
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	if (programStatus == GL_FALSE || !id) {
		VLOG_ERROR(vl::gl::log(), std::string("Failed linking shader program [") + program + "].");
	}

	checkGL();
	return id;
}
} //namespace detail

// Shader --------------------------------------------------------------

int Shader::nextUniformIndex = 0;

std::vector<std::string>& Shader::uniforms() {
	static std::vector<std::string> data;
	return data;
}

Shader* Shader::currentShader = nullptr;

int Shader::registerUniform(const std::string& name) {
	uniforms().emplace_back(name);
	return nextUniformIndex++;
}

GLint Shader::getUniformLocation(int id) {
	return currentShader->uniformLocs[id];
}

// ---

Shader::Shader(const std::string& name, const std::string& vs, const std::string& fs) :
	name(name),
	uniformLocs(new int[nextUniformIndex]),
	fs(fs),
	vs(vs) { }

Shader::Shader(const std::string& name, const std::string& vs, const std::string& gs, const std::string& fs) :
	name(name),
	uniformLocs(new int[nextUniformIndex]),
	fs(fs),
	gs(gs),
	vs(vs) { }

Shader::~Shader() {
	unloadProgram();
}

void Shader::loadProgram() {
	id = detail::loadShaderProgram(name, vs, gs, fs);
	if (id)
		for (int i = 0; i < nextUniformIndex; i++)
			uniformLocs[i] = glGetUniformLocation(id, uniforms().at(i).c_str());
	else
		std::fill(uniformLocs.get(), uniformLocs.get() + nextUniformIndex, -1);
}

void Shader::unloadProgram() {
	assert(currentShader != this);

	if (id)
		glDeleteProgram(id);
	id = 0;
}

void Shader::reloadProgram() {
	unloadProgram();
	loadProgram();
}

void Shader::useProgram() {
	glUseProgram(id);
	currentShader = this;
}

void Shader::printActiveUniforms() {
	int num = 0;
	for (int i = 0; i < nextUniformIndex; i++) {
		if (uniformLocs[i] != -1)
			num++;
	}
	VLOG_INFO(vl::gl::log(), std::string("Shader program [") + name + "] has " + std::to_string(num) + " active uniforms:");
	for (int i = 0; i < nextUniformIndex; i++) {
		if (uniformLocs[i] != -1)
			VLOG_INFO(vl::gl::log(), std::string("\t") + uniforms()[i]);
	}
}
} //namespace gl
} //namespace vl

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//Light::~Light() { }
//
//glm::mat4 Light::getPmat() {
//	if (type == spotLight)
//		return perspective<float>(acos(outerCosAngle) * 180.0f / PI * 2, 1.0f, range / 15.0f, range); //2szeres outer sz�g, mivel nek�nk nem a 'fele' kell hanem a teljes 'sug�r'
//	else if (type == dirLight)
//		return ortho<float>(-90, 90, -90, 90, -90, 90);
//	else //if (type == pointLight)
//		return ortho<float>(-30, 30, -30, 30, -10, 150);
//}
//
//glm::mat4 Light::getVmat() {
//	if (type == spotLight)
//		return lookAt(position, position + direction, glm::glm::vec3(0, 1, 0));
//	else if (type == dirLight)
//		return lookAt(glm::vec3(0, 0, 0), direction, glm::glm::vec3(0, 1, 0));
//	else //if (type == pointLight)
//		return ortho<float>(-30, 30, -30, 30, -10, 150);
//}

////==============================================================================
//// GL like API
////==============================================================================
//
//void vglUseShader(Shader s) {
//	s->useProgram();
//}
//
//void vglReloadShader(Shader s) {
//	s->reloadProgram();
//}
//
//Shader vglLoadShader(string vs, string fs) {
//	return new _Shader(vs, fs);
//}
//
//Shader vglLoadShader(string vs, string gs, string fs) {
//	return new _Shader(vs, gs, fs);
//}
//
//void vglDeleteShader(Shader s) {
//	delete s;
//}
