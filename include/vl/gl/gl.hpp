// File: vgl.hpp, Created on 2014. november 25. 12:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
#include <glm/glm.hpp>
// vl
#include <vl/worker_thread.hpp>
// std
#include <stack>

#define PI 3.1415926535897932384626433832795

namespace vl {
namespace gl {

class GL {
private:
	glm::mat4 matrixTexture_;
	std::stack<glm::mat4> matrixStackModel;
	std::stack<glm::mat4> matrixStackView;
	std::stack<glm::mat4> matrixStackProjection;
public:
	glm::mat4& matrixModel();
	glm::mat4& matrixView();
	glm::mat4& matrixProjection();
	const glm::mat4& matrixTexture() const ;
	glm::mat4 matrixMVP();

	void popMatrixModel();
	void popMatrixView();
	void popMatrixProjection();

	void pushMatrixModel();
	void pushMatrixModel(const glm::mat4&);
	void pushMatrixView();
	void pushMatrixView(const glm::mat4&);
	void pushMatrixProjection();
	void pushMatrixProjection(const glm::mat4&);

	GL();
};

void renderCube(float x, float y, float z, float size);

} //namespace gl
} //namespace vl
