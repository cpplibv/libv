// File: vgl.cpp, Created on 2014. november 25. 12:14, Author: Vader

// hpp
#include "vl/gl/vgl.hpp"
// ext
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace vl {
namespace gl {

glm::mat4& GL::matrixModel() {
	return matrixStackModel.top();
}

glm::mat4& GL::matrixView() {
	return matrixStackView.top();
}

glm::mat4& GL::matrixProjection() {
	return matrixStackProjection.top();
}

const glm::mat4& GL::matrixTexture() const {
	return matrixTexture_;
}

glm::mat4 GL::matrixMVP() {
	return matrixStackProjection.top() * matrixStackView.top() * matrixStackModel.top();
}

void GL::popMatrixModel() {
	matrixStackModel.pop();
}

void GL::popMatrixView() {
	matrixStackView.pop();
}

void GL::popMatrixProjection() {
	matrixStackProjection.pop();
}

void GL::pushMatrixModel() {
	matrixStackModel.push(matrixStackModel.top());
}

void GL::pushMatrixModel(const glm::mat4& m) {
	matrixStackModel.push(matrixStackModel.top() * m);
}

void GL::pushMatrixView() {
	matrixStackView.push(matrixStackView.top());
}

void GL::pushMatrixView(const glm::mat4& m) {
	matrixStackView.push(matrixStackView.top() * m);
}

void GL::pushMatrixProjection() {
	matrixStackProjection.push(matrixStackProjection.top());
}

void GL::pushMatrixProjection(const glm::mat4& m) {
	matrixStackProjection.push(matrixStackProjection.top() * m);
}

GL::GL() :
	matrixTexture_(0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0),
	matrixStackModel({glm::mat4(1.0f)}),
	matrixStackView({glm::mat4(1.0f)}),
	matrixStackProjection({glm::mat4(1.0f)}) {
}

//void renderCube(float x, float y, float z, float size) {
//	glBegin(GL_QUADS);
//
//	//Right
//	glNormal3f(1.0f, 0.0f, 0.0f);
//	glVertex3f(x + size, y + size, z + size);
//	glVertex3f(x + size, y - size, z + size);
//	glVertex3f(x + size, y - size, z - size);
//	glVertex3f(x + size, y + size, z - size);
//
//	//Left
//	glNormal3f(-1.0f, 0.0f, 0.0f);
//	glVertex3f(x - size, y - size, z - size);
//	glVertex3f(x - size, y - size, z + size);
//	glVertex3f(x - size, y + size, z + size);
//	glVertex3f(x - size, y + size, z - size);
//
//	//Top
//	glNormal3f(0.0f, 1.0f, 0.0f);
//	glVertex3f(x + size, y + size, z + size);
//	glVertex3f(x + size, y + size, z - size);
//	glVertex3f(x - size, y + size, z - size);
//	glVertex3f(x - size, y + size, z + size);
//
//	//Bottom
//	glNormal3f(0.0f, -1.0f, 0.0f);
//	glVertex3f(x - size, y - size, z - size);
//	glVertex3f(x + size, y - size, z - size);
//	glVertex3f(x + size, y - size, z + size);
//	glVertex3f(x - size, y - size, z + size);
//
//	//Near
//	glNormal3f(0.0f, 0.0f, -1.0f);
//	glVertex3f(x + size, y + size, z - size);
//	glVertex3f(x + size, y - size, z - size);
//	glVertex3f(x - size, y - size, z - size);
//	glVertex3f(x - size, y + size, z - size);
//
//	//Far
//	glNormal3f(0.0f, 0.0f, 1.0f);
//	glVertex3f(x + size, y + size, z + size);
//	glVertex3f(x - size, y + size, z + size);
//	glVertex3f(x - size, y - size, z + size);
//	glVertex3f(x + size, y - size, z + size);
//
//	glEnd();
//}
//
//void renderXYZ(float x, float y, float z, float size) {
//	glBegin(GL_LINES);
//	glColor3f(1, 0, 0);
//	glVertex3f(x, y, z);
//	glVertex3f(x + size, y, z);
//	glColor3f(0, 1, 0);
//	glVertex3f(x, y, z);
//	glVertex3f(x, y + size, z);
//	glColor3f(0, 0, 1);
//	glVertex3f(x, y, z);
//	glVertex3f(x, y, z + size);
//	glEnd();
//	glColor3f(1, 1, 1);
//}

} //namespace gl
} //namespace vl