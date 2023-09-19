// Project: libv.re, File: src/libv/re/core/pass/render_passes.hpp

#pragma once

#include <libv/math/vec.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/core/pass/render_pass.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class RenderPassOutline : public RenderPass {
	RenderTargetTextures_ptr rtMask;
	Texture_ptr textureJFASteps;

	Material_ptr materialJFAInit;
	MaterialJFAStep_ptr materialJFAStep;
	MaterialJFAOutline_ptr materialJFAOutline;

	float outlineRadius = 6.0f;

public:
	explicit RenderPassOutline();

private:
	virtual void doUpdate(ContextUpdate& ctx) override;
	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
