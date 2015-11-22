// File: shader.hpp, Created on 2014. december 7. 12:56, Author: Vader

#pragma once

// ext
#include <boost/container/flat_map.hpp>
#include <GL/glew.h>
// std
#include <memory>



#include <iostream>
// -------------------------------------------------------------------------------------------------

namespace boost {
namespace filesystem {
struct path;
} //namespace filesystem
namespace asio {
struct const_buffer;
} //namespace asio
} //namespace boost

// -------------------------------------------------------------------------------------------------

namespace vl {
namespace gl {

// -------------------------------------------------------------------------------------------------

constexpr const char DEFAULT_SHADER_NAME[] = "--UNNAMED--";
constexpr const char DEFAULT_SHADER_VERTEX_NAME[] = "--UNNAMED-VERTEX--";
constexpr const char DEFAULT_SHADER_GEOMETRY_NAME[] = "--UNNAMED-GEOMETRY--";
constexpr const char DEFAULT_SHADER_FRAGMENT_NAME[] = "--UNNAMED-FRAGMENT--";

constexpr const char DEFAULT_SHADER_TYPE_NAME[] = "--default--";
constexpr const char SHADER_TYPE_VERTEX_NAME[] = "vertex";
constexpr const char SHADER_TYPE_GEOMETRY_NAME[] = "geometry";
constexpr const char SHADER_TYPE_FRAGMENT_NAME[] = "fragment";

// toString ----------------------------------------------------------------------------------------

const char* shaderTypeToString(GLenum type);

// Attribute ---------------------------------------------------------------------------------------

enum class Attribute : GLuint {
	position = 0,
	psize = 1,
	normal = 2,
	diffuse = 3,
	color0 = 3, //Same as DIFFUSE
	specular = 4,
	color1 = 4, //Same as SPECULAR
	fogcoord = 5,
	tessfactor = 5, //Same as FOG
	boneweight = 6,
	boneindices = 7,
	texcoord0 = 8,
	texcoord1 = 9,
	texcoord2 = 10,
	texcoord3 = 11,
	texcoord4 = 12,
	texcoord5 = 13,
	texcoord6 = 14,
	tangent = 14, //Same as TEX6
	texcoord7 = 15,
	bitangent = 15, //Same as TEX7
};

// TextureType -------------------------------------------------------------------------------------

//TODO P2: Consider moving this to an other file? (and remove the includes from texture.hpp)

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

// BaseShader --------------------------------------------------------------------------------------

/**
 * @note Non-virtual class
 */
class BaseShader {
private:
	GLuint shaderID = 0;
	const GLenum type;
	std::string name;

public:
	BaseShader(
			const boost::asio::const_buffer& data, const GLenum type, const std::string& name = DEFAULT_SHADER_NAME);
	BaseShader(
			const boost::filesystem::path& filePath, const GLenum type);
	BaseShader(
			const boost::filesystem::path& filePath, const GLenum type, const std::string& name);
	BaseShader(
			const char* data, size_t size, const GLenum type, const std::string& name = DEFAULT_SHADER_NAME);

	void init(const char* source);
private:
	void loadGL(const char* source);
	void unloadGL();

public:
	inline auto id() const {
		return shaderID;
	}

	~BaseShader();
};

// ShaderHelper ------------------------------------------------------------------------------------

namespace detail {

template<GLenum TYPE, const char* DEFAULT_NAME>
struct ShaderHelper : public BaseShader {
	ShaderHelper(const boost::asio::const_buffer& data, const std::string& name = DEFAULT_NAME) :
		BaseShader(data, TYPE, name) { }
	ShaderHelper(const boost::filesystem::path& filePath) :
		BaseShader(filePath, TYPE) { }
	ShaderHelper(const boost::filesystem::path& filePath, const std::string& name) :
		BaseShader(filePath, TYPE, name) { }
	ShaderHelper(const char* data, size_t size, const std::string& name = DEFAULT_NAME) :
		BaseShader(data, size, TYPE, name) { }
};

} //namespace detail

// ShaderAliases -----------------------------------------------------------------------------------

using ShaderVertex = detail::ShaderHelper<GL_VERTEX_SHADER, DEFAULT_SHADER_VERTEX_NAME>;
using ShaderGeometry = detail::ShaderHelper<GL_GEOMETRY_SHADER, DEFAULT_SHADER_GEOMETRY_NAME>;
using ShaderFragment = detail::ShaderHelper<GL_FRAGMENT_SHADER, DEFAULT_SHADER_FRAGMENT_NAME>;

// ShaderProgram -----------------------------------------------------------------------------------

class ShaderProgram {
	struct UniformInfo {
		GLint location;
		GLint index;
		GLint size;
		GLenum type;
	};
private:
	GLuint programID = 0;
	std::string name;

private:
//	boost::container::flat_map<std::string, UniformInfo> addressesAttribute;
	boost::container::flat_map<std::string, UniformInfo> addressesUniform;

private:
	std::shared_ptr<BaseShader> shaderVertex;
	std::shared_ptr<BaseShader> shaderGeometry;
	std::shared_ptr<BaseShader> shaderFragment;

public:
	ShaderProgram(
			const std::string& name,
			const std::shared_ptr<BaseShader>& shaderVertex,
			const std::shared_ptr<BaseShader>& shaderFragment) :
		ShaderProgram(name, shaderVertex, nullptr, shaderFragment) { }
	ShaderProgram(
			const std::string& name,
			const std::shared_ptr<BaseShader>& shaderVertex,
			const std::shared_ptr<BaseShader>& shaderGeometry,
			const std::shared_ptr<BaseShader>& shaderFragment);

	virtual ~ShaderProgram();

private:
	void loadGL();
	void mapAttributes();
	void mapUniforms();
	void unloadGL();

public:
	void use();
	//	inline GLuint id() const {
	//		return programID;
	//	}
	inline GLint getActiveUniformLocation(const std::string& name) const {
		try {
			return addressesUniform.at(name).location;
		} catch (const std::out_of_range& ex) { // <<< performance (it in an "init function" but still...)
			return -1;
		}
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace vl