// File: vgl.hpp, Created on 2014. november 25. 12:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
#include <glm/glm.hpp>
// libv
#include <libv/vec.hpp>
// std
#include <stack>
// pro
#include <libv/gl/enum.hpp>
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

// Methods for matrix stack
//	Mat4& Translate(const Vec3& v);
//	Mat4& Scale(const Vec3& v);
//
//	Mat4& RotateX(float ang);
//	Mat4& RotateY(float ang);
//	Mat4& RotateZ(float ang);
//	Mat4& Rotate(const Vec3& axis, float ang);
//
//	Mat4 Transpose() const;
//
//	float Determinant() const;
//	Mat4 Inverse() const;
//
//	static Mat4 Frustum(float left, float right, float bottom, float top, float near, float far);
//	static Mat4 Perspective(float fovy, float aspect, float near, float far);
//	static Mat4 Ortho(float left, float right, float bottom, float top, float near, float far);
//	static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
//
//	static Vec3 UnProject(const Vec3& vec, const Mat4& view, const Mat4& proj, const float viewport[]);
//	static Vec3 Project(const Vec3& vec, const Mat4& view, const Mat4& proj, const float viewport[]);




public:
	inline GLint getMaxUniformBlockSize() const {
		GLint n;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &n);
		LIBV_GL_DEBUG_CHECK_GL();
		return n;
	}
	inline GLint getMaxUniformBufferBindings() const{
		GLint n;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &n);
		LIBV_GL_DEBUG_CHECK_GL();
		return n;
	}
	inline GLint getMaxVertexAttribs() const{
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

public:
	inline void activeTexture(TextureChannel channel) {
		glActiveTexture(GL_TEXTURE0 + to_value(channel));
		LIBV_GL_DEBUG_CHECK_GL();
	}

public:
//	inline void drawArrays(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t offset);
	inline void drawElements(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset);
	inline void drawElementsBaseVertex(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset, size_t vertexOffset);
	inline void viewport(ivec2 pos, ivec2 size) {
		glViewport(pos.x, pos.y, size.x, size.y);
	}

//	void Enable(Capability::capability_t capability);
//	void Disable(Capability::capability_t capability);
//
//	void ClearColor(const Color& col);
//	void Clear(Buffer::buffer_t buffers = Buffer::Color | Buffer::Depth);
//
//	void DepthMask(bool writeEnabled);
//	void StencilMask(bool writeEnabled);
//	void StencilMask(uint mask);
//
//	void StencilFunc(TestFunction::test_function_t function, int reference, uint mask = ~0);
//	void StencilOp(StencilAction::stencil_action_t fail, StencilAction::stencil_action_t zfail, StencilAction::stencil_action_t pass);
//
//	void UseProgram(const Program& program);
//
//	void BindTexture(const Texture& texture, uchar unit);
//
//	void BindFramebuffer(const Framebuffer& framebuffer);
//	void BindFramebuffer();
//
//	void BeginTransformFeedback(Primitive::primitive_t mode);
//	void EndTransformFeedback();

	GL() :
		matrixTexture_(0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0),
		matrixStackModel({glm::mat4(1.0f)}),
	matrixStackView({glm::mat4(1.0f)}),
	matrixStackProjection({glm::mat4(1.0f)}) { }
};

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
	glDrawElements(to_value(mode), vertexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof (GLuint) * indexOffset));
	LIBV_GL_DEBUG_CHECK_GL();
	glBindVertexArray(0);
}

inline void GL::drawElementsBaseVertex(const BaseVertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset, size_t vertexOffset) {
	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
	glBindVertexArray(vao);
	glDrawElementsBaseVertex(to_value(mode), vertexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof (GLuint) * indexOffset), vertexOffset);
	LIBV_GL_DEBUG_CHECK_GL();
	glBindVertexArray(0);
}

// Debug -------------------------------------------------------------------------------------------

void renderCube(float x, float y, float z, float size);

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv
