// File: Shader.hpp, Created on 2014. december 7. 12:56, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// vl
#include <vl/resource.hpp>
// std
#include <fstream>
#include <map>
#include <memory>
#include <vector>
// pro
#include <vl/gl/log.hpp>

namespace vl {
namespace gl {

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

enum class TextureType : uint32_t {
	diffuse = 0,
	normal = 1,
	specular = 2,
	emission = 3,
	environment = 4,
	pass = 5,
	ambient = 6,
	// =  7,
	// =  8,
	// =  9,
	shadow0 = 10,
	shadow1 = 11,
	shadow2 = 12,
	shadow3 = 13,
	shadow4 = 14,
	shadow5 = 15,
	shadow6 = 16,
	shadow7 = 17,
	// = 18,
	// = 19,
	// = 20
};

// Shader --------------------------------------------------------------

class ShaderFile {
	std::string path;
	std::string text;
public:
	ShaderFile(std::string path) : path(path) {
		std::ifstream file(path, std::ios::in);
		if (file) {
			std::ostringstream buffer;
			buffer << file.rdbuf();
			file.close();
			text = buffer.str();
		} else
			VLOG_ERROR(vl::gl::log(), "Failed to open file: [%s]", text);

		//std::ifstream shaderFile(path, std::ios::in);
		//if (shaderFile.is_open()) {
		//	std::string line;
		//	while (std::getline(shaderFile, line))
		//		text += "\n" + line;
		//	shaderFile.close();
		//} else
		//	VLOG_ERROR(vl::gl::log(), std::string("Failed to open file: ") + text);
	}
};

class Shader {
	Resource<ShaderFile> fs, gs, vs;
public:
	Shader() { }
};

Resource<Shader> toonShader;

//class Shader {
//private:
//	static int nextUniformIndex;
//	static std::vector<std::string>& uniforms();
//	static Shader* currentShader;
//public:
//	static int registerUniform(const std::string& name);
//	static GLint getUniformLocation(int name);
//private:
//	std::string name;
//	std::unique_ptr<int[] > uniformLocs;
//	std::string fs, gs, vs;
//	GLuint id;
//public:
//	Shader(const std::string& name, const std::string& vs, const std::string& fs);
//	Shader(const std::string& name, const std::string& vs, const std::string& gs, const std::string& fs);
//	void useProgram();
//	void loadProgram();
//	void unloadProgram();
//	void reloadProgram();
//	virtual ~Shader();
//
//public:
//	void printActiveUniforms();
//};

} //namespace gl
} //namespace vl

// -----------------------------------------------------------------------------
//
//void vglUseShader(Shader s);
//void vglReloadShader(Shader s);
//Shader vglLoadShader(string vs, string fs);
//Shader vglLoadShader(string vs, string gs, string fs);
//void vglDeleteShader(Shader s);
