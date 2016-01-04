// File: vgl.hpp, Created on 2014. november 25. 12:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
#include <glm/glm.hpp>
// libv
#include <libv/worker_thread.hpp>
#include <libv/vec.hpp>
// std
#include <stack>

//#include "types.hpp"

#define PI 3.1415926535897932384626433832795

namespace libv {
namespace gl {

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

// VertexAttribType --------------------------------------------------------------------------------

enum class VertexAttribType : GLenum {
	//glVertexAttribPointer
	//glVertexAttribIPointer
	BYTE = GL_BYTE,
	UBYTE = GL_UNSIGNED_BYTE,
	SHORT = GL_SHORT,
	USHORT = GL_UNSIGNED_SHORT,
	INT = GL_INT,
	UINT = GL_UNSIGNED_INT,

	//glVertexAttribPointer
	HFLOAT = GL_HALF_FLOAT,
	FLOAT = GL_FLOAT,
	FIXED = GL_FIXED,
	INT_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
	UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
	UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV,

	//glVertexAttribPointer
	//glVertexAttribLPointer
	DOUBLE = GL_DOUBLE
};

// -------------------------------------------------------------------------------------------------

class GL {
private:
	glm::mat4 matrixTexture_; // Make it static constexpr...
	std::stack<glm::mat4> matrixStackModel;
	std::stack<glm::mat4> matrixStackView;
	std::stack<glm::mat4> matrixStackProjection;

public:
	inline glm::mat4& matrixModel() {
		return matrixStackModel.top();
	}
	inline glm::mat4& matrixView() {
		return matrixStackView.top();
	}
	inline glm::mat4& matrixProjection() {
		return matrixStackProjection.top();
	}
	inline const glm::mat4& matrixTexture() const {
		return matrixTexture_;
	}
	inline glm::mat4 matrixMVP() const {
		return matrixStackProjection.top() * matrixStackView.top() * matrixStackModel.top();
	}
	inline void popMatrixModel() {
		matrixStackModel.pop();
	}
	inline void popMatrixView() {
		matrixStackView.pop();
	}
	inline void popMatrixProjection() {
		matrixStackProjection.pop();
	}
	inline void pushMatrixModel() {
		matrixStackModel.push(matrixStackModel.top());
	}
	inline void pushMatrixModel(const glm::mat4& m) {
		matrixStackModel.push(matrixStackModel.top() * m);
	}
	inline void pushMatrixView() {
		matrixStackView.push(matrixStackView.top());
	}
	inline void pushMatrixView(const glm::mat4& m) {
		matrixStackView.push(matrixStackView.top() * m);
	}
	inline void pushMatrixProjection() {
		matrixStackProjection.push(matrixStackProjection.top());
	}
	inline void pushMatrixProjection(const glm::mat4& m) {
		matrixStackProjection.push(matrixStackProjection.top() * m);
	}
	GL() :
		matrixTexture_(0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0),
		matrixStackModel({glm::mat4(1.0f)}),
	matrixStackView({glm::mat4(1.0f)}),
	matrixStackProjection({glm::mat4(1.0f)}) { }
};

// -------------------------------------------------------------------------------------------------
inline bool getShaderCompileStatus(GLuint shaderID) {
	GLint result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	return result;
}
inline std::string getShaderCompileMessage(GLuint shaderID) {
	int infoLength;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLength);

	std::string result;
	result.resize(infoLength);
	glGetShaderInfoLog(shaderID, infoLength, nullptr, &result[0]);
	return result;
}

// -------------------------------------------------------------------------------------------------
inline bool getProgamLinkStatus(GLuint programID) {
	GLint result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	return result;
}
inline std::string getProgamLinkMessage(GLuint programID) {
	int infoLength;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);

	std::string result;
	result.resize(infoLength);
	glGetProgramInfoLog(programID, infoLength, nullptr, &result[0]);
	return result;
}

// glViewport --------------------------------------------------------------------------------------
inline void viewport(::libv::ivec2 pos, ::libv::ivec2 size) {
	glViewport(pos.x, pos.y, size.x, size.y);
}

// glActiveTexture ---------------------------------------------------------------------------------
inline void activeTexture(const ::libv::gl::TextureType& textureType) {
	glActiveTexture(GL_TEXTURE0 + static_cast<GLuint>(textureType));
}

// glEnableVertexAttribArray -----------------------------------------------------------------------
inline void enableVertexAttribArray(const ::libv::gl::Attribute& attribute) {
	glEnableVertexAttribArray(static_cast<GLuint>(attribute));
}

// glVertexAttribPointer ---------------------------------------------------------------------------
inline void vertexAttribPointer(
		const ::libv::gl::Attribute& attribute,
		GLint size,
		const ::libv::gl::VertexAttribType& type,
		GLboolean normalized,
		GLsizei stride,
		const GLvoid* pointer) {
	glVertexAttribPointer(
			static_cast<GLuint> (attribute),
			size,
			static_cast<GLenum> (type),
			normalized,
			stride,
			pointer);
}
inline void vertexAttribIPointer(
		const ::libv::gl::Attribute& attribute,
		GLint size,
		const ::libv::gl::VertexAttribType& type,
		GLsizei stride,
		const GLvoid* pointer) {
	glVertexAttribIPointer(
			static_cast<GLuint> (attribute),
			size,
			static_cast<GLenum> (type),
			stride,
			pointer);
}
inline void vertexAttribLPointer(
		const ::libv::gl::Attribute& attribute,
		GLint size,
		GLsizei stride,
		const GLvoid* pointer) {
	glVertexAttribLPointer(
			static_cast<GLuint> (attribute),
			size,
			static_cast<GLenum> (libv::gl::VertexAttribType::DOUBLE),
			stride,
			pointer);
}

// =================================================================================================

// Debug -------------------------------------------------------------------------------------------

void renderCube(float x, float y, float z, float size);

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv
