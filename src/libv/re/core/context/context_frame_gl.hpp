// Project: libv.re, File: src/libv/re/core/render_context.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/core/context/context_frame.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct ContextFrameGL : ContextFrame {
	ReadbackBufferMapped& depthReadbackBuffer;
	UniformBufferMapped& uniformBuffer;
	IndirectCommandBufferMapped& indirectCommandBuffer;

	ContextFrameGL(ReadbackBufferMapped& depthReadbackBuffer, UniformBufferMapped& uniformBuffer, IndirectCommandBufferMapped& indirectCommandBuffer) :
			depthReadbackBuffer(depthReadbackBuffer),
			uniformBuffer(uniformBuffer),
			indirectCommandBuffer(indirectCommandBuffer) {}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
