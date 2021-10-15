// Project: libv.gl, File: src/libv/gl/gl.hpp

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/algo/fill.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/always.hpp>
#include <libv/utility/guard.hpp>
// std
#include <array>
#include <stack>
#include <vector>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/matrix_stack.hpp>
#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/buffer_object.hpp>
#include <libv/gl/framebuffer_object.hpp>
#include <libv/gl/program_object.hpp>
#include <libv/gl/query_object.hpp>
#include <libv/gl/renderbuffer_object.hpp>
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
struct AccessFramebuffer;
struct AccessProgram;
struct AccessRenderbuffer;
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
		return libv::guard([this, past_source, past_destination] { set(past_source, past_destination); });
	}
};

// -------------------------------------------------------------------------------------------------

struct CapabilitySetter {
	template <size_t CAPABILITY>
	struct Selector {
		bool current = false;

		inline void init() {
			current = glIsEnabled(CAPABILITY);
		}
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
			return libv::guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto disable_guard() noexcept {
			auto past = current;
			disable();
			return libv::guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto set_guard(bool enable_) noexcept {
			auto past = current;
			set(enable_);
			return libv::guard([this, past] { set(past); });
		}
	};

	Selector<to_value(Capability::Blend)> blend;
	Selector<to_value(Capability::CullFace)> cullFace;
	Selector<to_value(Capability::DepthTest)> depthTest;
	Selector<to_value(Capability::Multisample)> multisample;
	Selector<to_value(Capability::RasterizerDiscard)> rasterizerDiscard;
	Selector<to_value(Capability::ScissorTest)> scissorTest;
	Selector<to_value(Capability::StencilTest)> stencilTest;
	Selector<to_value(Capability::TextureCubeMapSeamless)> textureCubeMapSeamless;

