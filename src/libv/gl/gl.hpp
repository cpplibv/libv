// Project: libv.gl, File: src/libv/gl/gl.hpp

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/algo/fill.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
// std
#include <array>
#include <stack>
#include <string_view>
#include <vector>
// pro
#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/assert.hpp>
#include <libv/gl/buffer_object.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/framebuffer_object.hpp>
#include <libv/gl/fwd.hpp>
#include <libv/gl/indirect_command_buffer_object.hpp>
#include <libv/gl/matrix_stack.hpp>
#include <libv/gl/pixel_buffer_object.hpp>
#include <libv/gl/program_object.hpp>
#include <libv/gl/query_object.hpp>
#include <libv/gl/renderbuffer_object.hpp>
#include <libv/gl/shader_object.hpp>
#include <libv/gl/shader_storage_buffer_object.hpp>
#include <libv/gl/texture_object.hpp>
#include <libv/gl/uniform_buffer_object.hpp>
#include <libv/gl/vertex_array_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class AccessArrayBuffer;
class AccessBuffer;
class AccessFence;
class AccessFramebuffer;
class AccessPixelBuffer;
class AccessProgram;
class AccessQuery;
class AccessRenderbuffer;
class AccessShader;
class AccessShaderStorageBuffer;
template <typename T> class AccessTexture;
class AccessUniformBuffer;
class AccessIndirectCommandBuffer;
class AccessVertexArray;

// -------------------------------------------------------------------------------------------------

struct BlendEquationSetter {
	BlendEquation current = BlendEquation::Add;

	inline void set(BlendEquation value) noexcept {
		if (current != value) {
			current = value;
			glBlendEquation(to_value(value));
			checkGL();
		}
	}
	inline void operator()(BlendEquation value) noexcept {
		set(value);
	}
	[[nodiscard]] inline auto set_guard(BlendEquation value) noexcept {
		const auto past = current;
		set(value);
		return libv::guard([this, past] { set(past); });
	}

	inline void add() noexcept {
		set(BlendEquation::Add);
	}
	[[nodiscard]] inline auto add_guard() noexcept {
		return set_guard(BlendEquation::Add);
	}
	inline void subtract() noexcept {
		set(BlendEquation::Subtract);
	}
	[[nodiscard]] inline auto subtract_guard() noexcept {
		return set_guard(BlendEquation::Subtract);
	}
	inline void reverseSubtract() noexcept {
		set(BlendEquation::ReverseSubtract);
	}
	[[nodiscard]] inline auto reverseSubtract_guard() noexcept {
		return set_guard(BlendEquation::ReverseSubtract);
	}
	inline void min() noexcept {
		set(BlendEquation::Min);
	}
	[[nodiscard]] inline auto min_guard() noexcept {
		return set_guard(BlendEquation::Min);
	}
	inline void max() noexcept {
		set(BlendEquation::Max);
	}
	[[nodiscard]] inline auto max_guard() noexcept {
		return set_guard(BlendEquation::Max);
	}
};

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
		const auto past_source = current_source;
		const auto past_destination = current_destination;
		set(source, destination);
		return libv::guard([this, past_source, past_destination] { set(past_source, past_destination); });
	}
};

// -------------------------------------------------------------------------------------------------

struct CapabilitySetter {
	template <std::size_t CAPABILITY>
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
			const auto past = current;
			enable();
			return libv::guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto disable_guard() noexcept {
			const auto past = current;
			disable();
			return libv::guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto set_guard(bool enable_) noexcept {
			const auto past = current;
			set(enable_);
			return libv::guard([this, past] { set(past); });
		}
	};

	Selector<to_value(Capability::Blend)> blend;
	Selector<to_value(Capability::CullFace)> cull;
	Selector<to_value(Capability::DepthTest)> depthTest;
	Selector<to_value(Capability::Multisample)> multisample;
	Selector<to_value(Capability::RasterizerDiscard)> rasterizerDiscard;
	Selector<to_value(Capability::ScissorTest)> scissorTest;
	Selector<to_value(Capability::StencilTest)> stencilTest;
	Selector<to_value(Capability::TextureCubeMapSeamless)> textureCubeMapSeamless;

