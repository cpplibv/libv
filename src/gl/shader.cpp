// File: ShaderProgram.cpp, Created on 2014. december 7. 13:03, Author: Vader

// hpp
#include <libv/gl/shader.hpp>
// ext
#include <boost/asio/buffer.hpp>
#include <boost/filesystem/path.hpp>
// libv
#include <libv/read_file.hpp>
// std
#include <memory>
// pro
#include <libv/gl/log.hpp>
#include <libv/gl/gl.hpp>

namespace libv {
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

BaseShader::BaseShader() { }

BaseShader::BaseShader(
		const boost::asio::const_buffer& data, const GLenum type, const std::string& name) :
	BaseShader(boost::asio::buffer_cast<const char*>(data), boost::asio::buffer_size(data), type, name) { }

BaseShader::BaseShader(const boost::filesystem::path& filePath, const GLenum type) :
	BaseShader(filePath, type, filePath.string()) { }

BaseShader::BaseShader(const boost::filesystem::path& filePath, const GLenum type, const std::string& name) :
	type(type),
	name(name) {

	const auto data = readFile(filePath);
	loadImpl(data.data());
}

BaseShader::BaseShader(const char* data, const size_t, const GLenum type, const std::string& name) :
	type(type),
	name(name) {

	loadImpl(data);
}

BaseShader::~BaseShader() {
	unloadGL();
}

// -------------------------------------------------------------------------------------------------

void BaseShader::load(const boost::asio::const_buffer& data, const GLenum type, const std::string& name) {
	load(boost::asio::buffer_cast<const char*>(data), boost::asio::buffer_size(data), type, name);
}

void BaseShader::load(const boost::filesystem::path& filePath, const GLenum type) {
	load(filePath, type, filePath.string());
}

void BaseShader::load(const boost::filesystem::path& filePath, const GLenum type, const std::string& name) {
	this->type = type;
	this->name = name;

	const auto data = readFile(filePath);
	loadImpl(data.data());
}

void BaseShader::load(const char* data, const size_t, const GLenum type, const std::string& name) {
	this->type = type;
	this->name = name;

	loadImpl(data);
}

// -------------------------------------------------------------------------------------------------

void BaseShader::loadImpl(const char* source) {
	loadGL(source);
}

void BaseShader::loadGL(const char* source) {
	LIBV_GL_TRACE("GL Loading %s shader: [%s]", shaderTypeToString(type), name);
	shaderID = glCreateShader(type);

	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);

	if (!getShaderCompileStatus(shaderID)) {
		LIBV_GL_ERROR("Failed to compile %s shader [%s]: %s",
				shaderTypeToString(type), name, getShaderCompileMessage(shaderID));
		unloadGL();
	}
	checkGL();
}

void BaseShader::unloadGL() {
	LIBV_GL_TRACE("GL Unloading %s shader: [%s]", shaderTypeToString(type), name);
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
	mapAttributes();
	mapUniforms();
}

void ShaderProgram::load(const std::string& name, const std::shared_ptr<BaseShader>& shaderVertex, const std::shared_ptr<BaseShader>& shaderFragment) {
	load(name, shaderVertex, shaderFragment, nullptr);
}

void ShaderProgram::load(const std::string& name, const std::shared_ptr<BaseShader>& shaderVertex, const std::shared_ptr<BaseShader>& shaderFragment, const std::shared_ptr<BaseShader>& shaderGeometry) {
	this->name = name;
	this->shaderVertex = shaderVertex;
	this->shaderFragment = shaderFragment;
	this->shaderGeometry = shaderGeometry;
	loadGL();
}

ShaderProgram::~ShaderProgram() {
	unloadGL();
}

// -------------------------------------------------------------------------------------------------

void ShaderProgram::loadGL() {
	LIBV_GL_TRACE("GL Loading shader program: [%s]", name);
	programID = glCreateProgram();

	if (shaderVertex)
		glAttachShader(programID, shaderVertex->id());
	if (shaderGeometry)
		glAttachShader(programID, shaderGeometry->id());
	if (shaderFragment)
		glAttachShader(programID, shaderFragment->id());

	glLinkProgram(programID);

	if (!getProgamLinkStatus(programID)) {
		LIBV_GL_ERROR("Failed to link [%s]: %s", name, getProgamLinkMessage(programID));
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
				uniformSubName = fmt::sprintf("%s[%d]", uniformName, j);
				GLint uniformLocation = glGetUniformLocation(programID, uniformSubName.c_str());

				addressesUniform.emplace(uniformSubName,
						UniformInfo{uniformLocation, i, uniformSize, uniformType});
			}
		}
	}
}

void ShaderProgram::unloadGL() {
	LIBV_GL_TRACE("GL Unloading shader program: [%s]", name);
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
} //namespace libv


// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
