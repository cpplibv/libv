// Project: libv.re, File: src/libv/re/material/internal/material_outline_mask.cpp

#include <libv/re/material/internal/material_outline_mask.hpp>

#include <libv/re/common/node_type.hpp>
#include <libv/re/material/basic_material.hpp>
#include <libv/re/resource/shader_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MaterialOutlineMask* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MaterialOutlineMask* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MaterialOutlineMask::MaterialOutlineMask(NodeType nodeType, libv::vec4f color) :
	Material([](auto type) -> Shader_ptr {
		switch (type) {
		case NodeType::light:
			assert(false);
			return nullptr;
		case NodeType::object:
			return r.shader.load_async("shader/re/outline/mask.vs", "shader/re/outline/mask.fs");
		case NodeType::model:
			return r.shader.load_async("shader/re/outline/mask_model.vs", "shader/re/outline/mask.fs");
		case NodeType::proxy:
			assert(false);
			return nullptr;
		}
		assert(false);
		return nullptr;
	}(nodeType), nullptr),
	color(color) {

	layers = layerOutlineCaster;

	state.depthWrite(false);
	state.depthTest(false);
	state.blend(false);

	MaterialAccessor_assignShader access{shader, shaderDepth};
	access_uniforms(access);
}

void MaterialOutlineMask::gl_update(ContextUpdate& ctx) {
	MaterialAccessor_gl_update access{ctx};
	access_uniforms(access);
}

void MaterialOutlineMask::gl_bind(ContextRender& ctx) {
	MaterialAccessor_gl_bind access{ctx, shader, state};
	access_uniforms(access);
}

void MaterialOutlineMask::gl_bindDepth(ContextRender& ctx) {
	MaterialAccessor_gl_bindDepth access{ctx, shaderDepth, state};
	access_uniforms(access);
}

bool MaterialOutlineMask::equals(const Material& other) const noexcept {
	return static_cast<cptr::value_type&>(other) == *this;
}

Material_ptr MaterialOutlineMask::doClone() const {
	return ptr::make(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
