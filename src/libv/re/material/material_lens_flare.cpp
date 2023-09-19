// Project: libv.re, File: src/libv/re/material/material_lens_flare.cpp

#include <libv/re/material/material_lens_flare.hpp>

#include <libv/re/material/basic_material.hpp>
#include <libv/re/resource/shader_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MaterialLensFlare* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialLensFlare* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MaterialLensFlare::MaterialLensFlare(Texture_ptr&& texture_, UniformBlock_ptr<UniformBlockLensFlare>&& lensFlareInfo_) :
	texture(unitDiffuse, std::move(texture_)),
	lensFlareInfo(std::move(lensFlareInfo_)) {

	shader = r.shader.load_async(
			libv::gl::ShaderType::Vertex, "shader/re/lens_flare.vs",
			libv::gl::ShaderType::Fragment, "shader/re/lens_flare.fs",
			VariantID::Default
	);
	shaderDepth = nullptr;

	layers = layerFlare;

	// Additive
	state.blend(true);
	state.blendSrc(libv::gl::BlendFunction::One);
	state.blendDst(libv::gl::BlendFunction::One);
	state.depthWrite(false);
	state.depthTest(false);

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

void MaterialLensFlare::gl_update(ContextUpdate& ctx) {
	MaterialAccessor_gl_update access{ctx};
	access_uniforms(access);
}

void MaterialLensFlare::gl_bind(ContextRender& ctx) {
	MaterialAccessor_gl_bind access{ctx, shader, state};
	access_uniforms(access);
}

void MaterialLensFlare::gl_bindDepth(ContextRender& ctx) {
	MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
	access_uniforms(access);
}

bool MaterialLensFlare::equals(const Material& other) const noexcept {
	return static_cast<cptr::value_type&>(other) == *this;
}

Material_ptr MaterialLensFlare::doClone() const {
	return ptr::make(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
