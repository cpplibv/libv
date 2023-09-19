// Project: libv.re, File: src/libv/re/material/internal/material_pass_debug.cpp

#include <libv/re/material/internal/material_pass_debug.hpp>

#include <fmt/format.h>

#include <libv/re/material/basic_material.hpp>
#include <libv/re/resource/shader_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MaterialDebugPassCapture* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialDebugPassCapture* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MaterialDebugPassCapture::MaterialDebugPassCapture() :
	Material([&] {
		auto s = r.shader.load_delayed("shader/re/post_process/fullscreen.vs", "shader/re/debug/debug_pass_capture.fs");
		if (s.isFirstLoad()) {
			s->define(fmt::format("DEBUG_MODE_NORMAL {}", +DebugCaptureMode::normal));
			s->define(fmt::format("DEBUG_MODE_DEPTH {}", +DebugCaptureMode::depth));
			s->define(fmt::format("DEBUG_MODE_DEPTH_LINEAR {}", +DebugCaptureMode::depth_linear));
			s->define(fmt::format("DEBUG_MODE_DEPTH_DISTANCE {}", +DebugCaptureMode::depth_distance));
			s->define(fmt::format("DEBUG_MODE_STENCIL {}", +DebugCaptureMode::stencil));
			s->define(fmt::format("DEBUG_MODE_JFA {}", +DebugCaptureMode::jfa));
			s->define(fmt::format("DEBUG_MODE_MISSING {}", +DebugCaptureMode::missing));
		}
		return s.resource;
	}(), nullptr) {

	state.depthWrite(false);
	state.depthTest(false);
	state.blend(false);

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

void MaterialDebugPassCapture::gl_update(ContextUpdate& ctx) {
	MaterialAccessor_gl_update access{ctx};
	access_uniforms(access);
}

void MaterialDebugPassCapture::gl_bind(ContextRender& ctx) {
	MaterialAccessor_gl_bind access{ctx, shader, state};
	access_uniforms(access);
}

void MaterialDebugPassCapture::gl_bindDepth(ContextRender& ctx) {
	MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
	access_uniforms(access);
}

bool MaterialDebugPassCapture::equals(const Material& other) const noexcept {
	return static_cast<cptr::value_type&>(other) == *this;
}

Material_ptr MaterialDebugPassCapture::doClone() const {
	return ptr::make(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
