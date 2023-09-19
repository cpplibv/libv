// Project: libv.re, File: src/libv/re/core/pass/render_passes.cpp

#include <libv/re/core/pass/render_pass_outline.hpp>

#include <libv/algo/linear_find.hpp>
#include <libv/gl/gl.hpp>
#include <libv/re/core/context/context_pipeline.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/re/core/render_target.hpp>
#include <libv/re/material/internal/material_jfa_outline.hpp>
#include <libv/re/material/internal/material_jfa_step.hpp>
#include <libv/re/material/internal/material_outline_mask.hpp>
#include <libv/re/material/material.hpp>
#include <libv/re/mesh/mesh_fullscreen.hpp>
#include <libv/re/resource/shader_loader.hpp>
#include <libv/re/resource/texture.hpp>
#include <libv/utility/ceil_div.hpp>
#include <libv/re/log.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

RenderPassOutline::RenderPassOutline() :
	RenderPass("Outline") {
	layerMask = layerOutlineCaster;
	sortingOrder = RenderSortingOrder::none;

	auto& state = overrideState.emplace();
	state.depthTest(true);
	state.depthWrite(false);
	state.depthFunction(libv::gl::TestFunction::GEqual);
	state.stencilTest(false);
	state.stencilWrite(0x00);

	const auto spec = Texture2DSpec{
		libv::gl::Wrap::ClampToEdge,
		libv::gl::Wrap::ClampToEdge,
		libv::gl::MagFilter::Nearest,
		libv::gl::MinFilter::Nearest,
	};
	rtMask = RenderTargetTextures_ptr::make();
	rtMask->add(libv::gl::Attachment::Color0, libv::gl::FormatSized::RG8, spec);

	textureJFASteps = Texture::create("outline-jfa-steps");
	textureJFASteps->set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
	textureJFASteps->set(libv::gl::MagFilter::Nearest);
	textureJFASteps->set(libv::gl::MinFilter::Nearest);

	materialJFAInit = Material::create(r.shader.load("shader/re/outline/jump_flood_init.cs"));
	materialJFAStep = MaterialJFAStep::create();
	materialJFAOutline = MaterialJFAOutline::create(outlineRadius);
}

void RenderPassOutline::doUpdate(ContextUpdate& ctx) {
	// --- Assign color IDs ---

	static constexpr uint32_t maxNumColors = UniformBlockJFAOutline::maxNumColors;
	auto& colors = materialJFAOutline->block->outlineColors;
	colors.fill(libv::vec4f{-1});
	uint32_t numColors = 0;

	Material* lastMaterial = nullptr;
	for (const auto& entry : renderNodeEntries) {
		if (lastMaterial == entry.material)
			continue;

		assert(dynamic_cast<MaterialOutlineMask*>(entry.material) && "Invalid outline material type");
		auto& material = static_cast<MaterialOutlineMask&>(*entry.material);

		auto i = libv::linear_find_index(colors, material.color);
		if (i != -1) {
			material.colorID = static_cast<float>(i);
		} else {
			if (numColors == maxNumColors) {
				material.colorID = 0.f;
			} else {
				material.colorID = static_cast<float>(numColors);
				colors[numColors++] = material.color;
			}
		}
	}

	const auto colorIDEncoder = 1.0f / static_cast<float>(std::max(1u, numColors - 1));
	const auto colorIDDecoder = static_cast<float>(std::max(1u, numColors - 1));
	lastMaterial = nullptr;
	for (const auto& entry : renderNodeEntries) {
		if (lastMaterial == entry.material)
			continue;

		auto& material = static_cast<MaterialOutlineMask&>(*entry.material);
		material.colorID.value *= colorIDEncoder;
	}

	const auto jfaSteps = std::bit_width(static_cast<uint32_t>(outlineRadius + 0.5f));
	const auto finalLayer = jfaSteps % 2;
	materialJFAOutline->block->sourceLayer = finalLayer;
	materialJFAOutline->block->outlineRadius = outlineRadius;
	materialJFAOutline->block->outlineColorIDDecoder = colorIDDecoder;

	// Update materials

	materialJFAInit->gl_update(ctx);
	materialJFAStep->gl_update(ctx);
	materialJFAOutline->gl_update(ctx);
}

void RenderPassOutline::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
	if (renderNodeEntries.empty())
		return;

	// --- Init mask ---

	// Jump Flood Algorithm
	// Based on https://bgolus.medium.com/the-quest-for-very-wide-outlines-ba82ed442cd9

	const auto canvasSize = pipeline.rtStack.top()->size();

	{
		const auto rtGuard = pipeline.rtStack.pushGuard(ctx.gl, rtMask);
		// const auto rtGuard = pipeline.rtStack.pushGuard(ctx.gl, rtMask, 0, false);
		ctx.gl.clear(libv::gl::BufferBit::Color);

		gl_renderNodes(ctx);

		pipeline.debug.gl_recordColor("outline-jfa-mask", pipeline, ctx, libv::gl::Attachment::Color0);
	}

	// --- JFA Init ---

	rtMask->gl_resolve(ctx.gl, libv::gl::BufferBit::Color);
	rtMask->textureSSColor(libv::gl::Attachment::Color0)->gl_bind(ctx.gl, unitJFAMask);

	// On 4K resolution 16_SNORM gives 16x16 position per pixel
	textureJFASteps->storage(libv::gl::TextureTarget::_2DArray, libv::gl::FormatSized::RG16_SNORM, 1, libv::vec3i{pipeline.rtStack.top()->size(), 2}, 0, false);
	textureJFASteps->gl_bind(ctx.gl, unitJFAStep);
	textureJFASteps->gl_bindImage(ctx.gl, unitJFAStepOutput, 0, libv::gl::BufferAccessFull::Write);

	materialJFAInit->gl_bind(ctx);
	ctx.gl.dispatchCompute(libv::ceil_div(canvasSize.x, 8), libv::ceil_div(canvasSize.y, 8), 1);
	ctx.gl.memoryBarrier(libv::gl::BarrierBit::ShaderImageAccess);

	pipeline.debug.gl_recordTextureLayer("outline-jfa-init", 0, pipeline, ctx, textureJFASteps, 0, 0);

	// --- JFA Flood Step ---

	const auto jfaSteps = std::bit_width(static_cast<uint32_t>(outlineRadius + 0.5f));
	for (int32_t i = 0; i < jfaSteps; ++i) {
		const auto sourceLayer = (i + 0) % 2;
		const auto targetLayer = (i + 1) % 2;
		const auto stepWidth = static_cast<int32_t>(1u << (jfaSteps - i - 1u));

		materialJFAStep->packStepSourceTarget = libv::vec3i{stepWidth, sourceLayer, targetLayer};
		materialJFAStep->gl_bind(ctx);

		ctx.gl.dispatchCompute(libv::ceil_div(canvasSize.x, 8), libv::ceil_div(canvasSize.y, 8), 1);
		ctx.gl.memoryBarrier(libv::gl::BarrierBit::ShaderImageAccess);

		pipeline.debug.gl_recordTextureLayer("outline-jfa-step", i, pipeline, ctx, textureJFASteps, 0, targetLayer);
	}

	// --- JFA Outline ---

	materialJFAOutline->gl_bind(ctx);

	pipeline.fullscreenMesh->gl_render(ctx);

	pipeline.debug.gl_recordColor("outline-jfa-outline", pipeline, ctx, libv::gl::Attachment::Color0);
	pipeline.debug.gl_recordColor("outline", pipeline, ctx, libv::gl::Attachment::Color0);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
