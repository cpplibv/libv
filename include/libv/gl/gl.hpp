// File: vgl.hpp, Created on 2014. november 25. 12:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
// std
#include <stack>
#include <vector>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/matrix_stack.hpp>
#include <libv/gl/vertex_buffer.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

const float PI = 3.14159265358979323846f;
inline float rad(float degrees) {
	return degrees / 180.0f * PI;
}
inline float deg(float radians) {
	return radians / PI * 180.0f;
}

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

struct GL {
public:
	MatrixStack<libv::mat4f> model;
	MatrixStack<libv::mat4f> view;
	MatrixStack<libv::mat4f> projection;

public:
	inline libv::mat4f mvp() const {
		return projection * view * model;
	}
	inline libv::vec3f eye() const {
		return xyz(from_glm(view.top().inverse_copy()[3]));
	}
	inline libv::mat4f getMatrixTexture() const {
		return libv::mat4f{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.5f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f};
	}

private:
	template <typename T, typename P>
	inline T getInteger(P param) const {
		T value;
		glGetIntegerv(param, &value);
		return value;
	}
public:
	inline GLint getMaxUniformBlockSize() const {
		return getInteger<GLint>(GL_MAX_UNIFORM_BLOCK_SIZE);
	}
	inline GLint getMaxUniformBufferBindings() const {
		return getInteger<GLint>(GL_MAX_UNIFORM_BUFFER_BINDINGS);
	}
	inline GLint getMaxVertexAttribs() const {
		return getInteger<GLint>(GL_MAX_VERTEX_ATTRIBS);
	}
	inline GLint getMaxVertexUniformComponents() const {
		return getInteger<GLint>(GL_MAX_VERTEX_UNIFORM_COMPONENTS);
	}
	inline GLint getMaxCombinedTextureImageUnits() const {
		return getInteger<GLint>(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
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
		return getInteger<GLint>(GL_MAX_TEXTURE_SIZE);
	}

public:
	inline void activeTexture(TextureChannel channel);
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

protected:
	TextureChannel currentActiveTexture;

public:
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
};

// =================================================================================================

inline void GL::activeTexture(TextureChannel channel) {
	if (currentActiveTexture != channel) {
		currentActiveTexture = channel;
		glActiveTexture(GL_TEXTURE0 + to_value(channel));
		LIBV_GL_DEBUG_CHECK();
	}
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

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
