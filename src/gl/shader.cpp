// File: ShaderProgram.cpp, Created on 2014. december 7. 13:03, Author: Vader

// hpp
#include <vl/gl/shader.hpp>
// ext
#include <boost/asio/buffer.hpp>
#include <boost/filesystem/path.hpp>
// vl
#include <vl/read_file.hpp>
// std
#include <memory>
// pro
#include <vl/gl/log.hpp>
#include <vl/gl/gl.hpp>

namespace vl {
namespace gl {

// toString ----------------------------------------------------------------------------------------

const char* shaderTypeToString(GLenum type) {
	if (type == GL_VERTEX_SHADER)
		return SHADER_TYPE_VERTEX_NAME;
	if (type == GL_GEOMETRY_SHADER)
		return SHADER_TYPE_GEOMETRY_NAME;
	if (type == GL_FRAGMENT_SHADER)
		return SHADER_TYPE_FRAGMENT_NAME;

	return DEFAULT_SHADER_TYPE_NAME;
}

// Shader --------------------------------------------------------------------------------------

BaseShader::BaseShader(
		const boost::asio::const_buffer& data, const GLenum type, const std::string& name) :
	BaseShader(boost::asio::buffer_cast<const char*>(data), boost::asio::buffer_size(data), type, name) { }

BaseShader::BaseShader(const boost::filesystem::path& filePath, const GLenum type) :
	BaseShader(filePath, type, filePath.string()) { }

BaseShader::BaseShader(
		const boost::filesystem::path& filePath, const GLenum type, const std::string& name) :
	type(type),
	name(name) {

	const auto data = readFile(filePath);
	init(data.data());
}

BaseShader::BaseShader(
		const char* data, const size_t, const GLenum type, const std::string& name) :
	type(type),
	name(name) {

	init(data);
}

void BaseShader::init(const char* source) {
	loadGL(source);
}

BaseShader::~BaseShader() {
	unloadGL();
}

// -------------------------------------------------------------------------------------------------

void BaseShader::loadGL(const char* source) {
	VLOG_TRACE(vl::gl::log(), "GL Loading %s shader: [%s]", shaderTypeToString(type), name);
	shaderID = glCreateShader(type);

	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);

	if (!getShaderCompileStatus(shaderID)) {
		VLOG_ERROR(vl::gl::log(), "Failed to compile %s shader [%s]: %s",
				shaderTypeToString(type), name, getShaderCompileMessage(shaderID));
		unloadGL();
	}
	checkGL();
}

void BaseShader::unloadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Unloading %s shader: [%s]", shaderTypeToString(type), name);
	glDeleteShader(shaderID);
	checkGL();
}

// ShaderProgram -----------------------------------------------------------------------------------

ShaderProgram::ShaderProgram(
		const std::string& name,
		const std::shared_ptr<BaseShader>& shaderVertex,
		const std::shared_ptr<BaseShader>& shaderGeometry,
		const std::shared_ptr<BaseShader>& shaderFragment) :
	name(name),
	shaderVertex(shaderVertex),
	shaderGeometry(shaderGeometry),
	shaderFragment(shaderFragment) {
	loadGL();
	mapUniforms();
}

ShaderProgram::~ShaderProgram() {
	unloadGL();
}

// -------------------------------------------------------------------------------------------------

void ShaderProgram::loadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Loading shader program: [%s]", name);
	programID = glCreateProgram();

	if (shaderVertex)
		glAttachShader(programID, shaderVertex->id());
	if (shaderGeometry)
		glAttachShader(programID, shaderGeometry->id());
	if (shaderFragment)
		glAttachShader(programID, shaderFragment->id());

	glLinkProgram(programID);

	if (!getProgamLinkStatus(programID)) {
		VLOG_ERROR(vl::gl::log(), "Failed to link [%s]: %s", name, getProgamLinkMessage(programID));
		unloadGL();
	}
	checkGL();
}

void ShaderProgram::mapAttributes() {
	//	glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &count);
	//	glGetActiveAttrib (shaderID, i, bufSize, &length, &size, &type, name);
}

void ShaderProgram::mapUniforms() {
	if (!programID)
		return;

	GLint count;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);
	for (GLint i = 0; i < count; ++i) {
		char uniformName[255];
		GLint uniformNameLenght;
		GLint uniformSize;
		GLenum uniformType;
		glGetActiveUniform(programID, i, sizeof (uniformName),
				&uniformNameLenght, &uniformSize, &uniformType, uniformName);
		uniformName[uniformNameLenght] = '\0';

		if (uniformSize == 1) {
			GLint uniformLocation = glGetUniformLocation(programID, uniformName);
			addressesUniform.emplace(uniformName,
					UniformInfo{uniformLocation, i, uniformSize, uniformType});
		} else {
			*std::strrchr(uniformName, '[') = '\0';
			std::string uniformSubName;
			for (int j = 0; j < uniformSize; j++) {
				uniformSubName = vl::format("%s[%d]", uniformName, j);
				GLint uniformLocation = glGetUniformLocation(programID, uniformSubName.c_str());

				addressesUniform.emplace(uniformSubName,
						UniformInfo{uniformLocation, i, uniformSize, uniformType});
			}
		}
	}
}

void ShaderProgram::unloadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Unloading shader program: [%s]", name);
	glDeleteProgram(programID);
	programID = 0;
	checkGL();
}

// -------------------------------------------------------------------------------------------------

void ShaderProgram::use() {
	glUseProgram(programID);
}

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl


// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

//uniformLocs[i] = glGetUniformLocation(id, uniforms().at(i).c_str());
//
//void GL::useProgram(ShaderProgram&) { ...
//	glUseProgram(id); ...
//	currentShaderProgram = this;
//}
//
//void useProgram();
//void printActiveUniforms();
//void infos();

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

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