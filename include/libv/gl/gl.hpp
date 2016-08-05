// File: vgl.hpp, Created on 2014. november 25. 12:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
// libv
#include <libv/vec.hpp>
// std
#include <stack>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/guard.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/vertex_buffer.hpp>

namespace libv {
namespace gl {

class BaseVertexArray;

// -------------------------------------------------------------------------------------------------

const float PI = 3.14159265358979323846f;
inline float rad(float degrees) {
	return degrees / 180.0f * PI;
}
inline float deg(float radians) {
	return radians / PI * 180.0f;
}

// -------------------------------------------------------------------------------------------------

class MatrixStack {
private:
	std::stack<glm::mat4> stack;

public:
	inline MatrixStack();

	inline MatrixStack& push();
	inline MatrixStack& push(const glm::mat4&);
	inline StackGuard<MatrixStack> pushGuard();
	inline StackGuard<MatrixStack> pushGuard(const glm::mat4&);
	inline MatrixStack& pop();

	inline glm::mat4& top();
	inline const glm::mat4& top() const;

	inline MatrixStack& translate(const glm::vec3& v);
	inline MatrixStack& translate(float x, float y, float z);
	inline MatrixStack& scale(const glm::vec3& v);
	inline MatrixStack& scale(float x, float y, float z);
	inline MatrixStack& rotateX(float ang);
	inline MatrixStack& rotateY(float ang);
	inline MatrixStack& rotateZ(float ang);
	inline MatrixStack& rotate(float angle, const glm::vec3& axis);
	inline MatrixStack& rotate(const glm::quat& quat);
	inline MatrixStack& identity();
	inline MatrixStack& frustum(float left, float right, float bottom, float top, float near, float far);
	inline MatrixStack& perspective(float fovy, float aspect, float near, float far);
	inline MatrixStack& ortho(float left, float right, float bottom, float top);
	inline MatrixStack& ortho(float left, float right, float bottom, float top, float near, float far);
	inline MatrixStack& lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
	inline MatrixStack& multiply(const glm::mat4& m);
	inline MatrixStack& operator*=(const glm::mat4& m);

	inline glm::vec3 eye() const;
	inline glm::mat4 operator*(const glm::mat4& m) const;
	inline glm::vec4 operator*(const glm::vec4& v) const;
	inline glm::mat4 operator*(const MatrixStack& ms) const;
	friend inline glm::mat4 operator*(const glm::mat4& m, const MatrixStack& ms);
	inline operator glm::mat4() const;
	inline operator glm::mat4&();
	inline operator const glm::mat4&() const;

	inline glm::mat4 transpose() const;
	inline float determinant() const;
	inline glm::mat4 inverse() const;

	inline MatrixStack& setToFrustum(float left, float right, float bottom, float top, float near, float far);
	inline MatrixStack& setToPerspective(float fovy, float aspect, float near, float far);
	inline MatrixStack& setToOrtho(float left, float right, float bottom, float top);
	inline MatrixStack& setToOrtho(float left, float right, float bottom, float top, float near, float far);
	inline MatrixStack& setToLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
};

// -------------------------------------------------------------------------------------------------

class GL {
private:
	glm::mat4 matrixTexture_; // Make it static constexpr...
public:
	MatrixStack model;
	MatrixStack view;
	MatrixStack projection;

public:
	inline glm::mat4 mvp() const {
		return projection * view * model;
	}
	inline const glm::mat4& getMatrixTexture() const {
		return matrixTexture_;
	}

public:
	inline GLint getMaxUniformBlockSize() const {
		GLint n;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &n);
		LIBV_GL_DEBUG_CHECK_GL();
		return n;
	}
	inline GLint getMaxUniformBufferBindings() const {
		GLint n;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &n);
		LIBV_GL_DEBUG_CHECK_GL();
		return n;
	}
	inline GLint getMaxVertexAttribs() const {
		GLint n;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
		LIBV_GL_DEBUG_CHECK_GL();
		return n;
	}
	inline GLint getMaxVertexUniformComponents() const {
		GLint n;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &n);
		LIBV_GL_DEBUG_CHECK_GL();
		return n;
	}
	inline GLint getMaxCombinedTextureImageUnits() const {
		GLint n;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &n);
		LIBV_GL_DEBUG_CHECK_GL();
		return n;
	}

