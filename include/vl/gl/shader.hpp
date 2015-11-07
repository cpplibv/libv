// File: shader.hpp, Created on 2014. december 7. 12:56, Author: Vader

#pragma once

// vl
#include <vl/resource.hpp>
// std
#include <memory>
#include <ios>
// pro
#include <vl/gl/types.hpp>

namespace vl {
namespace gl {

class ServiceShader;

// Attribute ---------------------------------------------------------------------------------------

constexpr GLuint ATTRIBUTE_POSITION = 0;
constexpr GLuint ATTRIBUTE_PSIZE = 1;
constexpr GLuint ATTRIBUTE_NORMAL = 2;
constexpr GLuint ATTRIBUTE_DIFFUSE = 3;
constexpr GLuint ATTRIBUTE_COLOR0 = 3; //Same as DIFFUSE
constexpr GLuint ATTRIBUTE_SPECULAR = 4;
constexpr GLuint ATTRIBUTE_COLOR1 = 4; //Same as SPECULAR
constexpr GLuint ATTRIBUTE_FOGCOORD = 5;
constexpr GLuint ATTRIBUTE_TESSFACTOR = 5; //Same as FOG
constexpr GLuint ATTRIBUTE_BONEWEIGHT = 6;
constexpr GLuint ATTRIBUTE_BONEINDICES = 7;
constexpr GLuint ATTRIBUTE_TEXCOORD0 = 8;
constexpr GLuint ATTRIBUTE_TEXCOORD1 = 9;
constexpr GLuint ATTRIBUTE_TEXCOORD2 = 10;
constexpr GLuint ATTRIBUTE_TEXCOORD3 = 11;
constexpr GLuint ATTRIBUTE_TEXCOORD4 = 12;
constexpr GLuint ATTRIBUTE_TEXCOORD5 = 13;
constexpr GLuint ATTRIBUTE_TEXCOORD6 = 14;
constexpr GLuint ATTRIBUTE_TANGENT = 14; //Same as TEX6
constexpr GLuint ATTRIBUTE_TEXCOORD7 = 15;
constexpr GLuint ATTRIBUTE_BITANGENT = 15; //Same as TEX7

//enum class Attribute : GLuint {
//	position = 0,
//	psize = 1,
//	normal = 2,
//	diffuse = 3,
//	color0 = 3, //Same as DIFFUSE
//	specular = 4,
//	color1 = 4, //Same as SPECULAR
//	fogcoord = 5,
//	tessfactor = 5, //Same as FOG
//	boneweight = 6,
//	boneindices = 7,
//	texcoord0 = 8,
//	texcoord1 = 9,
//	texcoord2 = 10,
//	texcoord3 = 11,
//	texcoord4 = 12,
//	texcoord5 = 13,
//	texcoord6 = 14,
//	tangent = 14, //Same as TEX6
//	texcoord7 = 15,
//	bitangent = 15, //Same as TEX7
//};

// TextureType -------------------------------------------------------------------------------------

//TODO P2: Consider moving this to an other file? (and remove the includes from texture.hpp-s)

enum class TextureType : GLuint {
	diffuse = 0,
	normal = 1,
	specular = 2,
	emission = 3,
	environment = 4,
	pass = 5,
	ambient = 6,
	//	_7 = 7,
	//	_8 = 8,
	//	_9 = 9,
	shadow0 = 10,
	shadow1 = 11,
	shadow2 = 12,
	shadow3 = 13,
	shadow4 = 14,
	shadow5 = 15,
	shadow6 = 16,
	shadow7 = 17,
	//	_18 = 18,
	//	_19 = 19,
	//	_20 = 20,
};

// TextFile --------------------------------------------------------------------------------------

class TextFile : public std::enable_shared_from_this<TextFile>, public vl::Resource {
private:
	const size_t priority = 0; //<<< Default priority
	std::string filePath; //TODO P3: namespace fs = boost::filesystem;
	std::string text;
private:
	std::ios::iostate iostate;
private:
	ServiceShader* service;

public:
	TextFile(ServiceShader * const service, const std::string& filePath);
	inline const std::string& getText() const {
		return text;
	}
	virtual ~TextFile();

private:
	void loadIO();
	void unloadIO();
public:
	void reload();

protected:
	void load(const std::shared_ptr<TextFile>& self);
	void unload(const std::shared_ptr<TextFile>& self);
public:
	bool operator<(const TextFile& r) const;
	friend bool operator<(const std::string& filePath, const TextFile& r);
	friend bool operator<(const TextFile& r, const std::string& filePath);
};

// Shader --------------------------------------------------------------------------------------

class Shader : public vl::Resource {
private:
	const size_t priority = 0; //<<< Default priority
	std::string filePath;
	GLuint shaderID;
private:
	//status - compile
private:
	std::shared_ptr<TextFile> sourceFile;
	ServiceShader* service;

public:
	Shader(ServiceShader * const service, const std::string& filePath);
	inline auto getShaderID() const {
		return shaderID;
	}
	virtual ~Shader();

private:
	void loadGL();
	void unloadGL();

protected:
	void load(const std::shared_ptr<Shader>& self);
	void unload(const std::shared_ptr<Shader>& self);
public:
	bool operator<(const Shader& r) const;
	friend bool operator<(const std::string& filePath, const Shader& r);
	friend bool operator<(const Shader& r, const std::string& filePath);
};

// ShaderProgram -----------------------------------------------------------------------------------

class ShaderProgram : public vl::Resource {
private:
	const size_t priority = 0; //<<< Default priority
	std::string name;
	GLuint programID;
private:
	//status - link
private:
	std::shared_ptr<Shader> fs, gs, vs;
	ServiceShader* service;

public:
	ShaderProgram(
			ServiceShader * const service,
			const std::string& name,
			const std::string& fsPath,
			const std::string& gsPath,
			const std::string& vsPath);
	virtual ~ShaderProgram();

private:
	void loadGL();
	void unloadGL();

protected:
	void load(const std::shared_ptr<ShaderProgram>& self);
	void unload(const std::shared_ptr<ShaderProgram>& self);
public:
	bool operator<(const ShaderProgram& r) const;
	friend bool operator<(const std::string& name, const ShaderProgram& r);
	friend bool operator<(const ShaderProgram& r, const std::string& name);
};

// ShaderProgram -----------------------------------------------------------------------------------

class ShaderProgramProxy {
	//	GLuint shaderProgramID = 0;
	std::shared_ptr<ShaderProgram> impl;
public:
	ShaderProgramProxy(
			ServiceShader * const service,
			const std::string& name,
			const std::string& fsPath,
			const std::string& vsPath);

	ShaderProgramProxy(
			ServiceShader * const service,
			const std::string& name,
			const std::string& fsPath,
			const std::string& gsPath,
			const std::string& vsPath);
	virtual ~ShaderProgramProxy() { }

};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl