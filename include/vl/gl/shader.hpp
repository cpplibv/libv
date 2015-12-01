// File: shader.hpp, Created on 2014. december 7. 12:56, Author: Vader

#pragma once

// ext
#include <boost/container/flat_map.hpp>
#include <GL/glew.h>
// std
#include <memory>

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
			const char* data, const size_t size, const GLenum type, const std::string& name = DEFAULT_SHADER_NAME);

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
	ShaderHelper(const char* data, const size_t size, const std::string& name = DEFAULT_NAME) :
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