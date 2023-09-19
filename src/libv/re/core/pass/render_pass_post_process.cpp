// Project: libv.re, File: src/libv/re/core/pass/render_passes.cpp

#include <libv/re/core/pass/render_pass_post_process.hpp>

#include <libv/gl/gl.hpp>
#include <libv/re/core/context/context_pipeline.hpp>
#include <libv/re/core/render_target.hpp>
#include <libv/re/material/basic_material.hpp>
#include <libv/re/material/material.hpp>
#include <libv/re/mesh/mesh_fullscreen.hpp>
#include <libv/re/resource/shader_loader.hpp>
#include <libv/re/resource/texture.hpp>
#include <libv/re/resource/texture_loader.hpp>
#include <libv/utility/align.hpp>
#include <libv/utility/ceil_div.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct UniformBlockPostProcess {
	static constexpr std::string_view name = builtinBlockNamePostProcess;
	static constexpr BlockBinding binding = BlockBinding::passRelated;

	// float bloomThreshold = 1.2f; /// [0..inf) Filter out pixels under this level of brightness.
	// float bloomKnee = 0.2f;      /// [0..bloomThreshold)
	float bloomThreshold = 0.6f; /// [0..inf) Filter out pixels under this level of brightness.
	float bloomKnee = 0.4f;      /// [0..bloomThreshold)
	float bloomDecay = 1.0f;     /// [0..1] Decay between downsample levels
	float bloomFinalMultiplier = 1.0f; /// Derived value = bloomIntensity / bloomPyramidMagnitude

	float bloomCombineAdd = 0.0f; /// [0..1]
	float bloomCombineMix = 0.04f; /// [0..1]
	libv::vec2f bloomBoundingPosition; /// Derived value

	libv::vec2f bloomBoundingSize; /// Derived value
	float vignetteIntensity = 0.25f;  /// [0..1] Amount of vignetting on the screen
	float vignetteSmoothness = 0.40f; /// (0..1] Smoothness of the vignette borders (Cannot be zero)

	float vignetteRoundness = 1.00f;  /// [0, 1] Should the vignette be round or dependent on the aspect ratio
	Padding<12> _p1;

	libv::vec3f vignetteColor = {0.f, 0.f, 0.f}; /// Vignette color (Used as a multiple operation)
	Padding<4> _p2;

	libv::vec3f whitePoint = {5, 5, 5}; // [0..inf) White point in the input (what should yield white output after tonemapping)
	Padding<4> _p3;
	libv::vec3f whiteCurve = {7, 7, 7}; // [0..inf) White on the tonemap curve (how much should be used from the tonemap curve)
	Padding<4> _p4;

	template <typename Access>
	constexpr void access_uniforms(Access& access) {
		access(bloomThreshold, "bloomThreshold");
		access(bloomKnee, "bloomKnee");
		access(bloomDecay, "bloomDecay");
		access(bloomFinalMultiplier, "bloomFinalMultiplier");
		access(bloomCombineAdd, "bloomCombineAdd");
		access(bloomCombineMix, "bloomCombineMix");
		// access(_p0, "_p0");
		access(bloomBoundingPosition, "bloomBoundingPosition");
		access(bloomBoundingSize, "bloomBoundingSize");
		access(vignetteIntensity, "vignetteIntensity");
		access(vignetteSmoothness, "vignetteSmoothness");
		access(vignetteRoundness, "vignetteRoundness");
		access(_p1, "_p1");
		access(vignetteColor, "vignetteColor");
		access(_p2, "_p2");
		access(whitePoint, "whitePoint");
		access(_p3, "_p3");
		access(whiteCurve, "whiteCurve");
		access(_p4, "_p4");
	}

	[[nodiscard]] bool operator==(const UniformBlockPostProcess& other) const noexcept = default;
};

// -------------------------------------------------------------------------------------------------

