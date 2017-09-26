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

};

// -------------------------------------------------------------------------------------------------

#define __LIBV_GL_GEN_SETTER_FUNCTION(ENUM_VALUE, FUNC_NAME)                                       \
	inline void FUNC_NAME() noexcept { set(ENUM_VALUE); }                                          \
	[[nodiscard]] inline auto FUNC_NAME##_guard() noexcept {                                       \
		auto past = current; set(ENUM_VALUE); return Guard([this, past] { set(past); }); }

// -------------------------------------------------------------------------------------------------

struct BlendFunctionSetter {
	BlendFunction current_source = []{ GLint r[1]; glGetIntegerv(GL_BLEND_SRC_RGB, r); return BlendFunction{r[0]}; }();
	BlendFunction current_destination = []{ GLint r[1]; glGetIntegerv(GL_BLEND_DST_RGB, r); return BlendFunction{r[0]}; }();

	//ConstantAlpha
	//ConstantColor
	//DestinationAlpha
	//DestinationColor
	//One
	//One_Minus_ConstantAlpha
	//One_Minus_ConstantColor
	//One_Minus_DestinationAlpha
	//One_Minus_DestinationColor
	//One_Minus_Source1Alpha
	//One_Minus_Source1Color
	//One_Minus_SourceAlpha
	//One_Minus_SourceColor
	//Source1Alpha
	//Source1Color
	//SourceAlpha
	//SourceAlphaSaturate
	//SourceColor
	//Zero

	//glGet with argument GL_BLEND_SRC_RGB
	//glGet with argument GL_BLEND_SRC_ALPHA
	//glGet with argument GL_BLEND_DST_RGB
	//glGet with argument GL_BLEND_DST_ALPHA

	inline void set(BlendFunction source, BlendFunction destination) noexcept {
		if (current_source != source || current_destination != destination) {
			current_source = source;
			current_destination = destination;
			glBlendFunc(to_value(source), to_value(destination)); //Alpha Type
			LIBV_GL_DEBUG_CHECK();
		}
	}
	inline void operator()(BlendFunction source, BlendFunction destination) noexcept {
		set(source, destination);
	}
	[[nodiscard]] inline auto set_guard(BlendFunction source, BlendFunction destination) noexcept {
		auto past_source = current_source;
		auto past_destination = current_destination;
		set(source, destination);
		return Guard([this, past_source, past_destination] { set(past_source, past_destination); });
	}
};

// -------------------------------------------------------------------------------------------------

struct CapabilitySetter {
	template <size_t CAPABILITY>
	struct Selector {
		bool current = glIsEnabled(CAPABILITY);

		inline void set(bool enable_) noexcept {
			if (enable_)
				enable();
			else
				disable();
		}
		inline void enable() noexcept {
			if (!current) {
				current = true;
				glEnable(CAPABILITY);
				LIBV_GL_DEBUG_CHECK();
			}
		}
		inline void disable() noexcept {
			if (current) {
				current = false;
				glDisable(CAPABILITY);
				LIBV_GL_DEBUG_CHECK();
			}
		}
		inline void operator()(bool enable_) noexcept {
			set(enable_);
		}
		[[nodiscard]] inline auto enable_guard() noexcept {
			auto past = current;
			enable();
			return Guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto disable_guard() noexcept {
			auto past = current;
			disable();
			return Guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto set_guard(bool enable_) noexcept {
			auto past = current;
			operator()(enable_);
			return Guard([this, past] { set(past); });
		}
	};

	Selector<to_value(Capability::Blend)> blend;
	Selector<to_value(Capability::CullFace)> cullFace;
	Selector<to_value(Capability::DepthTest)> depthTest;
	Selector<to_value(Capability::RasterizerDiscard)> rasterizerDiscard;
	Selector<to_value(Capability::ScissorTest)> scissorTest;
	Selector<to_value(Capability::StencilTest)> stencilTest;
	Selector<to_value(Capability::TextureCubeMapSeamless)> textureCubeMapSeamless;

	inline void operator()(Capability capability, bool enable) {
		switch (capability) {
			case Capability::Blend: return blend(enable);
			case Capability::CullFace: return cullFace(enable);
			case Capability::DepthTest: return depthTest(enable);
			case Capability::RasterizerDiscard: return rasterizerDiscard(enable);
			case Capability::ScissorTest: return scissorTest(enable);
			case Capability::StencilTest: return stencilTest(enable);
			case Capability::TextureCubeMapSeamless: return textureCubeMapSeamless(enable);
		}
	}
};

// -------------------------------------------------------------------------------------------------

struct CullFaceSetter {
	CullFace current = []{ GLboolean r[1]; glGetBooleanv(GL_CULL_FACE, r); return CullFace{r[0]}; }();

