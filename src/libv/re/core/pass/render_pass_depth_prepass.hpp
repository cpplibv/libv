// Project: libv.re, File: src/libv/re/node/_post_process.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/core/pass/render_pass.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class RenderPassDepthPrime : public RenderPass {
public:
	explicit RenderPassDepthPrime();

private:
	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
