// File: quad.cpp - Created on 2017.10.22. 11:40 - Author: Vader

// hpp
#include <libv/ui/component/quad.hpp>
// libv
#include <libv/glr/mesh.hpp>
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_quad.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreQuad : BaseComponent {
	friend class Quad;
	[[nodiscard]] inline auto handler() { return Quad{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> color;
		PropertyR<ShaderQuad_view> quad_shader;
	} property;

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

public:
	using BaseComponent::BaseComponent;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreQuad::access_properties(T& ctx) {
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

void CoreQuad::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreQuad> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	BaseComponent::access_properties(setter);
}

void CoreQuad::doRender(ContextRender& context) {
	if (context.changedSize) {
		mesh.clear();
		auto pos = mesh.attribute(attribute_position);
		auto index = mesh.index();

		pos(0, 0, 0);
		pos(layout_size().x, 0, 0);
		pos(layout_size().x, layout_size().y, 0);
		pos(0, layout_size().y, 0);

		index.quad(0, 1, 2, 3);
	}

	const auto guard_m = context.gl.model.push_guard();
	context.gl.model.translate(layout_position());

	context.gl.program(*property.quad_shader());
	context.gl.uniform(property.quad_shader()->uniform_color, property.color());
	context.gl.uniform(property.quad_shader()->uniform_MVPmat, context.gl.mvp());
	context.gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

Quad::Quad(std::string name) :
	ComponenetHandler<CoreQuad, EventHostGeneral<Quad>>(std::move(name)) { }

Quad::Quad(GenerateName_t gen, const std::string_view type) :
	ComponenetHandler<CoreQuad, EventHostGeneral<Quad>>(gen, type) { }

Quad::Quad(base_ptr core) noexcept :
	ComponenetHandler<CoreQuad, EventHostGeneral<Quad>>(core) { }

// -------------------------------------------------------------------------------------------------

void Quad::color(Color value) {
	AccessProperty::manual(self(), self().property.color, value);
}

const Color& Quad::color() const noexcept {
	return self().property.color();
}

void Quad::shader(ShaderQuad_view value) {
	AccessProperty::manual(self(), self().property.quad_shader, std::move(value));
}

const ShaderQuad_view& Quad::shader() const noexcept {
	return self().property.quad_shader();
}


// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
