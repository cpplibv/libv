// File: quad.cpp - Created on 2017.10.22. 11:40 - Author: Vader

// hpp
#include <libv/ui/component/quad.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/property_access.hpp>
#include <libv/ui/shader/shader_quad.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
void Quad::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.color; },
			Color(1, 1, 1, 1),
			pgr::appearance, pnm::color,
			"Background color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.quad_shader; },
			[](auto& u) { return u.shaderQuad(); },
			pgr::appearance, pnm::quad_shader,
			"Background shader"
	);
}

// -------------------------------------------------------------------------------------------------

Quad::Quad(BaseComponent& parent) :
	BaseComponent(parent, GenerateName, "quad") { }

Quad::Quad(BaseComponent& parent, std::string name) :
	BaseComponent(parent, std::move(name)) { }

Quad::Quad(BaseComponent& parent, GenerateName_t, const std::string_view type) :
	BaseComponent(parent, GenerateName, type) { }

Quad::~Quad() { }

// -------------------------------------------------------------------------------------------------

void Quad::doStyle(ContextStyle& ctx) {
	PropertySetterContext<Quad> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
}

void Quad::doRender(ContextRender& context) {
	if (context.changedSize) {
		mesh.clear();
		auto pos = mesh.attribute(attribute_position);
		auto index = mesh.index();

		// 3-2
		// |/|
		// 0-1
		pos(0, 0, 0);
		pos(size().x, 0, 0);
		pos(size().x, size().y, 0);
		pos(0, size().y, 0);

		index.quad(0, 1, 2, 3);
	}

	const auto guard_m = context.gl.model.push_guard();
	context.gl.model.translate(position());

	context.gl.program(*property.quad_shader());
	context.gl.uniform(property.quad_shader()->uniform_color, property.color());
	context.gl.uniform(property.quad_shader()->uniform_MVPmat, context.gl.mvp());
	context.gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