class MaterialPPCompute : public Material {
public:
	Uniform<UniformBlockPostProcess> block;
	Uniform<int32_t> currentLevel;

	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
		access(currentLevel, "currentLevel");
		access(block);
	}

public:
	explicit MaterialPPCompute(const UniformBlock_ptr<UniformBlockPostProcess>& block, std::string_view computeShaderPath) :
		Material(r.shader.load(ShaderType::Compute, computeShaderPath), nullptr),
		block(block) {

		MaterialAccessor_assignShader access{shader, shaderDepth};
		access_uniforms(access);
	}
	inline MaterialPPCompute(const MaterialPPCompute&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialPPCompute& other) const noexcept = default;

public:
	virtual void gl_update(ContextUpdate& ctx) override {
		MaterialAccessor_gl_update access{ctx};
		access_uniforms(access);
	}
	virtual void gl_bind(ContextRender& ctx) override {
		MaterialAccessor_gl_bind access{ctx, shader, state};
		access_uniforms(access);
	}
	virtual void gl_bindDepth(ContextRender& ctx) override {
		MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
		access_uniforms(access);
	}
	virtual bool equals(const Material& other) const noexcept override {
		return static_cast<cptr::value_type&>(other) == *this;
	}
	[[nodiscard]] virtual Material_ptr doClone() const override {
		return ptr::make(*this);
	}
	[[nodiscard]] inline ptr clone() const { return static_pointer_cast<ptr::value_type>(doClone()); };
};