public:
	inline void activeTexture(TextureChannel channel);
	//	inline void drawArrays(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t offset);
	inline void drawElements(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset);
	inline void drawElementsBaseVertex(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset, size_t vertexOffset);
	inline void viewport(ivec2 pos, ivec2 size);

	inline void enable(Capability capability);
	inline void disable(Capability capability);
	inline EnableGuard<GL, Capability> enableGuard(Capability capability);
	inline DisableGuard<GL, Capability> disableGuard(Capability capability);

	inline void clearColor(const glm::vec4& col);
	inline void clearColor(float r, float g, float b, float a = 1.0f);
	inline void clear(Buffer buffers = Buffer::Color | Buffer::Depth);

	inline void frontFaceCCW();
	inline void frontFaceCW();
	//	void depthMask(bool writeEnabled);
	//	void stencilMask(bool writeEnabled);
	//	void stencilMask(uint mask);
	//
	//	void stencilFunc(TestFunction::test_function_t function, int reference, uint mask = ~0);
	//	void stencilOp(StencilAction::stencil_action_t fail, StencilAction::stencil_action_t zfail, StencilAction::stencil_action_t pass);
	//
	//	void useProgram(const Program& program);
	//
	//	void bindTexture(const Texture& texture, uchar unit);
	//
	//	void bindFramebuffer(const Framebuffer& framebuffer);
	//	void bindFramebuffer();
	//
	//	void beginTransformFeedback(Primitive::primitive_t mode);
	//	void endTransformFeedback();
	GL() :
		matrixTexture_(0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0) { }
};

// Debug -------------------------------------------------------------------------------------------

void renderCube(float x, float y, float z, float size);

// -------------------------------------------------------------------------------------------------

// =================================================================================================
inline MatrixStack::MatrixStack() {
	stack.emplace(1.0f);
}
inline MatrixStack& MatrixStack::push() {
	stack.push(stack.top());
	return *this;
}
inline MatrixStack& MatrixStack::push(const glm::mat4& mx) {
	stack.push(mx);
	return *this;
}
inline StackGuard<MatrixStack> MatrixStack::pushGuard() {
	return StackGuard<MatrixStack>(*this, top());
}
inline StackGuard<MatrixStack> MatrixStack::pushGuard(const glm::mat4& mx) {
	return StackGuard<MatrixStack>(*this, mx);
}
inline MatrixStack& MatrixStack::pop() {
	LIBV_GL_DEBUG_ASSERT(stack.size() > 0);
	stack.pop();
	return *this;
}
inline glm::mat4& MatrixStack::top() {
	return stack.top();
}
inline const glm::mat4& MatrixStack::top() const {
	return stack.top();
}
inline MatrixStack& MatrixStack::translate(const glm::vec3& v) {
	stack.top() = glm::translate(stack.top(), v);
	return *this;
}
inline MatrixStack& MatrixStack::translate(float x, float y, float z) {
	stack.top() = glm::translate(stack.top(), glm::vec3(x, y, z));
	return *this;
}
inline MatrixStack& MatrixStack::scale(const glm::vec3& v) {
	stack.top() = glm::scale(stack.top(), v);
	return *this;
}
inline MatrixStack& MatrixStack::scale(float x, float y, float z) {
	stack.top() = glm::scale(stack.top(), glm::vec3(x, y, z));
	return *this;
}
inline MatrixStack& MatrixStack::rotateX(float ang) {
	stack.top() = glm::rotate(stack.top(), ang, glm::vec3(1, 0, 0));
	return *this;
}
inline MatrixStack& MatrixStack::rotateY(float ang) {
	stack.top() = glm::rotate(stack.top(), ang, glm::vec3(0, 1, 0));
	return *this;
}
inline MatrixStack& MatrixStack::rotateZ(float ang) {
	stack.top() = glm::rotate(stack.top(), ang, glm::vec3(0, 0, 1));
	return *this;
}
inline MatrixStack& MatrixStack::rotate(float ang, const glm::vec3& axis) {
	stack.top() = glm::rotate(stack.top(), ang, axis);
	return *this;
}
inline MatrixStack& MatrixStack::rotate(const glm::quat& quat) {
	stack.top() *= glm::mat4_cast(quat);
	return *this;
}
inline MatrixStack& MatrixStack::identity() {
	stack.top() = glm::mat4(1.0f);
	return *this;
}
inline MatrixStack& MatrixStack::frustum(float left, float right, float bottom, float top, float near, float far) {
	stack.top() *= glm::frustum(left, right, bottom, top, near, far);
	return *this;
}
inline MatrixStack& MatrixStack::perspective(float fovy, float aspect, float near, float far) {
	stack.top() *= glm::perspective(fovy, aspect, near, far);
	return *this;
}
inline MatrixStack& MatrixStack::ortho(float left, float right, float bottom, float top) {
	stack.top() *= glm::ortho(left, right, bottom, top);
	return *this;
}
inline MatrixStack& MatrixStack::ortho(float left, float right, float bottom, float top, float near, float far) {
	stack.top() *= glm::ortho(left, right, bottom, top, near, far);
	return *this;
}
inline MatrixStack& MatrixStack::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
	stack.top() *= glm::lookAt(eye, center, up);
	return *this;
}
inline MatrixStack& MatrixStack::multiply(const glm::mat4& m) {
	stack.top() *= m;
	return *this;
}
inline MatrixStack& MatrixStack::operator*=(const glm::mat4& m) {
	stack.top() *= m;
	return *this;
}
inline glm::vec3 MatrixStack::eye() const {
	return glm::vec3(glm::inverse(stack.top())[3]);
}
inline glm::mat4 MatrixStack::operator*(const glm::mat4& m) const {
	return stack.top() * m;
}
inline glm::vec4 MatrixStack::operator*(const glm::vec4& v) const {
	return stack.top() * v;
}
inline glm::mat4 MatrixStack::operator*(const MatrixStack& ms) const {
	return stack.top() * ms.top();
}
inline glm::mat4 operator*(const glm::mat4& m, const MatrixStack& ms) {
	return m * ms.top();
}
inline MatrixStack::operator glm::mat4() const {
	return stack.top();
}
inline MatrixStack::operator glm::mat4&() {
	return stack.top();
}
inline MatrixStack::operator const glm::mat4&() const {
	return stack.top();
}
inline glm::mat4 MatrixStack::transpose() const {
	return glm::transpose(stack.top());
}
inline float MatrixStack::determinant() const {
	return glm::determinant(stack.top());
}
inline glm::mat4 MatrixStack::inverse() const {
	return glm::inverse(stack.top());
}
inline MatrixStack& MatrixStack::setToFrustum(float left, float right, float bottom, float top, float near, float far) {
	stack.top() = glm::frustum(left, right, bottom, top, near, far);
	return *this;
}
inline MatrixStack& MatrixStack::setToPerspective(float fovy, float aspect, float near, float far) {
	stack.top() = glm::perspective(fovy, aspect, near, far);
	return *this;
}
inline MatrixStack& MatrixStack::setToOrtho(float left, float right, float bottom, float top) {
	stack.top() = glm::ortho(left, right, bottom, top);
	return *this;
}
inline MatrixStack& MatrixStack::setToOrtho(float left, float right, float bottom, float top, float near, float far) {
	stack.top() = glm::ortho(left, right, bottom, top, near, far);
	return *this;
}
inline MatrixStack& MatrixStack::setToLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
	stack.top() = glm::lookAt(eye, center, up);
	return *this;
}

