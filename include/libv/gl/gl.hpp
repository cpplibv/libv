// File: vgl.hpp, Created on 2014. november 25. 12:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
// std
#include <stack>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/guard.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/vertex_buffer.hpp>

namespace libv {
namespace gl {

class VertexArray;

// -------------------------------------------------------------------------------------------------

const float PI = 3.14159265358979323846f;
inline float rad(float degrees) {
	return degrees / 180.0f * PI;
}
inline float deg(float radians) {
	return radians / PI * 180.0f;
}

// -------------------------------------------------------------------------------------------------

template <typename Mat>
class MatrixStack {
private:
	std::stack<Mat> stack;

public:
	using value_type = Mat;
	using T = typename Mat::value_type;

public:
	inline MatrixStack() {
		stack.emplace(static_cast<T>(1.0));
	}
	inline MatrixStack<Mat>& push() {
		stack.push(stack.top());
		return *this;
	}
	inline MatrixStack<Mat>& push(const Mat& mx) {
		stack.push(*this);
		stack.top() = mx;
		return *this;
	}
	inline StackGuard<MatrixStack<Mat>> pushGuard() {
		return StackGuard<MatrixStack>(*this, stack.top());
	}
	inline StackGuard<MatrixStack<Mat>> pushGuard(const Mat& mx) {
		return StackGuard<MatrixStack>(*this, mx);
	}
	inline MatrixStack<Mat>& pop() {
		LIBV_GL_DEBUG_ASSERT(stack.size() > 0);
		stack.pop();
		return *this;
	}
	inline decltype(auto) top() const {
		return stack.top();
	}
	inline decltype(auto) top() {
		return stack.top();
	}
	inline MatrixStack& operator=(const Mat& mat) {
		stack.top() = mat;
		return *this;
	}
	inline decltype(auto) operator*(const Mat& mat) const {
		return stack.top() * mat;
	}
	inline decltype(auto) operator*(const MatrixStack<Mat>& ms) const {
		return stack.top() * ms.stack.top();
	}
	inline operator Mat() const {
		return stack.top();
	}
	inline operator Mat&() {
		return stack.top();
	}
	inline operator const Mat&() const {
		return stack.top();
	}

	// ---------------------------------------------------------------------------------------------

	inline decltype(auto) rotate(const T& a, const vec3_t<T>& v) {
		stack.top().rotate(a, v);
		return *this;
	}

	inline decltype(auto) scale(const T& s) {
		stack.top().scale(vec3_t<T>(s, s, s));
		return *this;
	}
	inline decltype(auto) scale(const T& x, const T& y, const T& z) {
		stack.top().scale(vec3_t<T>(x, y, z));
		return *this;
	}
	inline decltype(auto) scale(const vec3_t<T>& v) {
		stack.top().scale(v);
		return *this;
	}

	inline decltype(auto) translate(const T& x, const T& y, const T& z) {
		stack.top().translate(vec3_t<T>(x, y, z));
		return *this;
	}
	inline decltype(auto) translate(const vec3_t<T>& v) {
		stack.top().translate(v);
		return *this;
	}

	inline decltype(auto) identity() {
		stack.top() = libv::identity<4, T>();
		return *this;
	}

