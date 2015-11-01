// File: ShaderProgram.cpp, Created on 2014. december 7. 13:03, Author: Vader

// hpp
#include <vl/gl/shader.hpp>
// ext
#include <GL/glew.h> // Including this is not nessessary due to the only use is for GLuint and GLint
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
// std
#include <fstream>
#include <memory>
// pro
#include <vl/gl/log.hpp>
#include <vl/gl/services.hpp>

namespace vl {
namespace gl {

// TextFile --------------------------------------------------------------------------------------

TextFile::TextFile(ServiceShader * const service, const std::string& filePath) :
	filePath(filePath),
	service(service) { }

void TextFile::loadIO() {
	VLOG_TRACE(vl::gl::log(), "IO Loading text file: [%s]", filePath);
	std::ifstream file(filePath, std::ios::in);
	iostate = file.rdstate();

	if (file) {
		VLOG_ERROR(vl::gl::log(), "Failed to open file: [%s]", filePath);
		changeResourceState(ResourceState::FAILED);
	} else {
		std::ostringstream buffer;
		buffer << file.rdbuf();
		file.close();
		text = buffer.str();
		changeResourceState(ResourceState::READY);
	}
}

void TextFile::unloadIO() {
	VLOG_TRACE(vl::gl::log(), "IO Unloading text file: [%s]", filePath);
	changeResourceState(ResourceState::UNREADY);
	text.clear();
}

void TextFile::reload() {
	const auto self = shared_from_this();
	unload(self);
	load(self);
}

void TextFile::load(const std::shared_ptr<TextFile>& self) {
	const auto self_wp = std::weak_ptr<TextFile>(self);
	service->threadIO->executeAsync([self_wp] {
		if (const auto self_sp = self_wp.lock()) {
			self_sp->loadIO();
		}
	}, priority);
}

void TextFile::unload(const std::shared_ptr<TextFile>&) {
	unloadIO(); // in context call
}

bool TextFile::operator<(const TextFile& r) const {
	return filePath < r.filePath;
}

bool operator<(const std::string& filePath, const TextFile& r) {
	return filePath < r.filePath;
}

bool operator<(const TextFile& r, const std::string& filePath) {
	return r.filePath < filePath;
}

// Shader --------------------------------------------------------------------------------------

Shader::Shader(ServiceShader * const service, const std::string& filePath) :
	filePath(filePath),
	service(service) {

	sourceFile = service->cacheTextFile.get<vl::use < 1 >> (service, filePath);
}

void Shader::loadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Loading shader: [%s]", filePath);
	GLuint type = 0;
	//TODO P3: boost file system extension detections
	switch (filePath[filePath.length() - 2]) {
	case 'f':
		type = GL_FRAGMENT_SHADER;
		break;
	case 'g':
		type = GL_GEOMETRY_SHADER;
		break;
	case 'v':
		type = GL_VERTEX_SHADER;
		break;
	default:
		VLOG_ERROR(vl::gl::log(), "Unsupported shader extension");
		changeResourceState(ResourceState::FAILED);
		return;
	}

	GLint statusCompile;
	shaderID = glCreateShader(type);

	const char* sourcePtr = sourceFile->getText().c_str();
	glShaderSource(shaderID, 1, &sourcePtr, nullptr);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &statusCompile);
	if (!statusCompile) {
		int infoLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLength);

		std::string errorMessage;
		errorMessage.resize(infoLength);
		glGetShaderInfoLog(shaderID, infoLength, nullptr, &errorMessage[0]);

		VLOG_ERROR(vl::gl::log(), "Failed to compile [%s]: %s", filePath, errorMessage);

		unloadGL();
		changeResourceState(ResourceState::FAILED);
	} else {
		changeResourceState(ResourceState::READY);
	}

	checkGL();
}

void Shader::unloadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Unloading shader: [%s]", filePath);
	changeResourceState(ResourceState::UNREADY);
	glDeleteShader(shaderID);
	checkGL();
}

void Shader::load(const std::shared_ptr<Shader>& self) {
	addDependency(sourceFile);

	const auto self_wp = std::weak_ptr<Shader>(self);
	setDependencyCallback([this, self_wp] {
		if (!isEveryDependency(ResourceState::READY)) return;
		service->threadGL->executeAsync([self_wp] {
			if (const auto self_sp = self_wp.lock()) {
				self_sp->loadGL();
			}
		}, priority);
	});
}

void Shader::unload(const std::shared_ptr<Shader>& self) {
	service->threadGL->executeAsync([self] {
		self->unloadGL();
	}, priority);
}

bool Shader::operator<(const Shader& r) const {
	return filePath < r.filePath;
}

bool operator<(const std::string& filePath, const Shader& r) {
	return filePath < r.filePath;
}

bool operator<(const Shader& r, const std::string& filePath) {
	return r.filePath < filePath;
}

// ShaderProgramImpl ------------------------------------------------------------------------------------------

ShaderProgramImpl::ShaderProgramImpl(
		ServiceShader * const service,
		const std::string& name,
		const std::string& fsPath,
		const std::string& gsPath,
		const std::string& vsPath) :
	name(name),
	service(service) {
	if (!fsPath.empty())
		fs = service->cacheShader.get<vl::use < 1 >> (service, fsPath);
	if (!gsPath.empty())
		gs = service->cacheShader.get<vl::use < 1 >> (service, gsPath);
	if (!vsPath.empty())
		vs = service->cacheShader.get<vl::use < 1 >> (service, vsPath);
}

void ShaderProgramImpl::loadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Loading shader program: [%s]", name);
	programID = glCreateProgram();

	if (vs)
		glAttachShader(programID, vs->getShaderID());
	if (gs)
		glAttachShader(programID, gs->getShaderID());
	if (fs)
		glAttachShader(programID, fs->getShaderID());

	glLinkProgram(programID);

	GLint statusLink;
	glGetProgramiv(programID, GL_LINK_STATUS, &statusLink);
	if (!statusLink) {
		int infoLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);

		std::string errorMessage;
		errorMessage.resize(infoLength);
		glGetProgramInfoLog(programID, infoLength, nullptr, &errorMessage[0]);

		VLOG_ERROR(vl::gl::log(), "Failed to link [%s]: %s", name, errorMessage);
		unloadGL();
	}

	checkGL();
	changeResourceState(ResourceState::READY);
}

void ShaderProgramImpl::unloadGL() {
	VLOG_TRACE(vl::gl::log(), "GL Unloading shader program: [%s]", name);
	changeResourceState(ResourceState::UNREADY);
	glDeleteProgram(programID);
	checkGL();
}

void ShaderProgramImpl::load(const std::shared_ptr<ShaderProgramImpl>& self) {
	addDependency(fs);
	addDependency(gs);
	addDependency(vs);

	const auto self_wp = std::weak_ptr<ShaderProgramImpl>(self);
	setDependencyCallback([this, self_wp] {
		if (!isEveryDependency(ResourceState::READY)) return;
		service->threadGL->executeAsync([self_wp] {
			if (const auto self_sp = self_wp.lock()) {
				self_sp->loadGL();
			}
		}, priority);
	});
}

void ShaderProgramImpl::unload(const std::shared_ptr<ShaderProgramImpl>& self) {
	service->threadGL->executeAsync([self] {
		self->unloadGL();
	}, priority);
}

bool ShaderProgramImpl::operator<(const ShaderProgramImpl& r) const {
	return name < r.name;
}

bool operator<(const std::string& name, const ShaderProgramImpl& r) {
	return name < r.name;
}

bool operator<(const ShaderProgramImpl& r, const std::string& name) {
	return r.name < name;
}

// ShaderProgram -----------------------------------------------------------------------------------

ShaderProgram::ShaderProgram(
		ServiceShader* const service,
		const std::string& name,
		const std::string& fsPath,
		const std::string& gsPath,
		const std::string& vsPath) {

	(void) name;
	(void) fsPath;
	(void) gsPath;
	(void) vsPath;

	//<<<
}

void ShaderProgram::callback(const std::shared_ptr<ShaderProgram>&) { }

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

} //namespace gl
} //namespace vl

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

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