	inline void operator()(Capability capability, bool enable) {
		switch (capability) {
			case Capability::Blend: return blend(enable);
			case Capability::CullFace: return cullFace(enable);
			case Capability::DepthTest: return depthTest(enable);
			case Capability::Multisample: return multisample(enable);
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
		return libv::guard([this, past] { set(past); });
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
		return libv::guard([this, past] { set(past); });
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
		return libv::guard([this, past] { set(past); });
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
		return libv::guard([this, past] { set(past); });
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
		return libv::guard([this, past] { set(past); });
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
			return libv::guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto disable_guard() noexcept {
			auto past = current;
			disable();
			return libv::guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto set_guard(bool enable_) noexcept {
			auto past = current;
			set(enable_);
			return libv::guard([this, past] { set(past); });
		}
	};
	struct Color {
		// R G B A
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

	Program current_program_{0};

	Framebuffer framebuffer_read_{0};
	Framebuffer framebuffer_draw_{0};

private:
	void init() {
		glewExperimental = true;

		if (GLenum err = glewInit() != GLEW_OK) {
			log_gl.error("Failed to initialize glew: {}", glewGetErrorString(err));
			return;
		}

		log_gl.debug("GL Vendor     {}", glGetString(GL_VENDOR));
		log_gl.debug("GL Renderer   {}", glGetString(GL_RENDERER));
		log_gl.debug("GL Version    {}", glGetString(GL_VERSION));

		// Disable row alignment for texture and pixel operations
		// Otherwise a default 4 byte alignment is required for pixels row starts (which is broken on R8G8B8 formats if the width is not multiple of 4)
		// For more information:
		//      https://www.khronos.org/opengl/wiki/Common_Mistakes#Texture_upload_and_pixel_reads
		//      https://www.khronos.org/opengl/wiki/Pixel_Transfer#Pixel_layout
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		checkGL();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		checkGL();

		// Fetch OpenGL context current state
		capability.blend.init();
		capability.cullFace.init();
		capability.depthTest.init();
		capability.multisample.init();
		capability.rasterizerDiscard.init();
		capability.scissorTest.init();
		capability.stencilTest.init();
		capability.textureCubeMapSeamless.init();
	}

public:
	GL() {
		for (auto& channels : textureBindings)
			libv::fill(channels, 0);

		init(); // This init function calls into the OpenGL context
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

private:
	constexpr inline size_t convertToTargetIndex(TextureTarget target) const noexcept;

public:
	inline GLint getMaxColorAttachments() const {
		return get<GLint>(GL_MAX_COLOR_ATTACHMENTS);
	}
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
	inline GLint getMaxSamples() const {
		return get<GLint>(GL_MAX_SAMPLES);
	}
	inline GLint getMaxSamplesInteger() const {
		return get<GLint>(GL_MAX_INTEGER_SAMPLES);
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
		auto& self = *static_cast<const GL*>(userData);

		const auto source_str = [&] {
			switch (source) {
			case GL_DEBUG_SOURCE_API:             return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
			case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
			case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
			case GL_DEBUG_SOURCE_OTHER:           return "Other";
			default:                              return "?";
			}
		}();

		const auto type_str = [&] {
			switch (type) {
			case GL_DEBUG_TYPE_ERROR:               return "Error";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour";
			case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
			case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
			case GL_DEBUG_TYPE_MARKER:              return "Marker";
			case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
			case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
			case GL_DEBUG_TYPE_OTHER:               return "Other";
			default:                                return "?";
			}
		}();

		const auto severity_str = [&] {
			switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:         return "high";
			case GL_DEBUG_SEVERITY_MEDIUM:       return "medium";
			case GL_DEBUG_SEVERITY_LOW:          return "low";
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "notification";
			default:                             return "?";
			}
		}();

		if (type == GL_DEBUG_TYPE_ERROR && self.debug)
			log_gl.error("OpenGL: src {} ({}), type {} ({:X}), id {}, severity {} ({:X}), message: {}",
					source_str, source, type_str, type, id, severity_str, severity, message);
		else if (self.trace)
			log_gl.trace("OpenGL: src {} ({}), type {} ({:X}), id {}, severity {} ({:X}), message: {}",
					source_str, source, type_str, type, id, severity_str, severity, message);
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
	inline void viewport(int32_t left, int32_t bottom, int32_t width, int32_t height);
	inline void blit(vec2i src_pos, vec2i src_size, vec2i dst_pos, vec2i dst_size, BufferBit mask, MagFilter filter);

	// void line_width(float width); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLineWidth.xhtml
	// void point_size(float size); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPointSize.xhtml

	inline void clearColor(const libv::vec4f& col);
	inline void clearColor(float r, float g, float b, float a = 1.0f);
	inline void clear(BufferBit buffers = BufferBit::ColorDepthStencil);

	inline void readPixels(vec2i pos, vec2i size, ReadFormat format, DataType type, void* data) {
		// Not supported read data types:
		//		GL_FLOAT_32_UNSIGNED_INT_24_8_REV.
		//		GL_UNSIGNED_INT_10F_11F_11F_REV,
		//		GL_UNSIGNED_INT_24_8
		//		GL_UNSIGNED_INT_5_9_9_9_REV
		glReadPixels(pos.x, pos.y, size.x, size.y, libv::to_value(format), libv::to_value(type), data);
	}

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

	inline Texture bound_texture(TextureTarget target) noexcept;

public:
	inline void use_program(const Program& program) noexcept;
	inline Program bound_program() noexcept;

public:
	inline void framebuffer(Framebuffer object) noexcept;
	inline void framebuffer_draw(Framebuffer object) noexcept;
	inline void framebuffer_read(Framebuffer object) noexcept;
	inline void framebuffer_default() noexcept;
	inline void framebuffer_default_draw() noexcept;
	inline void framebuffer_default_read() noexcept;

	inline Framebuffer framebuffer_draw() const noexcept;
	inline Framebuffer framebuffer_read() const noexcept;

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
	template <typename Access = AccessFramebuffer>
	inline Access operator()(Framebuffer& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessRenderbuffer>
	inline Access operator()(Renderbuffer& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessProgram>
	inline Access operator()(Program& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessQuery>
	inline Access operator()(Query& object) noexcept {
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

constexpr inline size_t GL::convertToTargetIndex(TextureTarget target) const noexcept {
	switch (target) {
	case TextureTarget::_2D: return 0;
	case TextureTarget::CubeMap: return 1;
	case TextureTarget::_1D: return 2;
	case TextureTarget::_3D: return 3;
	case TextureTarget::_1DArray: return 4;
	case TextureTarget::_2DArray: return 5;
	case TextureTarget::Rectangle: return 6;
	case TextureTarget::CubeMapArray: return 7;
	case TextureTarget::Buffer: return 8;
	case TextureTarget::_2DMultisample: return 9;
	case TextureTarget::_2DMultisampleArray: return 10;
	}
	// TODO P5: libv.gl: instead of default log and assert invalid input
	return 0;
}

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
inline void GL::viewport(int32_t left, int32_t bottom, int32_t width, int32_t height) {
	glViewport(left, bottom, width, height);
	checkGL();
}
inline void GL::blit(vec2i src_pos, vec2i src_size, vec2i dst_pos, vec2i dst_size, BufferBit mask, MagFilter filter) {
	LIBV_GL_DEBUG_ASSERT(filter != MagFilter::Linear || mask == BufferBit::Color);

	glBlitFramebuffer(
			src_pos.x,
			src_pos.y,
			src_pos.x + src_size.x,
			src_pos.y + src_size.y,
			dst_pos.x,
			dst_pos.y,
			dst_pos.x + dst_size.x,
			dst_pos.y + dst_size.y,
			libv::to_value(mask),
			libv::to_value(filter)
	);
	checkGL();
}
inline void GL::clearColor(const libv::vec4f& col) {
	glClearColor(col.x, col.y, col.z, col.w);
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

// -------------------------------------------------------------------------------------------------

inline void GL::bind(TextureTarget target, uint32_t id) {
	size_t targetID = convertToTargetIndex(target);

	auto& binding = textureBindings[targetID][to_value(currentActiveTexture)];
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

inline Texture GL::bound_texture(TextureTarget target) noexcept {
	size_t targetID = convertToTargetIndex(target);

	return Texture{textureBindings[targetID][libv::to_value(currentActiveTexture)], target};
}

// -------------------------------------------------------------------------------------------------

inline void GL::use_program(const Program& program) noexcept {
	if (current_program_.id != program.id) {
		glUseProgram(program.id);
		current_program_.id = program.id;
	}
}

inline Program GL::bound_program() noexcept {
	return current_program_;
}

// -------------------------------------------------------------------------------------------------

inline void GL::framebuffer(Framebuffer object) noexcept {
	if (framebuffer_draw_.id != object.id || framebuffer_read_.id != object.id) {
		glBindFramebuffer(GL_FRAMEBUFFER, object.id);
		framebuffer_draw_.id = object.id;
		framebuffer_read_.id = object.id;
	}
}

inline void GL::framebuffer_draw(Framebuffer object) noexcept {
	if (framebuffer_draw_.id != object.id) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object.id);
		framebuffer_draw_.id = object.id;
	}
}

inline void GL::framebuffer_read(Framebuffer object) noexcept {
	if (framebuffer_read_.id != object.id) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, object.id);
		framebuffer_read_.id = object.id;
	}
}

inline void GL::framebuffer_default() noexcept {
	if (framebuffer_draw_.id != 0 || framebuffer_read_.id != 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		framebuffer_draw_.id = 0;
		framebuffer_read_.id = 0;
	}
}

inline void GL::framebuffer_default_draw() noexcept {
	if (framebuffer_draw_.id != 0) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		framebuffer_draw_.id = 0;
	}
}

inline void GL::framebuffer_default_read() noexcept {
	if (framebuffer_read_.id != 0) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		framebuffer_read_.id = 0;
	}
}

inline Framebuffer GL::framebuffer_draw() const noexcept {
	return framebuffer_draw_;
}

inline Framebuffer GL::framebuffer_read() const noexcept {
	return framebuffer_read_;
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
