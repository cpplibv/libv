// Project: libv.re, File: src/libv/re/core/context/context_render.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/math/mat_fwd.hpp>
#include <libv/math/vec_fwd.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/core/context/context_frame_gl.hpp> // For convenience

#include <span>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct ContextUpdate {
	libv::GL& gl;
	ContextFrameGL& frame;

public:
	inline ContextUpdate(libv::GL& gl, ContextFrameGL& frame) :
		gl(gl),
		frame(frame) {}
};

// -------------------------------------------------------------------------------------------------

struct ContextRender {
	libv::GL& gl;
	ContextFrameGL& frame;

public:
	inline ContextRender(libv::GL& gl, ContextFrameGL& frame) :
		gl(gl),
		frame(frame) {}

public:
	[[nodiscard]] std::span<const libv::vec4f> gl_depthQueryResult(libv::GL& gl, const ReadbackQueryIndex& index, uint32_t count);

	[[nodiscard]] float linearizeDepth(float depth) const {
		return linearize_ReverseZinf_depth_01(depth);
	}
	[[nodiscard]] float linearize_ReverseZinf_depth_01(float depth) const {
		return frame.near / depth;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
