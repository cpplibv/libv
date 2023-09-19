// Project: libv.re, File: src/libv/re/core/render_pass_context.hpp

#pragma once

#include <libv/re/core/context/context_frame_gl.hpp>
#include <libv/re/core/indirect_comand_buffer_mapped.hpp>
#include <libv/re/core/readback_buffer_mapped.hpp>
#include <libv/re/core/render_pass_debug_visualizer.hpp>
#include <libv/re/core/uniform_buffer_mapped.hpp>
#include <libv/re/fwd.hpp>
#include <libv/utility/guard.hpp>

#include <optional>
#include <vector>
#include <stack>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class RenderTargetStack {
	std::stack<RenderTarget_ptr, std::vector<RenderTarget_ptr>> stack;

public:
	RenderTargetStack();

	[[nodiscard]] constexpr inline std::size_t stackSize() const noexcept {
		return stack.size();
	}
	[[nodiscard]] RenderTarget_ptr& top();
	[[nodiscard]] RenderTarget_ptr& prev();

	void push(libv::GL& gl, RenderTarget_ptr rt);
	void push(libv::GL& gl, RenderTarget_ptr rt, int sampleCount, bool sampleFixed);
	void push(libv::GL& gl, RenderTarget_ptr rt, libv::vec2i size, int sampleCount, bool sampleFixed);
	auto pushGuard(libv::GL& gl, RenderTarget_ptr rt) {
		push(gl, std::move(rt));
		return libv::guard([this, &gl] {
			this->pop(gl);
		});
	}
	auto pushGuard(libv::GL& gl, RenderTarget_ptr rt, int sampleCount, bool sampleFixed) {
		push(gl, std::move(rt), sampleCount, sampleFixed);
		return libv::guard([this, &gl] {
			this->pop(gl);
		});
	}
	auto pushGuard(libv::GL& gl, RenderTarget_ptr rt, libv::vec2i size, int sampleCount, bool sampleFixed) {
		push(gl, std::move(rt), size, sampleCount, sampleFixed);
		return libv::guard([this, &gl] {
			this->pop(gl);
		});
	}
	void pop(libv::GL& gl);
};

// -------------------------------------------------------------------------------------------------

class ContextPipeline {
public:
	RenderTargetStack rtStack;
	RenderTargetTextures_ptr rtMainHDRMS; // Optionally HDR and optionally MS

	ReadbackBufferMapped depthReadbackBuffer{256};
	UniformBufferMapped uniformBuffer{16 * 1024};                 // 16 KB (x4)
	IndirectCommandBufferMapped indirectCommandBuffer{16 * 1024}; // 16 KB (x4)

	ContextFrameGL contextFrame{depthReadbackBuffer, uniformBuffer, indirectCommandBuffer};

	RenderPassDebugVisualizer debug;
	MeshFullscreen_ptr fullscreenMesh;

	/// named scoped runtime shared objects (texture, fbo, ssbo, ubo) between passes
	// libv::flat_map<std::string, Texture_ptr> sharedTextures;
	// libv::flat_map<std::string, SSBO_ptr> sharedSSBOs;
	// libv::flat_map<std::string, UBO_ptr> sharedUBOs;
	// libv::flat_map<std::string, RenderTarget> sharedRenderTargets;

public:
	explicit ContextPipeline(libv::GL& gl);
	~ContextPipeline();

public:
	// [[nodiscard]] Texture_ptr sharedTexture(std::string_view name);
	// [[nodiscard]] SSBO_ptr sharedSSBO(std::string_view name);
	// [[nodiscard]] UBO_ptr sharedUBO(std::string_view name);
	// [[nodiscard]] RenderTarget sharedRenderTarget(std::string_view name);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
