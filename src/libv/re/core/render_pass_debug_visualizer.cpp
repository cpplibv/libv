// Project: libv.re, File: src/libv/re/core/render_pass_context.cpp

#include <libv/re/core/render_pass_debug_visualizer.hpp>

#include <libv/gl/gl.hpp>
#include <libv/re/common/texture_unit.hpp>
#include <libv/re/core/context/context_pipeline.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/re/core/render_target.hpp>
#include <libv/re/material/internal/material_pass_debug.hpp>
#include <libv/re/material/material_fullscreen.hpp>
#include <libv/re/mesh/mesh_fullscreen.hpp>
#include <libv/re/resource/texture.hpp>
#include <libv/re/resource/texture_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct RenderPassDebugVisualizerState {
	RenderTargetTextures_ptr defaultRT = RenderTargetTextures_ptr::make();
	RenderTargetTextures_ptr captureRT = RenderTargetTextures_ptr::make();
	Texture_ptr textureError;
	MaterialDebugPassCapture_ptr materialCapture;
	MaterialFullscreen_ptr materialView;

	bool found = false;
};

// -------------------------------------------------------------------------------------------------

RenderPassDebugVisualizer::RenderPassDebugVisualizer() {
}

RenderPassDebugVisualizer::~RenderPassDebugVisualizer() {
	// For the sake of forward declared types
}

void RenderPassDebugVisualizer::hide() {
	this->activeName = "";
	state.reset();
}

void RenderPassDebugVisualizer::gl_recordDepth(std::string_view name, ContextPipeline& pipeline, ContextRender& ctx) {
	if (name != activeName)
		return;
	auto debugGroup_guard = ctx.gl.pushDebugGroup_guard("RenderPassDebug");

	RenderTarget& rt = *pipeline.rtStack.top();
	rt.gl_resolve(ctx.gl, libv::gl::BufferBit::Depth);
	auto texture = rt.textureSSDepthStencil();
	if (texture == nullptr)
		return;

	state->found = true;
	texture->gl_bind(ctx.gl, unitDiffuse);

	const auto rtGuard = pipeline.rtStack.pushGuard(ctx.gl, state->captureRT, 0, false);

	state->materialCapture->captureMode = captureMode;
	state->materialCapture->levelIndex = -1;
	state->materialCapture->layerIndex = -1;
	state->materialCapture->gl_bind(ctx);
	pipeline.fullscreenMesh->gl_render(ctx);
}

void RenderPassDebugVisualizer::gl_recordStencil(std::string_view name, ContextPipeline& pipeline, ContextRender& ctx) {
	if (name != activeName)
		return;
	auto debugGroup_guard = ctx.gl.pushDebugGroup_guard("RenderPassDebug");

	RenderTarget& rt = *pipeline.rtStack.top();
	rt.gl_resolve(ctx.gl, libv::gl::BufferBit::Stencil);
	auto texture = rt.textureSSDepthStencil();
	if (texture == nullptr)
		return;

	state->found = true;

	texture->set(libv::gl::DepthStencilMode::Stencil);
	texture->gl_bind(ctx.gl, unitStencil);

	const auto rtGuard = pipeline.rtStack.pushGuard(ctx.gl, state->captureRT, 0, false);

	state->materialCapture->captureMode = captureMode;
	state->materialCapture->levelIndex = -1;
	state->materialCapture->layerIndex = -1;
	state->materialCapture->gl_bind(ctx);
	pipeline.fullscreenMesh->gl_render(ctx);

	texture->set(libv::gl::DepthStencilMode::Depth);
}

