// Project: libv.re, File: src/libv/re/material/material_skybox.cpp

#include <libv/re/material/material_skybox.hpp>

#include <libv/re/material/basic_material.hpp>
#include <libv/re/resource/shader_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MaterialSkybox* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialSkybox* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MaterialSkybox::MaterialSkybox(Texture_ptr sky) :
	Material(r.shader.load("shader/re/skybox.vs", "shader/re/skybox.fs"), nullptr) {

	layers = layerBackground;

	// state.depthFunction(libv::gl::TestFunction::LEqual); // NormalZ
	state.depthFunction(libv::gl::TestFunction::GEqual); // ReverseZ
	state.depthWrite(false);
	state.depthTest(true);
	state.blend(false);

	textureSky = std::move(sky);

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

void MaterialSkybox::gl_update(ContextUpdate& ctx) {
	MaterialAccessor_gl_update access{ctx};
	access_uniforms(access);
}

void MaterialSkybox::gl_bind(ContextRender& ctx) {
	MaterialAccessor_gl_bind access{ctx, shader, state};
	access_uniforms(access);
}

void MaterialSkybox::gl_bindDepth(ContextRender& ctx) {
	MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
	access_uniforms(access);
}

bool MaterialSkybox::equals(const Material& other) const noexcept {
	return static_cast<cptr::value_type&>(other) == *this;
}

Material_ptr MaterialSkybox::doClone() const {
	return ptr::make(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
