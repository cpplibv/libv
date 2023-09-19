// Project: libv.re, File: src/libv/re/material/material_test0.cpp

#include <libv/re/material/material_test0.hpp>

#include <libv/re/material/basic_material.hpp>
#include <libv/re/resource/material_scanner.hpp>
#include <libv/re/resource/shader_loader.hpp>
#include <libv/re/resource/texture_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MaterialTest0* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialTest0* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MaterialTest0::MaterialTest0() {
	shader = r.shader.load_async(
			libv::gl::ShaderType::Vertex, "shader/re/test/material_test0.vs",
			libv::gl::ShaderType::Fragment, "shader/re/test/material_test0.fs",
			VariantID::Default
	);
	// shaderDepth = re.shader.defaultDepth; // TODO P4: defaultDepth shader with explicit name *(it has multiple version, object, static, anim)
	shaderDepth = r.shader.load_async(
			libv::gl::ShaderType::Vertex, "shader/re/depth_only.vs",
			VariantID::Default
	);

	texture0 = r.texture.load("builtin:up");
	texture1 = texture0;
	texture1 = unitAmbient;
	texture2 = r.texture.load_async("texture/sky/sky_debug_x_front_uv_transparent_srgb.dds");
	auto x = r.texture.load_delayed_async("texture/sky/sky_debug_x_front_uv_transparent_srgb.dds", VariantID{2});
	x->swizzle("00rg");
	texture3 = *x;
	texture4 = r.texture.load_async("texture/stripes_border.png");
	texture5 = r.texture.load_async("texture/sky/sky_debug_x_front_uv_srgb.dds");
	textureSky = unitSky;
	textureSky = r.texture.load_async("texture/sky/sky_debug_x_front_uv_transparent_srgb.dds");

	// uniform_color = libv::vec4f{1, 0, 0, 1};
	block_custom->custom_block_color = libv::vec4f{0, 1, 0, 1};
	block_shared->shared_block_color = libv::vec4f{0, 0, 1, 1};

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

MaterialTest0::MaterialTest0(const MaterialScanner& scan) {
	shader = r.shader.load_async(
			libv::gl::ShaderType::Vertex, "shader/re/test/material_test0_model.vs",
			libv::gl::ShaderType::Fragment, "shader/re/test/material_test0.fs",
			VariantID::Default
	);
	shaderDepth = r.shader.load_async(
			libv::gl::ShaderType::Vertex, "shader/re/depth_only_model.vs",
			VariantID::Default
	);

	texture0 = unitBaseColor;
	texture0 = scan.texture_or(libv::vm4::TextureType::base_color, "builtin:white");
	texture1 = unitNormal;
	texture1 = scan.texture_or(libv::vm4::TextureType::normals, "builtin:up");
	texture2 = unitRMAO;
	texture2 = scan.texture_or(libv::vm4::TextureType::metalness, "builtin:up");
	texture3 = unitAmbient;
	texture3 = r.texture.load_async("builtin:white");
	texture4 = unitAmbient;
	texture4 = r.texture.load_async("builtin:white");
	texture5 = unitAmbient;
	texture5 = r.texture.load_async("builtin:white");
	textureSky = unitSky;
	textureSky = r.texture.load_async("texture/sky/sky_debug_x_front_uv_transparent_srgb.dds");

	mode = scan.int32_or("mode", 0);
	uniform_color = scan.vec4f_or("color_diffuse", {1, 1, 1, 1});
	block_custom->custom_block_color = scan.vec4f_or("color_ambient", {1, 1, 1, 1});
	block_shared->shared_block_color = scan.vec4f_or("color_specular", {0, 0, 0, 1});

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

void MaterialTest0::gl_update(ContextUpdate& ctx) {
	MaterialAccessor_gl_update access{ctx};
	access_uniforms(access);
}

void MaterialTest0::gl_bind(ContextRender& ctx) {
	MaterialAccessor_gl_bind access{ctx, shader, state};
	access_uniforms(access);
}

void MaterialTest0::gl_bindDepth(ContextRender& ctx) {
	MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
	access_uniforms(access);
}

bool MaterialTest0::equals(const Material& other) const noexcept {
	return static_cast<cptr::value_type&>(other) == *this;
}

Material_ptr MaterialTest0::doClone() const {
	return ptr::make(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