void increase_ref_count(MaterialPPCompute* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialPPCompute* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

class MaterialPPCombine : public Material {
public:
	Uniform<Texture> textureVignette = unitPPVignette;
	Uniform<Texture> textureBloomDirt = unitPPBloomDirt;
	Uniform<UniformBlockPostProcess> block;

	template <typename Access>
	void access_uniforms(Access& access) {
		Material::access_uniforms(access);
		access(textureVignette, "textureVignette");
		access(textureBloomDirt, "textureBloomDirt");
		access(block);
	}

public:
	MaterialPPCombine(const UniformBlock_ptr<UniformBlockPostProcess>& block, const Shader_ptr& shader_) :
		Material(shader_, nullptr),
		block(block) {

		textureVignette = r.texture.load("builtin:white");
		textureBloomDirt = textureVignette;

		state.depthTest(false);

		MaterialAccessor_assignShader access{shader, shaderDepth};
		access_uniforms(access);
	}
	inline MaterialPPCombine(const MaterialPPCombine&) noexcept = default;
	[[nodiscard]] bool operator==(const MaterialPPCombine& other) const noexcept = default;

public:
	virtual void gl_update(ContextUpdate& ctx) override {
		MaterialAccessor_gl_update access{ctx};
		access_uniforms(access);
	}
	virtual void gl_bind(ContextRender& ctx) override {
		MaterialAccessor_gl_bind access{ctx, shader, state};
		access_uniforms(access);
	}
	virtual void gl_bindDepth(ContextRender& ctx) override {
		MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
		access_uniforms(access);
	}
	virtual bool equals(const Material& other) const noexcept override {
		return static_cast<cptr::value_type&>(other) == *this;
	}
	[[nodiscard]] virtual Material_ptr doClone() const override {
		return ptr::make(*this);
	}
	[[nodiscard]] inline ptr clone() const { return static_pointer_cast<ptr::value_type>(doClone()); };
};

void increase_ref_count(MaterialPPCombine* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialPPCombine* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// =================================================================================================

RenderPassPostProcess::BloomState::BloomState(const UniformBlock_ptr<UniformBlockPostProcess>& uniformBlock) {
	materialDownsamplePrefilter = MaterialPPCompute_ptr::make(uniformBlock, "shader/re/post_process/pp_bloom_down_prefilter.cs");
	materialDownsample = MaterialPPCompute_ptr::make(uniformBlock, "shader/re/post_process/pp_bloom_down.cs");
	materialUpsample = MaterialPPCompute_ptr::make(uniformBlock, "shader/re/post_process/pp_bloom_up.cs");

	texturePyramid = Texture::create("bloom-pyramid");
	texturePyramid->set(libv::gl::Wrap::ClampToBorder, libv::gl::Wrap::ClampToBorder);
	// texturePyramid->set(libv::gl::Wrap::MirroredRepeat, libv::gl::Wrap::MirroredRepeat);
	// texturePyramid->set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
	// ClampToBorder or MirroredRepeat is the best, but each of them has pros and cons
	texturePyramid->set(libv::gl::MinFilter::LinearMipmapNearest);
	texturePyramid->set(libv::gl::MagFilter::Linear);
}

void RenderPassPostProcess::BloomState::calculateBloomPyramidInfo(libv::vec2ui framebufferSize, uint32_t maxBloomStepSize) noexcept {
	assert(std::has_single_bit(maxBloomStepSize));
	assert(maxBloomStepSize >= 8u);

	const auto sourceSize = libv::max(framebufferSize, libv::vec2ui{1, 1});
	const auto longerSide = std::max(sourceSize.x, sourceSize.y);

	const auto alignment = std::min(std::bit_ceil(longerSide), maxBloomStepSize);
	const auto levels = std::bit_width(libv::align_of_2(longerSide, alignment));
	// Correct would be extraPadding = alignment, but it would waste too much for little to no gain);
	// const auto extraPadding = alignment;
	const auto extraPadding = alignment / 2;

	const libv::vec2ui activeSize = libv::ceil_div(sourceSize, 2u);
	numMips = std::min(levels, std::bit_width(maxBloomStepSize));
	storageSize = libv::align_of_2(activeSize, alignment) + 2 * extraPadding;
	bloomBoundingPosition = libv::vec2ui::one(extraPadding);
	bloomBoundingSize = activeSize;
}

// =================================================================================================

RenderPassPostProcess::RenderPassPostProcess() :
	RenderPassNodeless("PostProcess") {
	layerMask = layerNone;
	sortingOrder = RenderSortingOrder::none;

	uniformBlock = UniformBlock_ptr<UniformBlockPostProcess>::create();

	if (settings.bloom.enable)
		bloomState.emplace(uniformBlock);

	// TODO P2: Post process settings setters
	// bloom(settings.bloom.enable);
	// bloomThreshold(settings.bloom.bloomThreshold);
	// bloomKnee(settings.bloom.bloomKnee);
	// bloomDecay(settings.bloom.bloomDecay);
	// bloomIntensity(settings.bloom.bloomIntensity);
	// bloomCombineAdd(settings.bloom.bloomCombineAdd);
	// bloomCombineMix(settings.bloom.bloomCombineMix);

	{
		auto load_guard = r.shader.load_delayed(ShaderType::Vertex, "shader/re/post_process/fullscreen.vs", ShaderType::Fragment, "shader/re/post_process/pp_combine.fs");
		shaderCombine = load_guard.resource;
		if (load_guard.isFirstLoad()) {
			if (settings.bloom.enable)
				shaderCombine->define("ENABLE_BLOOM");
			// shaderCombine->define("ENABLE_BLOOM_DIRT");
			if (settings.vignette.enable)
				shaderCombine->define("ENABLE_VIGNETTE");
			// shaderCombine->define("ENABLE_VIGNETTE_TEXTURE");
			// shaderCombine->define("ENABLE_EXPOSURE");
			if (settings.tonemap.enable) {
				shaderCombine->define("ENABLE_TONEMAPPING");
				shaderCombine->define("TONEMAP_FN " + settings.tonemap.tonemapFunction);
			}
			if (settings.tonemapWithWhite.enable) {
				shaderCombine->define("ENABLE_TONEMAPPING_WITH_WHITE");
				shaderCombine->define("TONEMAP_FN " + settings.tonemap.tonemapFunction);
			}
			if (settings.gammaCorrection.enable)
				shaderCombine->define("ENABLE_GAMMA_CORRECTION");
		}
	}

	materialCombine = MaterialPPCombine_ptr::make(uniformBlock, shaderCombine);
}

void RenderPassPostProcess::doUpdate(ContextUpdate& ctx) {
	if (bloomState) {
		// TODO P3: This coupling between ctx.canvasSize and pipeline.rtStack.top()->size() is
		// 	 	too lose, and it might not match. Once rtStack push manipulation happens inside this class
		//		this can be removed.
		const auto canvasSize = ctx.frame.canvasSize;
		const auto maxBloomStepSize = 128u;
		bloomState->calculateBloomPyramidInfo(canvasSize.cast<uint32_t>(), maxBloomStepSize);

		uniformBlock->bloomBoundingPosition = bloomState->bloomBoundingPosition.cast<float>();
		uniformBlock->bloomBoundingSize = bloomState->bloomBoundingSize.cast<float>();
		const auto bloomPyramidMagnitude = std::pow(uniformBlock->bloomDecay, static_cast<float>(bloomState->numMips));
		uniformBlock->bloomFinalMultiplier = settings.bloom.bloomIntensity / bloomPyramidMagnitude;

		// Materials that are not submitted into the render queue has to be updated manually
		bloomState->materialDownsamplePrefilter->gl_update(ctx);
		bloomState->materialDownsample->gl_update(ctx);
		bloomState->materialUpsample->gl_update(ctx);
	}

	// Materials that are not submitted into the render queue has to be updated manually
	materialCombine->gl_update(ctx);
}

void RenderPassPostProcess::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
	// No gl_renderNodes as RenderPassPostProcess is nodeless

	// --- Setup ---

	// Resolve color results
	pipeline.rtStack.top()->gl_resolve(ctx.gl, libv::gl::BufferBit::Color);
	pipeline.rtStack.top()->textureSSColor(libv::gl::Attachment::Color0)->gl_bind(ctx.gl, unitPPScene);

	// Switch to the previous render target
	pipeline.rtStack.prev()->gl_drawBuffersAll(ctx.gl);
	pipeline.rtStack.pop(ctx.gl);
	// TODO P4: This pop() feel a bit random, it would be nice if post process step would have a setup
	//			step which runs as a different pass where it owns the main render target and puts it into the rtStack
	//			Would make ownerships nice

	// --- Bloom ---
	if (bloomState) {
		// GLSL does not support RGB16F as image unit, RGBA16F has to be used
		bloomState->texturePyramid->storage(libv::gl::TextureTarget::_2D, libv::gl::FormatSized::RGBA16F, static_cast<int32_t>(bloomState->numMips), bloomState->storageSize.cast<int32_t>(), 0, false);
		bloomState->texturePyramid->gl_bind(ctx.gl, unitPPBloomPyramid);
		bloomState->texturePyramid->gl_bindImage(ctx.gl, unitPPBloomPyramidOutput, 0, libv::gl::BufferAccessFull::Write);

		// Bloom: Downsample and Prefilter on the first MIP
		bloomState->materialDownsamplePrefilter->gl_bind(ctx);
		ctx.gl.dispatchCompute(libv::ceil_div(bloomState->storageSize.x, 8u), libv::ceil_div(bloomState->storageSize.y, 8u), 1);
		ctx.gl.memoryBarrier(libv::gl::BarrierBit::ShaderImageAccess);
		pipeline.debug.gl_recordTextureLayer("post-process-prefilter", 0, pipeline, ctx, bloomState->texturePyramid, 0, -1);

		// Bloom: Downsample
		for (uint32_t i = 1; i < bloomState->numMips; ++i) {
			bloomState->texturePyramid->gl_bindImage(ctx.gl, unitPPBloomPyramidOutput, i, libv::gl::BufferAccessFull::Write);

			bloomState->materialDownsample->currentLevel = i;
			bloomState->materialDownsample->gl_bind(ctx);

			const auto activeSize = bloomState->storageSize >> i;
			ctx.gl.dispatchCompute(libv::ceil_div(activeSize.x, 8u), libv::ceil_div(activeSize.y, 8u), 1);
			ctx.gl.memoryBarrier(libv::gl::BarrierBit::ShaderImageAccess);
			pipeline.debug.gl_recordTextureLayer("post-process-down", i - 1, pipeline, ctx, bloomState->texturePyramid, i, -1);
		}

		// Bloom: Upsample
		for (int i = bloomState->numMips - 2; i >= 0; --i) {
			const auto activeSize = bloomState->storageSize >> i;

			bloomState->texturePyramid->gl_bindImage(ctx.gl, unitPPBloomPyramidOutput, i, libv::gl::BufferAccessFull::Write);

			bloomState->materialUpsample->currentLevel = i;
			bloomState->materialUpsample->gl_bind(ctx);

			ctx.gl.dispatchCompute(libv::ceil_div(activeSize.x, 8u), libv::ceil_div(activeSize.y, 8u), 1);
			ctx.gl.memoryBarrier(libv::gl::BarrierBit::ShaderImageAccess);
			pipeline.debug.gl_recordTextureLayer("post-process-up", i, pipeline, ctx, bloomState->texturePyramid, i, -1);
		}
	}

	// --- Combine ---

	// self->vignetteIntensity_ * 3.0f); // NOTE: Multiply by 3 to allow the 0..1 configuration range to yield a reasonable 3 as max value
	// self->vignetteSmoothness_ * 5.0f); // NOTE: Multiply by 5 to allow the 0..1 configuration range to yield a reasonable 5 as max value

	materialCombine->gl_bind(ctx);
	pipeline.fullscreenMesh->gl_render(ctx);

	pipeline.debug.gl_recordColor("post-process", pipeline, ctx, libv::gl::Attachment::Color0);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv

// //	void PostProcessing::bloom(bool value) noexcept {
// //		bloom_ = value;
// //	}
// //	bool PostProcessing::bloom() const noexcept {
// //		return bloom_;
// //	}
// void PostProcessing::bloomIntensity(float value) noexcept {
// 	self->bloomIntensity_ = value;
// }
// float PostProcessing::bloomIntensity() const noexcept {
// 	return self->bloomIntensity_;
// }
// void PostProcessing::bloomThreshold(float value) noexcept {
// 	self->bloomThreshold_ = value;
// }
// float PostProcessing::bloomThreshold() const noexcept {
// 	return self->bloomThreshold_;
// }
// void PostProcessing::bloomKnee(float value) noexcept {
// 	self->bloomKnee_ = value;
// }
// float PostProcessing::bloomKnee() const noexcept {
// 	return self->bloomKnee_;
// }
//
// //	void PostProcessing::vignette(bool value) noexcept {
// //		self->vignette_ = value;
// //	}
// //	bool PostProcessing::vignette() const noexcept {
// //		return self->vignette_;
// //	}
// void PostProcessing::vignetteIntensity(float value) noexcept {
// 	self->vignetteIntensity_ = value;
// }
// float PostProcessing::vignetteIntensity() const noexcept {
// 	return self->vignetteIntensity_;
// }
// void PostProcessing::vignetteRoundness(float value) noexcept {
// 	self->vignetteRoundness_ = value;
// }
// float PostProcessing::vignetteRoundness() const noexcept {
// 	return self->vignetteRoundness_;
// }
// void PostProcessing::vignetteSmoothness(float value) noexcept {
// 	self->vignetteSmoothness_ = value;
// }
// float PostProcessing::vignetteSmoothness() const noexcept {
// 	return self->vignetteSmoothness_;
// }

