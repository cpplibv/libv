// Project: libv.re, File: src/libv/re/core/pass/render_pass_depth_prepass.cpp

#include <libv/re/core/pass/render_pass_depth_prepass.hpp>

#include <libv/gl/gl.hpp>
#include <libv/re/common/texture_unit.hpp>
#include <libv/re/core/context/context_pipeline.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/re/core/render_target.hpp>
#include <libv/re/resource/texture.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

RenderPassDepthPrime::RenderPassDepthPrime() :
	RenderPass("DepthPrime") {
	layerMask = layerDepthCaster;
	sortingOrder = RenderSortingOrder::shaderDepthThenDistanceIncreasing;

	auto& state = overrideState.emplace();
	state.depthTest(true);
	state.depthWrite(true);
	state.depthFunction(libv::gl::TestFunction::Greater);
	state.stencilTest(false);
	state.stencilWrite(0x00);

	// RenderState stateMask;
	// stateMask.mask_blend();
	// stateMask.mask_blendDst();
	// stateMask.mask_blendSrc();
	// stateMask.mask_depthFunction();
	// stateMask.mask_depthTest();
	// stateMask.mask_depthWrite();
	// stateMask.mask_stencilFunction();
	// stateMask.mask_stencilFunctionMask();
	// stateMask.mask_stencilFunctionReference();
	// stateMask.mask_stencilOpDepthFail();
	// stateMask.mask_stencilOpStencilAndDepthPass();
	// stateMask.mask_stencilOpStencilFail();
	// stateMask.mask_stencilTest();
	// stateMask.mask_stencilWrite();
}

void RenderPassDepthPrime::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
	pipeline.rtMainHDRMS->gl_drawBuffer(ctx.gl, libv::gl::Attachment::None);
	pipeline.rtStack.push(ctx.gl, pipeline.rtMainHDRMS);
	pipeline.rtMainHDRMS->gl_invalidateColorAll(ctx.gl);

	// Clear depth and stencil, Only invalidating and not clearing color attachments
	ctx.gl.clear(libv::gl::BufferBit::DepthStencil);

	gl_renderDepthNodes(ctx);

	pipeline.debug.gl_recordDepth("depth-prime", pipeline, ctx);

	// Resolve and bind the depth pre-pass result, it will be used by culling and depth related effects
	pipeline.rtStack.top()->gl_resolve(ctx.gl, libv::gl::BufferBit::Depth);
	auto depthTexture = pipeline.rtStack.top()->textureSSDepthStencil();
	depthTexture->gl_bind(ctx.gl, unitDepth);

	// Dispatch async depth read-back
	ctx.frame.depthReadbackBuffer.gl_fetch(ctx.gl, depthTexture);

	// Re-enable color attachment reads
	pipeline.rtStack.top()->gl_drawBuffersAll(ctx.gl);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