	// ---------------------------------------------------------------------------------------------
};

// -------------------------------------------------------------------------------------------------

template<size_t FACE>
struct PolygonModeSelector {
	inline void point() {
		glPolygonMode(FACE, GL_POINT);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void line() {
		glPolygonMode(FACE, GL_LINE);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void fill() {
		glPolygonMode(FACE, GL_FILL);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void operator()(Mode mode) {
		glPolygonMode(FACE, to_value(mode));
		LIBV_GL_DEBUG_CHECK();
	}
};

struct PolygonModeSetter {
	PolygonModeSelector<GL_BACK> front;
	PolygonModeSelector<GL_FRONT> back;
	PolygonModeSelector<GL_FRONT_AND_BACK> frontAndBack;

	inline void operator()(Face face, Mode mode) {
		glPolygonMode(to_value(face), to_value(mode));
		LIBV_GL_DEBUG_CHECK();
	}
};

// -------------------------------------------------------------------------------------------------

class GL {
private:
	libv::mat4f matrixTexture_; // Make it static constexpr...
public:
	MatrixStack<libv::mat4f> model;
	MatrixStack<libv::mat4f> view;
	MatrixStack<libv::mat4f> projection;

public:
	inline libv::mat4f mvp() const {
		return projection * view * model;
	}
	inline libv::vec3f eye() const {
		return libv::vec::xyz(view.top().inverse_copy()[3]);
	}
	inline const libv::mat4f& getMatrixTexture() const {
		return matrixTexture_;
	}

public:
	inline GLint getMaxUniformBlockSize() const {
		GLint n;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &n);
		LIBV_GL_DEBUG_CHECK();
		return n;
	}
	inline GLint getMaxUniformBufferBindings() const {
		GLint n;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &n);
		LIBV_GL_DEBUG_CHECK();
		return n;
	}
	inline GLint getMaxVertexAttribs() const {
		GLint n;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
		LIBV_GL_DEBUG_CHECK();
		return n;
	}
	inline GLint getMaxVertexUniformComponents() const {
		GLint n;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &n);
		LIBV_GL_DEBUG_CHECK();
		return n;
	}
	inline GLint getMaxCombinedTextureImageUnits() const {
		GLint n;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &n);
		LIBV_GL_DEBUG_CHECK();
		return n;
	}
	inline GLint getCurrentAvailableVideoMemory() const {
//		int availableKB[4];
//		if (GLEW_NVX_gpu_memory_info)
//			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availableKB[0]);
//		int temp = GLEW_ATI_meminfo;
//		if (GLEW_ATI_meminfo)
//			glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, availableKB);
//		return availableKB[0];
		return 0;
	}

public:
	inline void activeTexture(TextureChannel channel);
	inline void drawArrays(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t vertexOffset);
	inline void drawElements(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset);
	inline void drawElementsBaseVertex(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset, size_t vertexOffset);
	inline void viewport(vec2i pos, vec2i size);

	inline void enable(Capability capability);
	inline void disable(Capability capability);
	inline EnableGuard<GL, Capability> enableGuard(Capability capability);
	inline DisableGuard<GL, Capability> disableGuard(Capability capability);

	inline void clearColor(const libv::vec4f& col);
	inline void clearColor(float r, float g, float b, float a = 1.0f);
	inline void clear(Buffer buffers = Buffer::Color | Buffer::Depth);

	inline void frontFaceCCW();
	inline void frontFaceCW();

	PolygonModeSetter polygonMode;

	//	void depthMask(bool writeEnabled);
	//	void stencilMask(bool writeEnabled);
	//	void stencilMask(uint mask);
	//
	//	void stencilFunc(TestFunctiris::test_function_t function, int reference, uint mask = ~0);
	//	void stencilOp(StencilAction::stencil_action_t fail, StencilAction::stencil_action_t zfail, StencilAction::stencil_action_t pass);

	inline void use(const Program& program);

	//	void bindTexture(const Texture& texture, uchar unit);
	//
	//	void bindFramebuffer(const Framebuffer& framebuffer);
	//	void bindFramebuffer();
	//
	//	void beginTransformFeedback(Primitive::primitive_t mode);
	//	void endTransformFeedback();
	GL() :
		matrixTexture_(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f) { }
};

// Debug -------------------------------------------------------------------------------------------

// <editor-fold defaultstate="collapsed" desc="inline void renderCube(float x, float y, float z, float size) {...">
inline void renderCube(float x, float y, float z, float size) {
	glBegin(GL_QUADS);

	//Right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x + size, y - size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x + size, y - size, z + size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y + size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x + size, y + size, z - size);

	//Left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y - size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x - size, y + size, z - size);
	glTexCoord2f(1, 1);
	glVertex3f(x - size, y + size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x - size, y - size, z + size);

	//Top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y + size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x + size, y + size, z - size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y + size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x - size, y + size, z + size);

	//Bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y - size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x - size, y - size, z + size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y - size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x + size, y - size, z - size);

	//Near
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y - size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x + size, y - size, z - size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y + size, z - size);
	glTexCoord2f(1, 0);
	glVertex3f(x - size, y + size, z - size);

