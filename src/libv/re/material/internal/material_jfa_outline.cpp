// Project: libv.re, File: src/libv/re/material/internal/material_jfa_outline.cpp

#include <libv/re/material/internal/material_jfa_outline.hpp>

#include <libv/re/material/basic_material.hpp>
#include <libv/re/resource/shader_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MaterialJFAOutline* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialJFAOutline* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MaterialJFAOutline::MaterialJFAOutline(float radius) :
	MaterialFullscreen("shader/re/outline/jump_flood_outline.fs") {
	block->outlineRadius = radius;

	state.depthWrite(false);
	state.depthTest(false);
	state.blend(true);
	state.blendDst(libv::gl::BlendFunction::One_Minus_SourceAlpha);
	state.blendSrc(libv::gl::BlendFunction::SourceAlpha);

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

void MaterialJFAOutline::gl_update(ContextUpdate& ctx) {
	MaterialAccessor_gl_update access{ctx};
	access_uniforms(access);
}

void MaterialJFAOutline::gl_bind(ContextRender& ctx) {
	MaterialAccessor_gl_bind access{ctx, shader, state};
	access_uniforms(access);
}

void MaterialJFAOutline::gl_bindDepth(ContextRender& ctx) {
	MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
	access_uniforms(access);
}

bool MaterialJFAOutline::equals(const Material& other) const noexcept {
	return static_cast<cptr::value_type&>(other) == *this;
}

Material_ptr MaterialJFAOutline::doClone() const {
	return ptr::make(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