void RenderPassDebugVisualizer::gl_recordColor(std::string_view name, int index, ContextPipeline& pipeline, ContextRender& ctx, libv::gl::Attachment attachment) {
	if (name != activeName || index != this->index)
		return;
	auto debugGroup_guard = ctx.gl.pushDebugGroup_guard("RenderPassDebug");

	RenderTarget& rt = *pipeline.rtStack.top();
	rt.gl_resolve(ctx.gl, libv::gl::BufferBit::Color);
	auto texture = rt.textureSSColor(attachment);
	if (texture == nullptr)
		return;

	state->found = true;
	texture->gl_bind(ctx.gl, unitDiffuse);

	const auto rtGuard = pipeline.rtStack.pushGuard(ctx.gl, state->captureRT, 0, false);

	state->materialCapture->captureMode = captureMode;
	state->materialCapture->levelIndex = -1;
	state->materialCapture->layerIndex = -1;
	state->materialCapture->gl_bind(ctx);
	pipeline.fullscreenMesh->gl_render(ctx);
}

void RenderPassDebugVisualizer::gl_recordColor(std::string_view name, ContextPipeline& pipeline, ContextRender& ctx, libv::gl::Attachment attachment) {
	gl_recordColor(name, 0, pipeline, ctx, attachment);
}

void RenderPassDebugVisualizer::gl_recordTextureLayer(std::string_view name, int index, ContextPipeline& pipeline, ContextRender& ctx, const Texture_ptr& texture, int32_t level, int32_t layer) {
	if (name != activeName || index != this->index)
		return;
	if (texture == nullptr)
		return;

	auto debugGroup_guard = ctx.gl.pushDebugGroup_guard("RenderPassDebug");

	state->found = true;

	if (layer == -1)
		texture->gl_bind(ctx.gl, unitDiffuse);
	else
		texture->gl_bind(ctx.gl, unitSpecular);

	const auto textureSize = texture->size();
	const auto mipSize = libv::vec2i{std::max(textureSize.x >> level, 1), std::max(textureSize.y >> level, 1)};
	const auto rtGuard = pipeline.rtStack.pushGuard(ctx.gl, state->captureRT, mipSize, 0, false);

	state->materialCapture->captureMode = captureMode;
	state->materialCapture->levelIndex = level;
	state->materialCapture->layerIndex = layer;
	state->materialCapture->gl_bind(ctx);
	pipeline.fullscreenMesh->gl_render(ctx);
}

void RenderPassDebugVisualizer::gl_frameStart(ContextPipeline& pipeline, ContextRender& ctx) {
	if (!isActive())
		return;

	if (!state) {
		state = std::make_unique<RenderPassDebugVisualizerState>();

		const auto spec = Texture2DSpec{
			libv::gl::Wrap::ClampToEdge,
			libv::gl::Wrap::ClampToEdge,
			libv::gl::MagFilter::Nearest,
			libv::gl::MinFilter::Nearest,
		};
		state->captureRT->add(libv::gl::Attachment::Color0, libv::gl::FormatSized::RGBA8, spec);
		state->captureRT->gl_drawBuffersAll(ctx.gl);
		state->defaultRT->add(libv::gl::Attachment::Color0, libv::gl::FormatSized::RGBA8, spec);

		// TODO P4: Material creation could miss the beginning of the frame
		//			Currently this is solved by careful ordering in scene.render, easy to use incorrectly
		//			This needs a universal solution
		state->materialCapture = MaterialDebugPassCapture::create();
		state->materialView = MaterialFullscreen::create("shader/re/debug/debug_view_texture.fs");
		state->textureError = r.texture.load("builtin:magenta", VariantID::Default, LoadingMode::blocking);
	}

	state->found = false;

	// Inject a new default render target to enable simulation of texture reads from default render target
	pipeline.rtStack.push(ctx.gl, state->defaultRT);
}

void RenderPassDebugVisualizer::gl_frameEnd(ContextPipeline& pipeline, ContextRender& ctx) {
	if (!isActive())
		return;

	if (state->found) {
		state->captureRT->gl_resolve(ctx.gl, libv::gl::BufferBit::Color);
		state->captureRT->textureSSColor(libv::gl::Attachment::Color0)->gl_bind(ctx.gl, unitDiffuse);
	} else {
		state->textureError->gl_bind(ctx.gl, unitDiffuse);
	}

	// Remove the injected new default render target
	pipeline.rtStack.pop(ctx.gl);

	// Render to the real default render target
	state->materialView->gl_bind(ctx);
	pipeline.fullscreenMesh->gl_render(ctx);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
