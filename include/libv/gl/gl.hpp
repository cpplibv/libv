// File: vgl.hpp, Created on 2014. november 25. 12:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/algorithm/fill.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/always.hpp>
#include <libv/utility/guard.hpp>
// std
#include <array>
#include <stack>
#include <vector>
// pro
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/matrix_stack.hpp>
#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/buffer_object.hpp>
#include <libv/gl/program_object.hpp>
#include <libv/gl/shader_object.hpp>
#include <libv/gl/texture_object.hpp>
#include <libv/gl/uniform_buffer_object.hpp>
#include <libv/gl/vertex_array_object.hpp>


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

struct AccessArrayBuffer;
struct AccessBuffer;
struct AccessProgram;
struct AccessShader;
template <typename T> struct AccessTexture;
struct AccessUniformBuffer;
struct AccessVertexArray;

// -------------------------------------------------------------------------------------------------

struct BlendFunctionSetter {
	BlendFunction current_source{0};
	BlendFunction current_destination{0};

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

	inline void set(BlendFunction source, BlendFunction destination) noexcept {
		if (current_source != source || current_destination != destination) {
			current_source = source;
			current_destination = destination;
			glBlendFunc(to_value(source), to_value(destination)); //Alpha Type
			checkGL();
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
		// TODO P5: This is the only setter that uses gl call on init, would be nice to get rid of it
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
				checkGL();
			}
		}
		inline void disable() noexcept {
			if (current) {
				current = false;
				glDisable(CAPABILITY);
				checkGL();
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
			set(enable_);
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

struct ClipPlanesSetter {
	uint32_t current{0};

	inline void set(uint32_t value) noexcept {
		if (current != value) {
			for (uint32_t i = current; i < value; ++i)
				glEnable(GL_CLIP_DISTANCE0 + i);

			for (uint32_t i = value; i < current; ++i)
				glDisable(GL_CLIP_DISTANCE0 + i);

			current = value;
			checkGL();
		}
	}
	inline void operator()(uint32_t value) noexcept {
		set(value);
	}
	[[nodiscard]] inline auto set_guard(uint32_t value) noexcept {
		auto past = current;
		set(value);
		return Guard([this, past] { set(past); });
	}
};

// -------------------------------------------------------------------------------------------------

struct CullFaceSetter {
	CullFace current{0};

	inline void set(CullFace face) noexcept {
		if (current != face) {
			current = face;
			glCullFace(to_value(face));
			checkGL();
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

	inline void front() noexcept {
		set(CullFace::Front);
	}
	[[nodiscard]] inline auto front_guard() noexcept {
		return set_guard(CullFace::Front);
	}
	inline void back() noexcept {
		set(CullFace::Back);
	}
	[[nodiscard]] inline auto back_guard() noexcept {
		return set_guard(CullFace::Back);
	}
};

// -------------------------------------------------------------------------------------------------

struct DepthFunctionSetter {
	TestFunction current{0};

	inline void set(TestFunction value) noexcept {
		if (current != value) {
			current = value;
			glDepthFunc(to_value(value));
			checkGL();
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

	inline void always() noexcept {
		set(TestFunction::Always);
	}
	[[nodiscard]] inline auto always_guard() noexcept {
		return set_guard(TestFunction::Always);
	}
	inline void equal() noexcept {
		set(TestFunction::Equal);
	}
	[[nodiscard]] inline auto equal_guard() noexcept {
		return set_guard(TestFunction::Equal);
	}
	inline void gequal() noexcept {
		set(TestFunction::GEqual);
	}
	[[nodiscard]] inline auto gequal_guard() noexcept {
		return set_guard(TestFunction::GEqual);
	}
	inline void greater() noexcept {
		set(TestFunction::Greater);
	}
	[[nodiscard]] inline auto greater_guard() noexcept {
		return set_guard(TestFunction::Greater);
	}
	inline void lequal() noexcept {
		set(TestFunction::LEqual);
	}
	[[nodiscard]] inline auto lequal_guard() noexcept {
		return set_guard(TestFunction::LEqual);
	}
	inline void less() noexcept {
		set(TestFunction::Less);
	}
	[[nodiscard]] inline auto less_guard() noexcept {
		return set_guard(TestFunction::Less);
	}
	inline void never() noexcept {
		set(TestFunction::Never);
	}
	[[nodiscard]] inline auto never_guard() noexcept {
		return set_guard(TestFunction::Never);
	}
	inline void notEqual() noexcept {
		set(TestFunction::NotEqual);
	}
	[[nodiscard]] inline auto notEqual_guard() noexcept {
		return set_guard(TestFunction::NotEqual);
	}
};

// -------------------------------------------------------------------------------------------------

struct FrontFaceSetter {
	FrontFace current{0};

	inline void set(FrontFace face) noexcept {
		if (current != face) {
			current = face;
			glFrontFace(to_value(face));
			checkGL();
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

	inline void ccw() noexcept {
		set(FrontFace::CCW);
	}
	[[nodiscard]] inline auto ccw_guard() noexcept {
		return set_guard(FrontFace::CCW);
	}
	inline void cw() noexcept {
		set(FrontFace::CW);
	}
	[[nodiscard]] inline auto cw_guard() noexcept {
		return set_guard(FrontFace::CW);
	}
};

// -------------------------------------------------------------------------------------------------

struct PolygonModeSetter {
	PolygonMode current{0};

	inline void set(PolygonMode mode) noexcept {
		if (current != mode) {
			current = mode;
			glPolygonMode(GL_FRONT_AND_BACK, to_value(mode));
			checkGL();
		}
	}
	inline void operator()(PolygonMode mode) noexcept {
		set(mode);
	}
	[[nodiscard]] inline auto set_guard(PolygonMode mode) noexcept {
		auto past = current;
		set(mode);
		return Guard([this, past] { set(past); });
	}

	inline void point() noexcept {
		set(PolygonMode::Point);
	}
	[[nodiscard]] inline auto point_guard() noexcept {
		return set_guard(PolygonMode::Point);
	}
	inline void line() noexcept {
		set(PolygonMode::Line);
	}
	[[nodiscard]] inline auto line_guard() noexcept {
		return set_guard(PolygonMode::Line);
	}
	inline void fill() noexcept {
		set(PolygonMode::Fill);
	}
	[[nodiscard]] inline auto fill_guard() noexcept {
		return set_guard(PolygonMode::Fill);
	}
};

// -------------------------------------------------------------------------------------------------

struct MaskSetter {
	struct Depth {
		// TODO P5: Would be nice to get rid of this assumption of initial value being 'true'
		bool current = true;

		inline void set(bool enable_) noexcept {
			if (current != enable_) {
				current = enable_;
				glDepthMask(enable_);
				checkGL();
			}
		}
		inline void enable() noexcept {
			if (!current) {
				current = true;
				glDepthMask(true);
				checkGL();
			}
		}
		inline void disable() noexcept {
			if (current) {
				current = false;
				glDepthMask(false);
				checkGL();
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
			set(enable_);
			return Guard([this, past] { set(past); });
		}
	};
	struct Color {
		// ?
	};
	struct Stencil {
		// ?
		//GLuint mask;
	};

	Depth depth;
	Color color;
	Stencil stencil;
};

// -------------------------------------------------------------------------------------------------

struct GL {
public:
	MatrixStack<libv::mat4f> model;
	MatrixStack<libv::mat4f> view;
	MatrixStack<libv::mat4f> projection;

private:
	bool debug = false;
	bool trace = false;

public:
	BlendFunctionSetter blendFunction;
	CapabilitySetter capability;
	ClipPlanesSetter clipPlanes;
	CullFaceSetter cullFace;
	DepthFunctionSetter depthFunction;
	FrontFaceSetter frontFace;
	PolygonModeSetter polygonMode;
	MaskSetter mask;

private:
	TextureChannel currentActiveTexture{0};
	std::array<std::array<uint32_t, 16>, 11> textureBindings; /// Target -> Channel -> ID

public:
	GL() {
		for (auto& channels : textureBindings)
			libv::fill(channels, 0);
	}

public:
	inline libv::mat4f mvp() const {
		return projection * view * model;
	}
	inline libv::vec3f eye() const {
		return libv::vec::xyz(view.top().inverse_copy()[3]);
	}
	inline libv::mat4f getMatrixTexture() const {
		return libv::mat4f{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.5f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f};
	}

private:
	template <typename T>
	inline T get(GLenum param) const {
		T value;

		if constexpr (std::is_same_v<GLboolean, T>)
			glGetBooleanv(param, &value);

		else if constexpr (std::is_same_v<GLint, T>)
			glGetIntegerv(param, &value);

		else if constexpr (std::is_same_v<GLint64, T>)
			glGetInteger64v(param, &value);

		else if constexpr (std::is_same_v<GLfloat, T>)
			glGetFloatv(param, &value);

		else if constexpr (std::is_same_v<GLdouble, T>)
			glGetDoublev(param, &value);

		else
			static_assert(libv::meta::always_false_v<T>, "Type T is not supported");

		return value;
	}

public:
	inline GLint getMaxUniformBlockSize() const {
		return get<GLint>(GL_MAX_UNIFORM_BLOCK_SIZE);
	}
	inline GLint getMaxUniformBufferBindings() const {
		return get<GLint>(GL_MAX_UNIFORM_BUFFER_BINDINGS);
	}
	inline GLint getMaxVertexAttribs() const {
		return get<GLint>(GL_MAX_VERTEX_ATTRIBS);
	}
	inline GLint getMaxVertexUniformComponents() const {
		return get<GLint>(GL_MAX_VERTEX_UNIFORM_COMPONENTS);
	}
	inline GLint getMaxTextureImageUnits() const {
		return get<GLint>(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	}
	inline GLint getMaxTextureImageUnitsVertex() const {
		return get<GLint>(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
	}
	inline GLint getMaxTextureImageUnitsGeometry() const {
		return get<GLint>(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS);
	}
	inline GLint getMaxTextureImageUnitsCompute() const {
		return get<GLint>(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS);
	}
	inline GLint getMaxTextureImageUnitsTessControl() const {
		return get<GLint>(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS);
	}
	inline GLint getMaxTextureImageUnitsTessEvaluation() const {
		return get<GLint>(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS);
	}
	inline GLint getMaxTextureImageUnitsFragment() const {
		return get<GLint>(GL_MAX_TEXTURE_IMAGE_UNITS);
	}
	inline GLint getCurrentAvailableVideoMemory() const {
		GLint availableKB[4];

		if (GLEW_NVX_gpu_memory_info)
			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, availableKB);
			//GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX
			//GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX
			//GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX
			//GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX
			//GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX

		if (GLEW_ATI_meminfo)
			glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, availableKB);

		return availableKB[0];
	}
	inline GLint getMaxTextureSize() const {
		return get<GLint>(GL_MAX_TEXTURE_SIZE);
	}
	inline GLint getUniformBufferOffsetAlignment() const {
		return get<GLint>(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT);
	}

private:
	static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
			const GLchar* message, const void* userData) {

		(void) length;

		//const auto type_str = libv::switch_(type)
		//	(GL_DEBUG_TYPE_ERROR, "Error")
		//	(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behavior")
		//	(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined Behavior")
		//	(GL_DEBUG_TYPE_PORTABILITY, "Portability")
		//	(GL_DEBUG_TYPE_PERFORMANCE, "Performance")
		//	(GL_DEBUG_TYPE_OTHER, "Other")
		//	(GL_DEBUG_TYPE_MARKER, "Marker")
		//	(GL_DEBUG_TYPE_PUSH_GROUP, "Push Group")
		//	(GL_DEBUG_TYPE_POP_GROUP, "Pop Group")
		//;

//		const auto source_str = [&] {
//			switch (source) {
//			case GL_DEBUG_SOURCE_API:             return "API"; break;
//			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System"; break;
//			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler"; break;
//			case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party"; break;
//			case GL_DEBUG_SOURCE_APPLICATION:     return "Application"; break;
//			case GL_DEBUG_SOURCE_OTHER:           return "Other"; break;
//			default:                              return ""; break;
//			}
//		}();
//
//		const auto type_str = [&] {
//			switch (type) {
//			case GL_DEBUG_TYPE_ERROR:               return "Error"; break;
//			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour"; break;
//			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour"; break;
//			case GL_DEBUG_TYPE_PORTABILITY:         return "Portability"; break;
//			case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance"; break;
//			case GL_DEBUG_TYPE_MARKER:              return "Marker"; break;
//			case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group"; break;
//			case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group"; break;
//			case GL_DEBUG_TYPE_OTHER:               return "Other"; break;
//			default:                                return ""; break;
//			}
//		}();
//
//		const auto severity_str = [&] {
//			switch (severity) {
//			case GL_DEBUG_SEVERITY_HIGH:         return "high"; break;
//			case GL_DEBUG_SEVERITY_MEDIUM:       return "medium"; break;
//			case GL_DEBUG_SEVERITY_LOW:          return "low"; break;
//			case GL_DEBUG_SEVERITY_NOTIFICATION: return "notification"; break;
//			default:                             return ""; break;
//			}
//		}();

		auto& self = *static_cast<const GL*>(userData);
		if (type == GL_DEBUG_TYPE_ERROR && self.debug)
			log_gl.error("OpenGL: src {}, type {:X}, id {}, severity {:X}, message: {}",
					source, type, id, severity, message);
		else if (self.trace)
			log_gl.trace("OpenGL: src {}, type {:X}, id {}, severity {:X}, message: {}",
					source, type, id, severity, message);
	};

public:
	void enableTrace() {
		// TODO P4: test and integrate with OpenGL context debug bit more
		if (!std::exchange(trace, true) && !debug) {
			debug = true;
			// glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Sync would allow collection of stacktrace information
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(debugCallback, this);
		}
	}

	void enableDebug() {
		if (!std::exchange(debug, true) && !trace) {
			// glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Sync would allow collection of stacktrace information
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(debugCallback, this);
		}
	}

public:
	inline void activeTexture(TextureChannel channel);
	inline void viewport(vec2i pos, vec2i size);

	inline void clearColor(const libv::vec4f& col);
	inline void clearColor(float r, float g, float b, float a = 1.0f);
	inline void clear(BufferBit buffers = BufferBit::Color | BufferBit::Depth);

public:
	//	void depthMask(bool writeEnabled);
	//	void stencilMask(bool writeEnabled);
	//	void stencilMask(uint mask);
	//
	//	void stencilFunc(TestFunctiris::test_function_t function, int reference, uint mask = ~0);
	//	void stencilOp(StencilAction::stencil_action_t fail, StencilAction::stencil_action_t zfail, StencilAction::stencil_action_t pass);
//	inline void use(const Program& program);

private:
	inline void bind(TextureTarget target, uint32_t id);

public:
	template <TextureTarget Target>
	inline void bind(const Texture_t<Target>& texture);
	inline void bind(const Texture& texture);
	template <TextureTarget Target>
	inline void unbind(const Texture_t<Target>& texture);
	inline void unbind(const Texture& texture);

	//	void bindTexture(const Texture& texture, uchar unit);
	//
	//	void bindFramebuffer(const Framebuffer& framebuffer);
	//	void bindFramebuffer();

public:
	template <typename Access = AccessUniformBuffer>
	inline Access operator()(UniformBuffer& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessArrayBuffer>
	inline Access operator()(ArrayBuffer& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessBuffer>
	inline Access operator()(Buffer& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessProgram>
	inline Access operator()(Program& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessShader>
	inline Access operator()(Shader& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessVertexArray>
	inline Access operator()(VertexArray& object) noexcept {
		return Access{object};
	}
	template <TextureTarget Target, typename Access = AccessTexture<Texture_t<Target>>>
	inline Access operator()(Texture_t<Target>& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessTexture<Texture>>
	inline Access operator()(Texture& object) noexcept {
		return Access{object, *this};
	}
};

// =================================================================================================

inline void GL::activeTexture(TextureChannel channel) {
	if (currentActiveTexture != channel) {
		currentActiveTexture = channel;
		glActiveTexture(GL_TEXTURE0 + to_value(channel));
		checkGL();
	}
}
inline void GL::viewport(vec2i pos, vec2i size) {
	glViewport(pos.x, pos.y, size.x, size.y);
	checkGL();
}
inline void GL::clearColor(const libv::vec4f& col) {
	glClearColor(col.r, col.g, col.b, col.a);
	checkGL();
}
inline void GL::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	checkGL();
}
inline void GL::clear(BufferBit buffers) {
	glClear(to_value(buffers));
	checkGL();
}

inline void GL::bind(TextureTarget target, uint32_t id) {
	size_t targetIndex;

	switch (target) {
	default: // TODO P5: libv.gl: instead of default log and assert invalid input
	case TextureTarget::_2D: targetIndex = 0; break;
	case TextureTarget::CubeMap: targetIndex = 1; break;
	case TextureTarget::_1D: targetIndex = 2; break;
	case TextureTarget::_3D: targetIndex = 3; break;
	case TextureTarget::_1DArray: targetIndex = 4; break;
	case TextureTarget::_2DArray: targetIndex = 5; break;
	case TextureTarget::Rectangle: targetIndex = 6; break;
	case TextureTarget::CubeMapArray: targetIndex = 7; break;
	case TextureTarget::Buffer: targetIndex = 8; break;
	case TextureTarget::_2DMultisample: targetIndex = 9; break;
	case TextureTarget::_2DMultisampleArray: targetIndex = 10; break;
	}

	auto& binding = textureBindings[targetIndex][to_value(currentActiveTexture)];
	if (binding != id) {
		glBindTexture(to_value(target), id);
		binding = id;
	}
}
template <TextureTarget Target>
inline void GL::bind(const Texture_t<Target>& texture) {
	bind(texture.target, texture.id);
}
inline void GL::bind(const Texture& texture) {
	bind(texture.target, texture.id);
}
template <TextureTarget Target>
inline void GL::unbind(const Texture_t<Target>& texture) {
	bind(texture.target, 0);
}
inline void GL::unbind(const Texture& texture) {
	bind(texture.target, 0);
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
