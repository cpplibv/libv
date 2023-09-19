// Project: libv.re, File: src/libv/re/material/material.cpp

#include <libv/re/material/material.hpp>

#include <libv/re/core/context/context_render.hpp>
#include <libv/re/material/basic_material.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Material* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Material* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

Material::Material(Shader_ptr shader, Shader_ptr shaderDepth) :
	shader(std::move(shader)),
	shaderDepth(std::move(shaderDepth)) {
}

Material::~Material() {
	// For the sake of forward declared types
}

void Material::gl_update(ContextUpdate& ctx) {
	MaterialAccessor_gl_update access{ctx};
	access_uniforms(access);
}

void Material::gl_bind(ContextRender& ctx) {
	MaterialAccessor_gl_bind access{ctx, shader, state};
	access_uniforms(access);
}

void Material::gl_bindDepth(ContextRender& ctx) {
	MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
	access_uniforms(access);
}

bool Material::equals(const Material& other) const noexcept {
	return static_cast<cptr::value_type&>(other) == *this;
}

Material_ptr Material::doClone() const {
	return ptr::make(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
