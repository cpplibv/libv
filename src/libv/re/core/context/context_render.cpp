// Project: libv.re, File: src/libv/re/core/context/context_render.cpp

#include <libv/re/core/context/context_render.hpp>

#include <libv/re/core/readback_buffer_mapped.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

std::span<const libv::vec4f> ContextRender::gl_depthQueryResult(libv::GL& gl, const ReadbackQueryIndex& index, uint32_t count) {
	return frame.depthReadbackBuffer.gl_result(gl, index, count);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
