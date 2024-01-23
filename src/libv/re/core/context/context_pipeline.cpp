// Project: libv.re, File: src/libv/re/core/context/context_pipeline.cpp

#include <libv/re/core/context/context_pipeline.hpp>

// #include <libv/gl/gl.hpp>
// #include <libv/gl/texture.hpp>
// #include <libv/re/core/canvas.hpp>
// #include <libv/re/material/material_fullscreen.hpp>
// #include <libv/re/mesh/mesh_fullscreen.hpp>
// #include <libv/re/resource/texture.hpp>
// #include <libv/re/resource/texture_loader.hpp>
#include <libv/re/core/render_target.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

RenderTargetStack::RenderTargetStack() {
	stack.push(RenderTargetDefault_ptr::make());
}

RenderTarget_ptr& RenderTargetStack::top() {
	assert(!stack.empty());
	return stack.top();
}

RenderTarget_ptr& RenderTargetStack::prev() {
	assert(stack.size() > 1);
	return *((&stack.top()) - 1);
}

void RenderTargetStack::push(libv::GL& gl, RenderTarget_ptr rtptr) {
	auto& prev = *stack.top();
	push(gl, std::move(rtptr), prev.size(), prev.sampleCount(), prev.sampleFixed());
}

void RenderTargetStack::push(libv::GL& gl, RenderTarget_ptr rtptr, int sampleCount, bool sampleFixed) {
	auto& prev = *stack.top();
	push(gl, std::move(rtptr), prev.size(), sampleCount, sampleFixed);
}

void RenderTargetStack::push(libv::GL& gl, RenderTarget_ptr rtptr, libv::vec2i size, int sampleCount, bool sampleFixed) {
	stack.push(std::move(rtptr));
	stack.top()->gl_bindDraw(gl, size, sampleCount, sampleFixed);
}

void RenderTargetStack::pop(libv::GL& gl) {
	assert(stack.size() > 1); // The default rt must be kept in the stack
	stack.pop();
	auto& rt = *stack.top();
	rt.gl_bindDraw(gl, rt.size(), rt.sampleCount(), rt.sampleFixed());
}

// =================================================================================================

ContextPipeline::ContextPipeline(libv::GL& gl) {
	(void) gl;

	const bool hasStencil = true;

	Texture2DSpec rtMainTSpec{
		.wrapX = libv::gl::Wrap::MirroredRepeat,
		.wrapY = libv::gl::Wrap::MirroredRepeat,
		.magFilter = libv::gl::MagFilter::Linear,
		.minFilter = libv::gl::MinFilter::Linear
	};
	rtMainHDRMS = RenderTargetTextures_ptr::make();
	rtMainHDRMS->add(libv::gl::Attachment::Color0, libv::gl::FormatSized::RGBA16F, rtMainTSpec);
	if (hasStencil)
		rtMainHDRMS->add(libv::gl::Attachment::DepthStencil, libv::gl::FormatDepthStencil::DEPTH32F_STENCIL8, rtMainTSpec);
	else
		rtMainHDRMS->add(libv::gl::Attachment::Depth, libv::gl::FormatDepth::DEPTH_COMPONENT32F, rtMainTSpec);
}

ContextPipeline::~ContextPipeline() = default;

// -------------------------------------------------------------------------------------------------

// Texture_ptr ContextPipeline::sharedTexture(std::string_view name) {
// 	return nullptr;
// }
//
// SSBO_ptr ContextPipeline::sharedSSBO(std::string_view name) {
// 	return nullptr;
// }
// UBO_ptr ContextPipeline::sharedUBO(std::string_view name) {
// 	return nullptr;
// }
//
// RenderTarget_ptr ContextPipeline::sharedRenderTarget(std::string_view name) {
// 	return nullptr;
// }

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
