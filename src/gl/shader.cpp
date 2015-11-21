// File: ShaderProgram.cpp, Created on 2014. december 7. 13:03, Author: Vader

// hpp
#include <vl/gl/shader.hpp>
// ext
#include <boost/asio/buffer.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
// std
#include <memory>
// pro
#include <vl/gl/log.hpp>
#include <vl/gl/status.hpp>

namespace vl {
namespace gl {

// -------------------------------------------------------------------------------------------------

static const auto toString(GLenum type) {
	if (type == GL_VERTEX_SHADER)
		return SHADER_TYPE_VERTEX_NAME;
	if (type == GL_GEOMETRY_SHADER)
		return SHADER_TYPE_GEOMETRY_NAME;
	if (type == GL_FRAGMENT_SHADER)
		return SHADER_TYPE_FRAGMENT_NAME;

	return DEFAULT_SHADER_TYPE_NAME;
}

static std::string readTextFile(const boost::filesystem::path& filePath) {
	std::string result;
	boost::filesystem::ifstream file;
	file.open(filePath.c_str());

	if (!file) {
		VLOG_ERROR(vl::gl::log(), "Failed to open file: [%s]", filePath.string());
		return result;
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();
	result = buffer.str();

	return result;
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

	const auto data = readTextFile(filePath);
	loadGL(data.data());
}

BaseShader::BaseShader(
		const char* data, size_t, const GLenum type, const std::string& name) :
	type(type),
	name(name) {

	loadGL(data);
}

BaseShader::~BaseShader() {
	unloadGL();
}

// -------------------------------------------------------------------------------------------------

void BaseShader::loadGL(const char* source) {
	VLOG_TRACE(vl::gl::log(), "GL Loading %s shader: [%s]", toString(type), name);
	shaderID = glCreateShader(type);

	//			const auto sourcePtr = source.c_str();
	//			glShaderSource(shaderID, 1, &sourcePtr, nullptr);
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);

	if (!glGetShaderCompileStatus(shaderID)) {

		VLOG_ERROR(vl::gl::log(), "Failed to compile %s shader [%s]: %s",
				toString(type), name, glGetShaderCompileMessage(shaderID));
		unloadGL();
	}
	checkGL();
}

void BaseShader::unloadGL() {

	VLOG_TRACE(vl::gl::log(), "GL Unloading %s shader: [%s]", toString(type), name);
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
}

ShaderProgram::~ShaderProgram() {

	unloadGL();
}

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

	if (!glGetProgamLinkStatus(programID)) {

		VLOG_ERROR(vl::gl::log(), "Failed to link [%s]: %s", name, glGetProgamLinkMessage(programID));
		unloadGL();
	}
	checkGL();
}

void ShaderProgram::unloadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Unloading shader program: [%s]", name);
	glDeleteProgram(programID);
	programID = 0;
	checkGL();
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