	inline void operator()(Capability capability, bool enable) {
		switch (capability) {
			case Capability::Blend: return blend(enable);
			case Capability::CullFace: return cull(enable);
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
		const auto past = current;
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
		const auto past = current;
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
		const auto past = current;
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
		const auto past = current;
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
		const auto past = current;
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
	struct Color {
		bool currentR = true;
		bool currentG = true;
		bool currentB = true;
		bool currentA = true;

		// inline void init() {
		// 	current = get(GL_COLOR_WRITEMASK);
		// }

		inline void set(bool r, bool g, bool b, bool a) noexcept {
			if (currentR != r || currentG != g || currentB != b || currentA != a) {
				currentR = r;
				currentG = g;
				currentB = b;
				currentA = a;
				glColorMask(currentR, currentG, currentB, currentA);
				checkGL();
			}
		}
		/// Bitmask 0000'RGBA
		inline void set(uint8_t bitmask) noexcept {
			assert(bitmask <= 0b1111);
			set(
					(bitmask & (1u << 3u)) != 0,
					(bitmask & (1u << 2u)) != 0,
					(bitmask & (1u << 1u)) != 0,
					(bitmask & (1u << 0u)) != 0
			);
		}
		inline void operator()(bool r, bool g, bool b, bool a) noexcept {
			set(r, g, b, a);
		}
		[[nodiscard]] inline auto set_guard(bool r, bool g, bool b, bool a) noexcept {
			const auto pastR = currentR;
			const auto pastG = currentG;
			const auto pastB = currentB;
			const auto pastA = currentA;
			set(r, g, b, a);
			return libv::guard([this, pastR, pastG, pastB, pastA] {
				set(pastR, pastG, pastB, pastA);
			});
		}

		// void glColorMaski(GLuint buf, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	};

	struct Depth {
		bool current = true;

		// inline void init() {
		// 	current = get(GL_DEPTH_WRITEMASK);
		// }

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
			const auto past = current;
			enable();
			return libv::guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto disable_guard() noexcept {
			const auto past = current;
			disable();
			return libv::guard([this, past] { set(past); });
		}
		[[nodiscard]] inline auto set_guard(bool enable_) noexcept {
			const auto past = current;
			set(enable_);
			return libv::guard([this, past] { set(past); });
		}
	};
	struct Stencil {
		uint8_t bitmask = 0;

		// inline void init() {
		// 	current = get(GL_DEPTH_WRITEMASK);
		// }

		inline void set(uint8_t bitmask_) noexcept {
			if (bitmask != bitmask_) {
				bitmask = bitmask_;
				glStencilMask(bitmask_);
				checkGL();
			}
		}
		inline void operator()(uint8_t bitmask_) noexcept {
			set(bitmask_);
		}
		[[nodiscard]] inline auto set_guard(uint8_t bitmask_) noexcept {
			const auto past = bitmask;
			set(bitmask_);
			return libv::guard([this, past] { set(past); });
		}
	};

	Color color;
	Depth depth;
	Stencil stencil;
};

struct StencilSetter {
	struct Function {
		TestFunction function = TestFunction::Always;
		uint8_t reference = 0;
		uint8_t bitmask = 255;

		// inline void init() {
		// 	current = get(...);
		// }

		inline void set(TestFunction function_, uint8_t reference_, uint8_t bitmask_) noexcept {
			if (function != function_ || reference != reference_ || bitmask != bitmask_) {
				function = function_;
				reference = reference_;
				bitmask = bitmask_;
				glStencilFunc(libv::to_underlying(function), reference, bitmask);
				checkGL();
			}
		}
		inline void operator()(TestFunction function_, uint8_t reference_, uint8_t bitmask_) noexcept {
			set(function_, reference_, bitmask_);
		}
		[[nodiscard]] inline auto set_guard(TestFunction function_, uint8_t reference_, uint8_t bitmask_) noexcept {
			const auto pastFunction = function;
			const auto pastReference = reference;
			const auto pastBitmask = bitmask;
			set(function_, reference_, bitmask_);
			return libv::guard([this, pastFunction, pastReference, pastBitmask] {
				set(pastFunction, pastReference, pastBitmask);
			});
		}
	};

	struct Operation {
		StencilAction stencilFail = StencilAction::Keep;
		StencilAction depthFail = StencilAction::Keep;
		StencilAction stencilAndDepthPass = StencilAction::Keep;

		// inline void init() {
		// 	current = get(...);
		// }

		inline void set(StencilAction stencilFail_, StencilAction depthFail_, StencilAction stencilAndDepthPass_) noexcept {
			if (stencilFail != stencilFail_ || depthFail != depthFail_ || stencilAndDepthPass != stencilAndDepthPass_) {
				stencilFail = stencilFail_;
				depthFail = depthFail_;
				stencilAndDepthPass = stencilAndDepthPass_;
				glStencilOp(libv::to_underlying(stencilFail), libv::to_underlying(depthFail), libv::to_underlying(stencilAndDepthPass));
				checkGL();
			}
		}
		inline void operator()(StencilAction stencilFail_, StencilAction depthFail_, StencilAction stencilAndDepthPass_) noexcept {
			set(stencilFail_, depthFail_, stencilAndDepthPass_);
		}
		[[nodiscard]] inline auto set_guard(StencilAction stencilFail_, StencilAction depthFail_, StencilAction stencilAndDepthPass_) noexcept {
			const auto pastStencilFail = stencilFail;
			const auto pastDepthFail = depthFail;
			const auto pastStencilAndDepthPass = stencilAndDepthPass;
			set(stencilFail_, depthFail_, stencilAndDepthPass_);
			return libv::guard([this, pastStencilFail, pastDepthFail, pastStencilAndDepthPass] {
				set(pastStencilFail, pastDepthFail, pastStencilAndDepthPass);
			});
		}
	};

	Function function;
	Operation operation;
};

// -------------------------------------------------------------------------------------------------

class GL {
public:
	MatrixStack<libv::mat4f> model;
	MatrixStack<libv::mat4f> view;
	MatrixStack<libv::mat4f> projection;

private:
	bool debug = false;
	bool trace = false;

	int versionMajor;
	int versionMinor;

public:
	BlendEquationSetter blendEquation;
	BlendFunctionSetter blendFunction;
	CapabilitySetter capability;
	ClipPlanesSetter clipPlanes;
	MaskSetter mask;
	StencilSetter stencil;
	CullFaceSetter cullFace;
	DepthFunctionSetter depthFunction;
	FrontFaceSetter frontFace;
	PolygonModeSetter polygonMode;

	ClipOrigin clipOrigin = ClipOrigin::LowerLeft;
	ClipDepth clipDepth = ClipDepth::NegativeOneToOne;

private:
	VertexArray currentVertexArray{0};

	TextureUnit currentActiveTexture{0};
	std::array<std::array<uint32_t, 16>, 11> textureBindings; /// Target -> Unit -> ID

	Program currentProgram_{0};

	struct UniformBlockBinding {
		UniformBuffer ubo;
		uint32_t offset = 0;
		// uint32_t size = 0;
	};
	std::array<UniformBlockBinding, 64> uniformBlockBindings; /// Binding -> UBO, Offset, Size

	Framebuffer framebufferRead_{0};
	Framebuffer framebufferDraw_{0};

private:
	void init();

public:
	GL() {
		for (auto& units : textureBindings)
			libv::fill(units, 0);

		init(); // This init function calls into the OpenGL context
	}

public:
	[[nodiscard]] inline bool versionOrGreater(int major, int minor) const noexcept {
		return versionMajor > major || (versionMajor == major && versionMinor >= minor);
	}

public:
	[[nodiscard]] inline libv::mat4f mvp() const {
		return projection * view * model;
	}
	[[nodiscard]] inline libv::vec3f eye() const {
		return libv::vec::xyz(view.top().inverse_copy()[3]);
	}
	[[nodiscard]] inline libv::mat4f getMatrixTexture() const {
		return libv::mat4f{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.5f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f};
	}

public:
	template <typename T>
	[[nodiscard]] inline T get(GLenum param) const {
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

		else if constexpr (std::is_same_v<libv::vec3i, T>)
			glGetIntegerv(param, value.data());

		else if constexpr (std::is_same_v<libv::vec4f, T>)
			glGetFloatv(param, value.data());

		else
			static_assert(false, "Type T is not supported");

		return value;
	}

	template <typename T>
	[[nodiscard]] inline T getIndexed(GLenum param) const {
		T value;

		if constexpr (std::is_same_v<libv::vec3i, T>) {
			glGetIntegeri_v(param, 0, &value.x);
			glGetIntegeri_v(param, 1, &value.y);
			glGetIntegeri_v(param, 2, &value.z);

		} else
			static_assert(false, "Type T is not supported");

		return value;
	}

private:
	[[nodiscard]] static constexpr inline std::size_t convertToTargetIndex(TextureTarget target) noexcept;

public:
	[[nodiscard]] inline auto getViewport() const {
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		struct Result {
			libv::vec2i position;
			libv::vec2i size;
		};
		return Result{
				libv::vec2i{m_viewport[0], m_viewport[1]},
				libv::vec2i{m_viewport[2], m_viewport[3]}
		};
	}
	[[nodiscard]] inline libv::vec2i getViewportPosition() const {
		return getViewport().position;
	}
	[[nodiscard]] inline libv::vec2i getViewportSize() const {
		return getViewport().size;
	}

public:
	[[nodiscard]] inline GLint getMaxColorAttachments() const {
		return get<GLint>(GL_MAX_COLOR_ATTACHMENTS);
	}
	[[nodiscard]] inline GLint getMaxUniformBlockSize() const {
		return get<GLint>(GL_MAX_UNIFORM_BLOCK_SIZE);
	}
	[[nodiscard]] inline GLint getMaxUniformBufferBindings() const {
		return get<GLint>(GL_MAX_UNIFORM_BUFFER_BINDINGS);
	}
	[[nodiscard]] inline GLint getMaxVertexUniformBlocks() const {
		return get<GLint>(GL_MAX_VERTEX_UNIFORM_BLOCKS);
	}
	[[nodiscard]] inline GLint getMaxGeometryUniformBlocks() const {
		return get<GLint>(GL_MAX_GEOMETRY_UNIFORM_BLOCKS);
	}
	[[nodiscard]] inline GLint getMaxFragmentUniformBlocks() const {
		return get<GLint>(GL_MAX_FRAGMENT_UNIFORM_BLOCKS);
	}
	[[nodiscard]] inline GLint getMaxComputeUniformBlocks() const {
		return get<GLint>(GL_MAX_COMPUTE_UNIFORM_BLOCKS);
	}
	[[nodiscard]] inline GLint getMaxCombinedUniformBlocks() const {
		return get<GLint>(GL_MAX_COMBINED_UNIFORM_BLOCKS);
	}
	[[nodiscard]] inline GLint getMaxVertexAttribs() const {
		return get<GLint>(GL_MAX_VERTEX_ATTRIBS);
	}
	[[nodiscard]] inline GLint getMaxVertexUniformComponents() const {
		return get<GLint>(GL_MAX_VERTEX_UNIFORM_COMPONENTS);
	}
	[[nodiscard]] inline GLint getMaxSamples() const {
		return get<GLint>(GL_MAX_SAMPLES);
	}
	[[nodiscard]] inline GLint getMaxSamplesInteger() const {
		return get<GLint>(GL_MAX_INTEGER_SAMPLES);
	}
	[[nodiscard]] inline GLint getMaxTextureImageUnits() const {
		return get<GLint>(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	}
	[[nodiscard]] inline GLint getMaxTextureImageUnitsVertex() const {
		return get<GLint>(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
	}
	[[nodiscard]] inline GLint getMaxTextureImageUnitsGeometry() const {
		return get<GLint>(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS);
	}
	[[nodiscard]] inline GLint getMaxTextureImageUnitsCompute() const {
		return versionOrGreater(4, 3) ? get<GLint>(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS) : -1;
	}
	[[nodiscard]] inline GLint getMaxTextureImageUnitsTessControl() const {
		return get<GLint>(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS);
	}
	[[nodiscard]] inline GLint getMaxTextureImageUnitsTessEvaluation() const {
		return get<GLint>(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS);
	}
	[[nodiscard]] inline GLint getMaxTextureImageUnitsFragment() const {
		return get<GLint>(GL_MAX_TEXTURE_IMAGE_UNITS);
	}
	[[nodiscard]] inline GLint getCurrentAvailableVideoMemory() const {
		GLint availableKB[4]{-1, -1, -1, -1};

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
	[[nodiscard]] inline GLint getMaxTextureSize() const {
		return get<GLint>(GL_MAX_TEXTURE_SIZE);
	}
	[[nodiscard]] inline GLint getUniformBufferOffsetAlignment() const {
		return get<GLint>(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT);
	}

	[[nodiscard]] inline libv::vec3i getMaxComputeWorkGroupCount() const {
		return versionOrGreater(4, 3) ? getIndexed<libv::vec3i>(GL_MAX_COMPUTE_WORK_GROUP_COUNT) : libv::vec3i{-1};
	}
	[[nodiscard]] inline libv::vec3i getMaxComputeWorkGroupSize() const {
		return versionOrGreater(4, 3) ? getIndexed<libv::vec3i>(GL_MAX_COMPUTE_WORK_GROUP_SIZE) : libv::vec3i{-1};
	}
	[[nodiscard]] inline GLint getMaxComputeWorkGroupInvocations() const {
		return versionOrGreater(4, 3) ? get<GLint>(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS) : -1;
	}

private:
	static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userData);

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

	void pushDebugGroup(const std::string_view message) {
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 42000, static_cast<GLsizei>(message.size()), message.data());
	}

	[[nodiscard]] inline auto pushDebugGroup_guard(const std::string_view message) {
		pushDebugGroup(message);
		return libv::guard([this] { popDebugGroup(); });
	}

	void popDebugGroup() {
		glPopDebugGroup();
	}

public:
	inline void clipControl(ClipOrigin clipOrigin, ClipDepth clipDepth);

	inline void viewport(vec2i pos, vec2i size);
	inline void viewport(int32_t left, int32_t bottom, int32_t width, int32_t height);
	inline void blit(vec2i src_pos, vec2i src_size, vec2i dst_pos, vec2i dst_size, BufferBit mask, MagFilter filter);
	inline void dsa_blit(Framebuffer read, Framebuffer draw, vec2i src_pos, vec2i src_size, vec2i dst_pos, vec2i dst_size, BufferBit mask, MagFilter filter);

	// void line_width(float width); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLineWidth.xhtml
	// void point_size(float size); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPointSize.xhtml

	inline void clearColor(const libv::vec4f& col);
	inline void clearColor(float r, float g, float b, float a);
	inline void clearDepth(float d);
	inline void clearStencil(uint8_t value);
	inline void clear(BufferBit buffers = BufferBit::ColorDepthStencil);

	inline void readPixels(vec2i pos, vec2i size, ReadFormat format, DataType type, void* data) {
		// Not supported read data types:
		//		GL_FLOAT_32_UNSIGNED_INT_24_8_REV.
		//		GL_UNSIGNED_INT_10F_11F_11F_REV,
		//		GL_UNSIGNED_INT_24_8
		//		GL_UNSIGNED_INT_5_9_9_9_REV
		glReadPixels(pos.x, pos.y, size.x, size.y, libv::to_value(format), libv::to_value(type), data);
	}

	inline void readTextureImage(const Texture& texture, int32_t level, ReadFormat format, DataType type, void* data) {
		auto prev = boundTexture(texture.target);
		bind(texture);
		glGetTexImage(libv::to_value(texture.target), level, libv::to_value(format), libv::to_value(type), data);
		bind(prev);

//		// OpenGL 4.5
//		glGetTextureImage(libv::to_value(texture.id), level, libv::to_value(format), libv::to_value(type), buffer_size, data);
	}

public:
	inline void bind(const VertexArray& object) noexcept;
	inline void unbindVertexArray() noexcept;
	[[nodiscard]] inline VertexArray boundVertexArray() noexcept;
	inline void cleanupDestroyedVertexArray(uint32_t id) noexcept;

private:
	inline void bind(TextureTarget target, uint32_t id);
	inline void bindUnit(TextureUnit unit, TextureTarget target, uint32_t id);

public:
	inline void activeTexture(TextureUnit unit);

	template <TextureTarget Target>
	inline void bind(const Texture_t<Target>& texture);
	inline void bind(const Texture& texture);
	template <TextureTarget Target>
	inline void bindUnit(TextureUnit unit, const Texture_t<Target>& texture);
	inline void bindUnit(TextureUnit unit, const Texture& texture);
	template <TextureTarget Target>
	inline void unbind(const Texture_t<Target>& texture);
	inline void unbind(const Texture& texture);

	[[nodiscard]] inline Texture boundTexture(TextureTarget target) noexcept;
	[[nodiscard]] inline Texture boundTexture(TextureTarget target, TextureUnit unit) noexcept;

	inline void cleanupDestroyedTexture(TextureTarget target, uint32_t id) noexcept;

public:
	inline void useProgram(const Program& program) noexcept;
	[[nodiscard]] inline Program boundProgram() noexcept;

public:
	inline void bindUniformBufferBase(const UniformBuffer& object, uint32_t binding) noexcept;
	inline void bindUniformBufferRange(const UniformBuffer& object, uint32_t binding, uint32_t offset, uint32_t size) noexcept;
	inline void cleanupDestroyedBuffer(const UniformBuffer& object) noexcept;

public:
	inline void bindImageTexture(uint32_t unit, libv::gl::Texture texture, int32_t level, libv::gl::BufferAccessFull access, libv::gl::Format format) noexcept;
	inline void bindImageTexture(uint32_t unit, libv::gl::Texture texture, int32_t level, int32_t layer, libv::gl::BufferAccessFull access, libv::gl::Format format) noexcept;

public:
	inline void dispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y = 1, uint32_t num_groups_z = 1) noexcept;

public:
	inline void memoryBarrier(BarrierBit bits);

public:
	inline void framebuffer(Framebuffer object) noexcept;
	inline void framebuffer_draw(Framebuffer object) noexcept;
	inline void framebuffer_read(Framebuffer object) noexcept;
	inline void framebuffer_default() noexcept;
	inline void framebuffer_default_draw() noexcept;
	inline void framebuffer_default_read() noexcept;
	[[nodiscard]] inline Framebuffer framebuffer_draw() const noexcept;
	[[nodiscard]] inline Framebuffer framebuffer_read() const noexcept;
	inline void cleanupDestroyedFramebuffer(uint32_t id) noexcept;

public:
	template <typename Access = AccessUniformBuffer>
	inline Access operator()(UniformBuffer& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessIndirectCommandBuffer>
	inline Access operator()(IndirectCommandBuffer& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessArrayBuffer>
	inline Access operator()(ArrayBuffer& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessBuffer>
	static inline Access operator()(Buffer& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessFence>
	inline Access operator()(Fence& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessFramebuffer>
	inline Access operator()(Framebuffer& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessRenderbuffer>
	static inline Access operator()(Renderbuffer& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessPixelBuffer>
	inline Access operator()(PixelBuffer& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessProgram>
	inline Access operator()(Program& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessQuery>
	static inline Access operator()(Query& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessShader>
	static inline Access operator()(Shader& object) noexcept {
		return Access{object};
	}
	template <typename Access = AccessShaderStorageBuffer>
	inline Access operator()(ShaderStorageBuffer& object) noexcept {
		return Access{object, *this};
	}
	template <typename Access = AccessVertexArray>
	inline Access operator()(VertexArray& object) noexcept {
		return Access{object, *this};
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

void GL::clipControl(ClipOrigin clipOrigin, ClipDepth clipDepth) {
	if (this->clipOrigin == clipOrigin && this->clipDepth == clipDepth)
		return;
	this->clipOrigin = clipOrigin;
	this->clipDepth = clipDepth;
	glClipControl(libv::to_underlying(clipOrigin), libv::to_underlying(clipDepth));
	checkGL();
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
inline void GL::dsa_blit(Framebuffer read, Framebuffer draw, vec2i src_pos, vec2i src_size, vec2i dst_pos, vec2i dst_size, BufferBit mask, MagFilter filter) {
	LIBV_GL_DEBUG_ASSERT(read.id != 0);
	LIBV_GL_DEBUG_ASSERT(draw.id != 0);
	LIBV_GL_DEBUG_ASSERT(filter != MagFilter::Linear || mask == BufferBit::Color);

	glBlitNamedFramebuffer(
			read.id,
			draw.id,
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
inline void GL::clearDepth(float d) {
	glClearDepth(d);
	checkGL();
}
inline void GL::clearStencil(uint8_t value) {
	glClearStencil(value);
	checkGL();
}
inline void GL::clear(BufferBit buffers) {
	glClear(to_value(buffers));
	checkGL();
}

// -------------------------------------------------------------------------------------------------

inline void GL::bind(const VertexArray& object) noexcept {
	if (currentVertexArray.id != object.id) {
		currentVertexArray.id = object.id;
		glBindVertexArray(object.id);
		checkGL();
	}
}

inline void GL::unbindVertexArray() noexcept {
	currentVertexArray.id = 0;
	glBindVertexArray(0);
	checkGL();
}

inline VertexArray GL::boundVertexArray() noexcept {
	return currentVertexArray;
}

inline void GL::cleanupDestroyedVertexArray(uint32_t id) noexcept {
	if (currentVertexArray.id == id)
		currentVertexArray.id = 0;
}

// -------------------------------------------------------------------------------------------------

constexpr inline std::size_t GL::convertToTargetIndex(TextureTarget target) noexcept {
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
	// TODO P5: libv.gl: instead of default (0): log and assert invalid input
	return 0;
}

inline void GL::activeTexture(TextureUnit unit) {
	if (currentActiveTexture != unit) {
		currentActiveTexture = unit;
		glActiveTexture(GL_TEXTURE0 + to_value(unit));
		checkGL();
	}
}

inline void GL::bind(TextureTarget target, uint32_t id) {
	const std::size_t targetID = convertToTargetIndex(target);
	auto& binding = textureBindings[targetID][to_value(currentActiveTexture)];
	if (binding != id) {
		glBindTexture(to_value(target), id);
		binding = id;
	}
}
inline void GL::bindUnit(TextureUnit unit, TextureTarget target, uint32_t id) {
	const std::size_t targetID = convertToTargetIndex(target);
	auto& binding = textureBindings[targetID][to_value(unit)];
	if (binding != id) {
		glBindTextureUnit(to_value(unit), id);
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
inline void GL::bindUnit(TextureUnit unit, const Texture_t<Target>& texture) {
	bindUnit(unit, texture.target, texture.id);
}
inline void GL::bindUnit(TextureUnit unit, const Texture& texture) {
	bindUnit(unit, texture.target, texture.id);
}
template <TextureTarget Target>
inline void GL::unbind(const Texture_t<Target>& texture) {
	bind(texture.target, 0);
}
inline void GL::unbind(const Texture& texture) {
	bind(texture.target, 0);
}

inline Texture GL::boundTexture(TextureTarget target) noexcept {
	const std::size_t targetID = convertToTargetIndex(target);
	return Texture{textureBindings[targetID][+currentActiveTexture], target};
}
inline Texture GL::boundTexture(TextureTarget target, TextureUnit unit) noexcept {
	const std::size_t targetID = convertToTargetIndex(target);
	return Texture{textureBindings[targetID][+unit], target};
}

inline void GL::cleanupDestroyedTexture(TextureTarget target, uint32_t id) noexcept {
	const std::size_t targetID = convertToTargetIndex(target);
	for (auto& activeID : textureBindings[targetID])
		if (activeID == id)
			activeID = 0;
}

// -------------------------------------------------------------------------------------------------

inline void GL::useProgram(const Program& program) noexcept {
	if (currentProgram_.id != program.id) {
		currentProgram_.id = program.id;
		glUseProgram(program.id);
		checkGL();
	}
}

inline Program GL::boundProgram() noexcept {
	return currentProgram_;
}

// -------------------------------------------------------------------------------------------------

inline void GL::bindUniformBufferBase(const UniformBuffer& object, uint32_t binding) noexcept {
	if (binding < std::size(uniformBlockBindings)) {
		auto& current = uniformBlockBindings[binding];
		// if (current.ubo != object && current.offset != offset && current.size != size)
		if (current.ubo.id == object.id && current.offset == 0)
			return;

		current.ubo = object;
		current.offset = 0;
		// current.size = size;
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, object.id);
	checkGL();
}

inline void GL::bindUniformBufferRange(const UniformBuffer& object, uint32_t binding, uint32_t offset, uint32_t size) noexcept {
	if (binding < std::size(uniformBlockBindings)) {
		auto& current = uniformBlockBindings[binding];
		// if (current.ubo != object && current.offset != offset && current.size != size)
		if (current.ubo.id == object.id && current.offset == offset)
			return;

		current.ubo = object;
		current.offset = offset;
		// current.size = size;
	}
	glBindBufferRange(GL_UNIFORM_BUFFER, binding, object.id, offset, size);
	checkGL();
}

inline void GL::cleanupDestroyedBuffer(const UniformBuffer& object) noexcept {
	for (auto& binding : uniformBlockBindings) {
		if (binding.ubo.id == object.id) {
			binding.ubo.id = 0;
			binding.offset = 0;
		}
	}
}

// -------------------------------------------------------------------------------------------------

inline void GL::bindImageTexture(
		uint32_t unit,
		libv::gl::Texture texture,
		int32_t level,
		libv::gl::BufferAccessFull access,
		libv::gl::Format format) noexcept {
	glBindImageTexture(unit, texture.id, level, true, 0, libv::to_underlying(access), format.format);
	checkGL();
}

inline void GL::bindImageTexture(
		uint32_t unit,
		libv::gl::Texture texture,
		int32_t level,
		int32_t layer,
		libv::gl::BufferAccessFull access,
		libv::gl::Format format) noexcept {
	glBindImageTexture(unit, texture.id, level, false, layer, libv::to_underlying(access), format.format);
	checkGL();
}

// -------------------------------------------------------------------------------------------------

inline void GL::dispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z) noexcept {
	glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
	checkGL();
}

// -------------------------------------------------------------------------------------------------

inline void GL::memoryBarrier(BarrierBit bits) {
	glMemoryBarrier(libv::to_underlying(bits));
	checkGL();
}

// -------------------------------------------------------------------------------------------------

inline void GL::framebuffer(Framebuffer object) noexcept {
	if (framebufferDraw_.id != object.id || framebufferRead_.id != object.id) {
		glBindFramebuffer(GL_FRAMEBUFFER, object.id);
		framebufferDraw_.id = object.id;
		framebufferRead_.id = object.id;
	}
}

inline void GL::framebuffer_draw(Framebuffer object) noexcept {
	if (framebufferDraw_.id != object.id) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, object.id);
		framebufferDraw_.id = object.id;
	}
}

inline void GL::framebuffer_read(Framebuffer object) noexcept {
	if (framebufferRead_.id != object.id) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, object.id);
		framebufferRead_.id = object.id;
	}
}

inline void GL::framebuffer_default() noexcept {
	if (framebufferDraw_.id != 0 || framebufferRead_.id != 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		framebufferDraw_.id = 0;
		framebufferRead_.id = 0;
	}
}

inline void GL::framebuffer_default_draw() noexcept {
	if (framebufferDraw_.id != 0) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		framebufferDraw_.id = 0;
	}
}

inline void GL::framebuffer_default_read() noexcept {
	if (framebufferRead_.id != 0) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		framebufferRead_.id = 0;
	}
}

inline Framebuffer GL::framebuffer_draw() const noexcept {
	return framebufferDraw_;
}

inline Framebuffer GL::framebuffer_read() const noexcept {
	return framebufferRead_;
}

inline void GL::cleanupDestroyedFramebuffer(uint32_t id) noexcept {
	if (framebufferDraw_.id == id)
		framebufferDraw_.id = 0;
	if (framebufferRead_.id == id)
		framebufferRead_.id = 0;
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
