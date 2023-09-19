// Project: libv.re, File: src/libv/re/material/material_solid.cpp

#include <libv/re/material/material_solid.hpp>

#include <libv/re/material/basic_material.hpp>
#include <libv/re/resource/material_scanner.hpp>
#include <libv/re/resource/shader_loader.hpp>
#include <libv/re/resource/texture_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MaterialSolid* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialSolid* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MaterialSolid::MaterialSolid(libv::vec4f color) {
	shader = r.shader.load_async(
			libv::gl::ShaderType::Vertex, "shader/re/solid.vs",
			libv::gl::ShaderType::Fragment, "shader/re/solid.fs",
			VariantID::Default
	);
	shaderDepth = r.shader.load_async(
			libv::gl::ShaderType::Vertex, "shader/re/depth_only.vs",
			VariantID::Default
	);

	uniform_color = color;

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

MaterialSolid::MaterialSolid(const MaterialScanner& scan) {
	shader = r.shader.load_async(
			libv::gl::ShaderType::Vertex, "shader/re/test/solid.vs",
			libv::gl::ShaderType::Fragment, "shader/re/test/solid.fs",
			VariantID::Default
	);
	shaderDepth = r.shader.load_async(
			libv::gl::ShaderType::Vertex, "shader/re/depth_only.vs",
			VariantID::Default
	);

	uniform_color = scan.vec4f_or("color_diffuse", {1, 0, 0, 1});

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

void MaterialSolid::gl_update(ContextUpdate& ctx) {
	MaterialAccessor_gl_update access{ctx};
	access_uniforms(access);
}

void MaterialSolid::gl_bind(ContextRender& ctx) {
	MaterialAccessor_gl_bind access{ctx, shader, state};
	access_uniforms(access);
}

void MaterialSolid::gl_bindDepth(ContextRender& ctx) {
	MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
	access_uniforms(access);
}

bool MaterialSolid::equals(const Material& other) const noexcept {
	return static_cast<cptr::value_type&>(other) == *this;
}

Material_ptr MaterialSolid::doClone() const {
	return ptr::make(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