	inline void set(CullFace face) noexcept {
		if (current != face) {
			current = face;
			glCullFace(to_value(face));
			LIBV_GL_DEBUG_CHECK();
		}
	}
	inline void operator()(CullFace face) noexcept {
		set(face);
	}
	[[nodiscard]] inline auto set_guard(CullFace face) noexcept {
		auto past = current;
		set(face);
		return Guard([this, past] { set(past); });
	}

	__LIBV_GL_GEN_SETTER_FUNCTION(CullFace::Front, front)
	__LIBV_GL_GEN_SETTER_FUNCTION(CullFace::Back, back)
};

// -------------------------------------------------------------------------------------------------

struct DepthFunctionSetter {
	TestFunction current = []{ GLint r[1]; glGetIntegerv(GL_DEPTH_FUNC, r); return TestFunction{r[0]}; }();

	inline void set(TestFunction value) noexcept {
		if (current != value) {
			current = value;
			glDepthFunc(to_value(value));
			LIBV_GL_DEBUG_CHECK();
		}
	}
	inline void operator()(TestFunction value) noexcept {
		set(value);
	}
	[[nodiscard]] inline auto set_guard(TestFunction value) noexcept {
		auto past = current;
		set(value);
		return Guard([this, past] { set(past); });
	}

	__LIBV_GL_GEN_SETTER_FUNCTION(TestFunction::Always, always)
	__LIBV_GL_GEN_SETTER_FUNCTION(TestFunction::Equal, equal)
	__LIBV_GL_GEN_SETTER_FUNCTION(TestFunction::GEqual, gequal)
	__LIBV_GL_GEN_SETTER_FUNCTION(TestFunction::Greater, greater)
	__LIBV_GL_GEN_SETTER_FUNCTION(TestFunction::LEqual, lequal)
	__LIBV_GL_GEN_SETTER_FUNCTION(TestFunction::Less, less)
	__LIBV_GL_GEN_SETTER_FUNCTION(TestFunction::Never, never)
	__LIBV_GL_GEN_SETTER_FUNCTION(TestFunction::NotEqual, notEqual)
};

// -------------------------------------------------------------------------------------------------

struct FrontFaceSetter {
	FrontFace current = []{ GLint r[1]; glGetIntegerv(GL_FRONT_FACE, r); return FrontFace{r[0]}; }();

	inline void set(FrontFace face) noexcept {
		if (current != face) {
			current = face;
			glFrontFace(to_value(face));
			LIBV_GL_DEBUG_CHECK();
		}
	}
	inline void operator()(FrontFace face) noexcept {
		set(face);
	}
	[[nodiscard]] inline auto set_guard(FrontFace face) noexcept {
		auto past = current;
		set(face);
		return Guard([this, past] { set(past); });
	}

	__LIBV_GL_GEN_SETTER_FUNCTION(FrontFace::CCW, ccw)
	__LIBV_GL_GEN_SETTER_FUNCTION(FrontFace::CW, cw)

};

// -------------------------------------------------------------------------------------------------

struct PolygonModeSetter {
	Mode current = []{ GLint r[2]; glGetIntegerv(GL_POLYGON_MODE, r); return Mode{r[0]}; }();

