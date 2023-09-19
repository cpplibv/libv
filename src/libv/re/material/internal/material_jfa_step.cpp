// Project: libv.re, File: src/libv/re/material/internal/material_jfa_step.cpp

#include <libv/re/material/internal/material_jfa_step.hpp>

#include <libv/re/material/basic_material.hpp>
#include <libv/re/resource/shader_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MaterialJFAStep* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialJFAStep* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MaterialJFAStep::MaterialJFAStep() :
	Material(r.shader.load(libv::gl::ShaderType::Compute, "shader/re/outline/jump_flood_step.cs"), nullptr) {

	state.depthTest(false);
	state.depthWrite(false);
	state.blend(false);

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

void MaterialJFAStep::gl_update(ContextUpdate& ctx) {
	MaterialAccessor_gl_update access{ctx};
	access_uniforms(access);
}

void MaterialJFAStep::gl_bind(ContextRender& ctx) {
	MaterialAccessor_gl_bind access{ctx, shader, state};
	access_uniforms(access);
}

void MaterialJFAStep::gl_bindDepth(ContextRender& ctx) {
	MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
	access_uniforms(access);
}

bool MaterialJFAStep::equals(const Material& other) const noexcept {
	return static_cast<cptr::value_type&>(other) == *this;
}

Material_ptr MaterialJFAStep::doClone() const {
	return ptr::make(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