// =================================================================================================
inline void GL::activeTexture(TextureChannel channel) {
	glActiveTexture(GL_TEXTURE0 + to_value(channel));
	LIBV_GL_DEBUG_CHECK_GL();
}
//inline void GL::drawArrays(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t offset) {
//	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
//	glBindVertexArray(vao);
//	glDrawArrays(to_value(mode), offset, vertexCount);
//	LIBV_GL_DEBUG_CHECK_GL();
//	glBindVertexArray(0);
//}
inline void GL::drawElements(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset) {
	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
	glBindVertexArray(vao);
	glDrawElements(to_value(mode), vertexCount, GL_UNSIGNED_INT, reinterpret_cast<void*> (sizeof (GLuint) * indexOffset));
	LIBV_GL_DEBUG_CHECK_GL();
	glBindVertexArray(0);
}
inline void GL::drawElementsBaseVertex(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset, size_t vertexOffset) {
	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
	glBindVertexArray(vao);
	glDrawElementsBaseVertex(to_value(mode), vertexCount, GL_UNSIGNED_INT, reinterpret_cast<void*> (sizeof (GLuint) * indexOffset), vertexOffset);
	LIBV_GL_DEBUG_CHECK_GL();
	glBindVertexArray(0);
}
inline void GL::viewport(ivec2 pos, ivec2 size) {
	glViewport(pos.x, pos.y, size.x, size.y);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void GL::enable(Capability capability) {
	glEnable(to_value(capability));
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void GL::disable(Capability capability) {
	glDisable(to_value(capability));
	LIBV_GL_DEBUG_CHECK_GL();
}
inline EnableGuard<GL, Capability> GL::enableGuard(Capability capability) {
	return EnableGuard<GL, Capability>(*this, capability);
}
inline DisableGuard<GL, Capability> GL::disableGuard(Capability capability) {
	return DisableGuard<GL, Capability>(*this, capability);
}
inline void GL::clearColor(const glm::vec4& col) {
	clearColor(col.r, col.g, col.b, col.a);
}
inline void GL::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void GL::clear(Buffer buffers) {
	glClear(to_value(buffers));
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void GL::frontFaceCCW() {
	glFrontFace(GL_CCW);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void GL::frontFaceCW() {
	glFrontFace(GL_CW);
	LIBV_GL_DEBUG_CHECK_GL();
}

} //namespace gl
} //namespace libv
