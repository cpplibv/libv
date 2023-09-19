// Project: libv.re, File: src/libv/re/core/pass/render_passes.cpp

#include <libv/re/core/pass/render_passes.hpp>

#include <libv/re/core/context/context_pipeline.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

// RenderPassLightCull::RenderPassLightCull() :
// 	RenderPass("LightCull") {
// }
//
// void RenderPassLightCull::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
// 	shader->gl_bind(ctx.gl);
// 	ctx.gl.dispatchCompute(...);
// }

// -------------------------------------------------------------------------------------------------

// RenderPassShadow::RenderPassShadow() :
// 	RenderPass("Shadow") {
// 	layerMask = layerShadowCaster;
// 	sortingOrder = RenderSortingOrder::shaderDepthThenDistanceIncreasing;
// }
//
// void RenderPassShadow::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
// 	// ContextPipeline renderContext{gl, libv::re::RenderState{}};
// 	// renderContext.currentState.gl_apply(gl); // Apply default state
// 	//
// 	// auto rtGuard = pipeline.rtStack.pushGuard();
// 	//
// 	// gl_renderDepthNodes(ctx);
// }

// -------------------------------------------------------------------------------------------------

RenderPassOpaque::RenderPassOpaque() :
	RenderPass("Opaque") {
	layerMask = layerShadowCaster;
	sortingOrder = RenderSortingOrder::material;
}

void RenderPassOpaque::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
	gl_renderNodes(ctx);

	pipeline.debug.gl_recordColor("opaque", pipeline, ctx, libv::gl::Attachment::Color0);
	pipeline.debug.gl_recordStencil("opaque-stencil", pipeline, ctx);
}

// -------------------------------------------------------------------------------------------------

RenderPassBackground::RenderPassBackground() :
	RenderPass("Background") {
	layerMask = layerBackground;
	sortingOrder = RenderSortingOrder::none;
}

void RenderPassBackground::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
	gl_renderNodes(ctx);

	pipeline.debug.gl_recordColor("background", pipeline, ctx, libv::gl::Attachment::Color0);
}

// -------------------------------------------------------------------------------------------------

RenderPassTransparent::RenderPassTransparent() :
	RenderPass("Transparent") {
	layerMask = layerTransparent;
	sortingOrder = RenderSortingOrder::distanceDecreasing;
}

void RenderPassTransparent::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
	gl_renderNodes(ctx);

	pipeline.debug.gl_recordColor("transparent", pipeline, ctx, libv::gl::Attachment::Color0);
}

// -------------------------------------------------------------------------------------------------

RenderPassFlare::RenderPassFlare() :
	RenderPass("Flare") {
	layerMask = layerFlare;
	sortingOrder = RenderSortingOrder::material;
}

void RenderPassFlare::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
	gl_renderNodes(ctx);

	pipeline.debug.gl_recordColor("flare", pipeline, ctx, libv::gl::Attachment::Color0);
}

// -------------------------------------------------------------------------------------------------

RenderPassHUD::RenderPassHUD() :
	RenderPass("HUD") {
	layerMask = layerHUD;
	sortingOrder = RenderSortingOrder::none;
}

void RenderPassHUD::doPass(ContextPipeline& pipeline, ContextRender& ctx) {
	gl_renderNodes(ctx);

	pipeline.debug.gl_recordColor("hud", pipeline, ctx, libv::gl::Attachment::Color0);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