	//Far
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y - size, z + size);
	glTexCoord2f(0, 1);
	glVertex3f(x - size, y + size, z + size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y + size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x + size, y - size, z + size);

	glEnd();
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="inline void renderXYZ(float x, float y, float z, float size) {...">
inline void renderXYZ(float x, float y, float z, float size) {
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(x, y, z);
	glVertex3f(x + size, y, z);
	glColor3f(0, 1, 0);
	glVertex3f(x, y, z);
	glVertex3f(x, y + size, z);
	glColor3f(0, 0, 1);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + size);
	glEnd();
	glColor3f(1, 1, 1);
}
// </editor-fold>

// =================================================================================================

inline void GL::activeTexture(TextureChannel channel) {
	glActiveTexture(GL_TEXTURE0 + to_value(channel));
	LIBV_GL_DEBUG_CHECK();
}
inline void GL::drawArrays(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t vertexOffset) {
	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
	glBindVertexArray(vao);
	glDrawArrays(to_value(mode), vertexOffset, vertexCount);
	LIBV_GL_DEBUG_CHECK();
	glBindVertexArray(0);
}
inline void GL::drawElements(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset) {
	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
	glBindVertexArray(vao);
	glDrawElements(to_value(mode), vertexCount, GL_UNSIGNED_INT, reinterpret_cast<void*> (sizeof (GLuint) * indexOffset));
	LIBV_GL_DEBUG_CHECK();
	glBindVertexArray(0);
}
inline void GL::drawElementsBaseVertex(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset, size_t vertexOffset) {
	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
	glBindVertexArray(vao);
	glDrawElementsBaseVertex(to_value(mode), vertexCount, GL_UNSIGNED_INT, reinterpret_cast<void*> (sizeof (GLuint) * indexOffset), vertexOffset);
	LIBV_GL_DEBUG_CHECK();
	glBindVertexArray(0);
}
inline void GL::viewport(vec2i pos, vec2i size) {
	glViewport(pos.x, pos.y, size.x, size.y);
	LIBV_GL_DEBUG_CHECK();
}
inline void GL::enable(Capability capability) {
	glEnable(to_value(capability));
	LIBV_GL_DEBUG_CHECK();
}
inline void GL::disable(Capability capability) {
	glDisable(to_value(capability));
	LIBV_GL_DEBUG_CHECK();
}
inline EnableGuard<GL, Capability> GL::enableGuard(Capability capability) {
	return EnableGuard<GL, Capability>(*this, capability);
}
inline DisableGuard<GL, Capability> GL::disableGuard(Capability capability) {
	return DisableGuard<GL, Capability>(*this, capability);
}
inline void GL::clearColor(const libv::vec4f& col) {
	glClearColor(col.r, col.g, col.b, col.a);
	LIBV_GL_DEBUG_CHECK();
}
inline void GL::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	LIBV_GL_DEBUG_CHECK();
}
inline void GL::clear(Buffer buffers) {
	glClear(to_value(buffers));
	LIBV_GL_DEBUG_CHECK();
}
inline void GL::frontFaceCCW() {
	glFrontFace(GL_CCW);
	LIBV_GL_DEBUG_CHECK();
}
inline void GL::frontFaceCW() {
	glFrontFace(GL_CW);
	LIBV_GL_DEBUG_CHECK();
}
inline void GL::use(const Program& program) {
	program.use();
	LIBV_GL_DEBUG_CHECK();
}

} // namespace gl
} // namespace libv
