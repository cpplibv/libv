// Project: libv.ui, File: src/libv/ui/component/quad.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/quad.hpp>
// pro
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/core_component.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_quad.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreQuad : CoreComponent {
	friend class Quad;
	[[nodiscard]] inline auto handler() { return Quad{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> color;
		PropertyR<ShaderQuad_view> quad_shader;
	} property;

public:
	using CoreComponent::CoreComponent;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doRender(Renderer& r) override;
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
	CoreComponent::access_properties(setter);
}

libv::vec3f CoreQuad::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	return {padding_size(), 0.f};
}

void CoreQuad::doRender(Renderer& r) {
	r.quad(padding_LB(), layout_size2() - padding_size(),
			property.color(),
			property.quad_shader());
}

// -------------------------------------------------------------------------------------------------

Quad::Quad(std::string name) :
	ComponentHandler<CoreQuad, EventHostGeneral<Quad>>(std::move(name)) { }

Quad::Quad(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CoreQuad, EventHostGeneral<Quad>>(gen, type) { }

Quad::Quad(core_ptr core) noexcept :
	ComponentHandler<CoreQuad, EventHostGeneral<Quad>>(core) { }

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
