// Project: libv.re, File: src/libv/re/core/render_pass_debug_visualizer.hpp

#pragma once

#include <libv/gl/enum.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/common/debug_capture_mode.hpp>

#include <memory>
#include <string>
#include <string_view>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class RenderPassDebugVisualizerState;

class RenderPassDebugVisualizer {
private:
	std::string activeName;
	int32_t index;
	DebugCaptureMode captureMode = DebugCaptureMode::normal;

	std::unique_ptr<RenderPassDebugVisualizerState> state;

public:
	RenderPassDebugVisualizer();
	~RenderPassDebugVisualizer();

public:
	[[nodiscard]] bool isActive() const {
		return !activeName.empty();
	}
	void show(std::string_view name, int index, DebugCaptureMode captureMode) {
		this->activeName = std::string(name);
		this->index = index;
		this->captureMode = captureMode;
	}
	void hide();

public:
	void gl_recordDepth(std::string_view name, ContextPipeline& pipeline, ContextRender& ctx);
	void gl_recordStencil(std::string_view name, ContextPipeline& pipeline, ContextRender& ctx);
	void gl_recordColor(std::string_view name, int index, ContextPipeline& pipeline, ContextRender& ctx, libv::gl::Attachment attachment);
	void gl_recordColor(std::string_view name, ContextPipeline& pipeline, ContextRender& ctx, libv::gl::Attachment attachment);
	void gl_recordTextureLayer(std::string_view name, int index, ContextPipeline& pipeline, ContextRender& ctx, const Texture_ptr& texture, int32_t level, int32_t layer);

	void gl_frameStart(ContextPipeline& pipeline, ContextRender& ctx);
	void gl_frameEnd(ContextPipeline& pipeline, ContextRender& ctx);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
