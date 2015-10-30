// File: ShaderProgramImpl.hpp, Created on 2014. december 7. 12:56, Author: Vader

#pragma once

// vl
#include <vl/resource.hpp>
// std
#include <memory>
#include <ios>
// pro
#include "vl/gl/types.hpp"


namespace vl {
namespace gl {

class ShaderService;

// Attribute ---------------------------------------------------------------------------------------

constexpr uint32_t ATTRIBUTE_POSITION = 0;
constexpr uint32_t ATTRIBUTE_PSIZE = 1;
constexpr uint32_t ATTRIBUTE_NORMAL = 2;
constexpr uint32_t ATTRIBUTE_DIFFUSE = 3;
constexpr uint32_t ATTRIBUTE_COLOR0 = 3; //Same as DIFFUSE
constexpr uint32_t ATTRIBUTE_SPECULAR = 4;
constexpr uint32_t ATTRIBUTE_COLOR1 = 4; //Same as SPECULAR
constexpr uint32_t ATTRIBUTE_FOGCOORD = 5;
constexpr uint32_t ATTRIBUTE_TESSFACTOR = 5; //Same as FOG
constexpr uint32_t ATTRIBUTE_BONEWEIGHT = 6;
constexpr uint32_t ATTRIBUTE_BONEINDICES = 7;
constexpr uint32_t ATTRIBUTE_TEXCOORD0 = 8;
constexpr uint32_t ATTRIBUTE_TEXCOORD1 = 9;
constexpr uint32_t ATTRIBUTE_TEXCOORD2 = 10;
constexpr uint32_t ATTRIBUTE_TEXCOORD3 = 11;
constexpr uint32_t ATTRIBUTE_TEXCOORD4 = 12;
constexpr uint32_t ATTRIBUTE_TEXCOORD5 = 13;
constexpr uint32_t ATTRIBUTE_TEXCOORD6 = 14;
constexpr uint32_t ATTRIBUTE_TANGENT = 14; //Same as TEX6
constexpr uint32_t ATTRIBUTE_TEXCOORD7 = 15;
constexpr uint32_t ATTRIBUTE_BITANGENT = 15; //Same as TEX7

//enum class Attribute : uint32_t {
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

enum class TextureType : uint32_t {
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
	std::string path; // TODO P3: namespace fs = boost::filesystem;
	std::string text;
private:
	std::ios::iostate iostate;
private:
	ShaderService* shaderService;

public:
	TextFile(const std::string& path, ShaderService* shaderService);
	const std::string& getText() const {
		return text;
	}

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
	friend bool operator<(const std::string& path, const TextFile& r);
	friend bool operator<(const TextFile& r, const std::string& path);
};

// Shader --------------------------------------------------------------------------------------

class Shader : public vl::Resource {
private:
	const size_t priority = 0; //<<< Default priority
	std::string path;
	GLuint shaderID;
private:
	//status - compile
private:
	std::shared_ptr<TextFile> sourceFile;
	ShaderService* shaderService;

public:
	Shader(const std::string& path, ShaderService* shaderService);
	auto getShaderID() const {
		return shaderID;
	}
private:
	void loadGL();
	void unloadGL();

protected:
	void load(const std::shared_ptr<Shader>& self);
	void unload(const std::shared_ptr<Shader>& self);
public:
	bool operator<(const Shader& r) const;
	friend bool operator<(const std::string& path, const Shader& r);
	friend bool operator<(const Shader& r, const std::string& path);
};

// ShaderProgramImpl -----------------------------------------------------------------------------------

class ShaderProgramImpl : public vl::Resource {
private:
	const size_t priority = 0; //<<< Default priority
	std::string name;
	GLuint programID;
private:
	//status - link
private:
	std::shared_ptr<Shader> fs, gs, vs;
	ShaderService* shaderService;

public:
	ShaderProgramImpl(const std::string& name,
			const std::string& fsPath,
			const std::string& gsPath,
			const std::string& vsPath,
			ShaderService* shaderService);
private:
	void loadGL();
	void unloadGL();
	void callbackShaderLoaded(const std::shared_ptr<ShaderProgramImpl>&);

protected:
	void load(const std::shared_ptr<ShaderProgramImpl>& self);
	void unload(const std::shared_ptr<ShaderProgramImpl>& self);
public:
	bool operator<(const ShaderProgramImpl& r) const;
	friend bool operator<(const std::string& name, const ShaderProgramImpl& r);
	friend bool operator<(const ShaderProgramImpl& r, const std::string& name);
};

// ShaderProgram -----------------------------------------------------------------------------------

class ShaderProgram {
	GLuint shaderProgramID;
	ShaderService* shaderService;
	std::shared_ptr<ShaderProgramImpl> impl;
public:
	ShaderProgram(const std::string& name,
			const std::string& fsPath,
			const std::string& gsPath,
			const std::string& vsPath,
			ShaderService* shaderService);
private:
	void callback(const std::shared_ptr<ShaderProgram>&);
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl