// Project: libv.re, File: src/libv/re/core/pass/render_passes.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/core/pass/render_pass.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------
// TODO P1: Split this header into multiple files

// Light cull is nodeless
// class RenderPassLightCull : public RenderPass {
// 	Shader_ptr shader = r.shader.load(libv::gl::ShaderType::Compute, "shader/re/light_cull.cs");
//
// public:
//	explicit RenderPassLightCull();
//
// private:
// 	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
// };

// class RenderPassShadow : public RenderPass {
// public:
//	explicit RenderPassShadow();
// private:
// 	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
// };

class RenderPassOpaque : public RenderPass {
public:
	explicit RenderPassOpaque();

private:
	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
};

class RenderPassBackground : public RenderPass {
public:
	explicit RenderPassBackground();

private:
	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
};

class RenderPassTransparent : public RenderPass {
public:
	explicit RenderPassTransparent();

private:
	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
};

class RenderPassFlare : public RenderPass {
public:
	explicit RenderPassFlare();

private:
	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
};

class RenderPassHUD : public RenderPass {
public:
	explicit RenderPassHUD();

private:
	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