	inline void set(Mode mode) noexcept {
		if (current != mode) {
			current = mode;
			glPolygonMode(GL_FRONT_AND_BACK, to_value(mode));
			LIBV_GL_DEBUG_CHECK();
		}
	}
	inline void operator()(Mode mode) noexcept {
		set(mode);
	}
	[[nodiscard]] inline auto set_guard(Mode mode) noexcept {
		auto past = current;
		operator()(mode);
		return Guard([this, past] { set(past); });
	}

	__LIBV_GL_GEN_SETTER_FUNCTION(Mode::Point, point)
	__LIBV_GL_GEN_SETTER_FUNCTION(Mode::Line, line)
	__LIBV_GL_GEN_SETTER_FUNCTION(Mode::Fill, fill)
};

// -------------------------------------------------------------------------------------------------

#undef __LIBV_GL_GEN_SETTER_FUNCTION

// -------------------------------------------------------------------------------------------------

class GL {
private:
	libv::mat4f matrixTexture_; // Make it static constexpr...

	TextureChannel currentActiveTexture;

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
	inline GLint getMaxTextureSize() const {
		GLint n;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &n);
		LIBV_GL_DEBUG_CHECK();
		return n;
	}

public:
	inline void activeTexture(TextureChannel channel);
	inline void drawArrays(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t vertexOffset);
	inline void drawElements(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset);
	inline void drawElementsBaseVertex(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset, size_t vertexOffset);
	inline void viewport(vec2i pos, vec2i size);

	inline void clearColor(const libv::vec4f& col);
	inline void clearColor(float r, float g, float b, float a = 1.0f);
	inline void clear(Buffer buffers = Buffer::Color | Buffer::Depth);

	BlendFunctionSetter blendFunction;
	CapabilitySetter capability;
	CullFaceSetter cullFace;
	DepthFunctionSetter depthFunction;
	FrontFaceSetter frontFace;
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
	if (currentActiveTexture != channel) {
		currentActiveTexture = channel;
		glActiveTexture(GL_TEXTURE0 + to_value(channel));
		LIBV_GL_DEBUG_CHECK();
	}
}
inline void GL::drawArrays(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t vertexOffset) {
	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
	glBindVertexArray(vao);
	glDrawArrays(to_value(mode), static_cast<GLint>(vertexOffset), static_cast<GLsizei>(vertexCount));
	LIBV_GL_DEBUG_CHECK();
	glBindVertexArray(0);
}
inline void GL::drawElements(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset) {
	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
	glBindVertexArray(vao);
	glDrawElements(to_value(mode), static_cast<GLsizei>(vertexCount), GL_UNSIGNED_INT, reinterpret_cast<void*> (sizeof (GLuint) * indexOffset));
	LIBV_GL_DEBUG_CHECK();
	glBindVertexArray(0);
}
inline void GL::drawElementsBaseVertex(const VertexArray& vao, Primitive mode, size_t vertexCount, size_t indexOffset, size_t vertexOffset) {
	LIBV_GL_DEBUG_ASSERT(vao.id() != 0);
	glBindVertexArray(vao);
	glDrawElementsBaseVertex(to_value(mode), static_cast<GLsizei>(vertexCount), GL_UNSIGNED_INT, reinterpret_cast<void*> (sizeof (GLuint) * indexOffset), static_cast<GLint>(vertexOffset));
	LIBV_GL_DEBUG_CHECK();
	glBindVertexArray(0);
}
inline void GL::viewport(vec2i pos, vec2i size) {
	glViewport(pos.x, pos.y, size.x, size.y);
	LIBV_GL_DEBUG_CHECK();
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
inline void GL::use(const Program& program) {
	program.use();
	LIBV_GL_DEBUG_CHECK();
}

} // namespace gl
} // namespace libv
