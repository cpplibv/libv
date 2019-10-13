// File: image.cpp - Created on 2017.10.22. 11:40 - Author: Vader

// hpp
#include <libv/ui/component/image.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/property_access.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
void Image::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.bg_color; },
			Color(1, 1, 1, 1),
			pgr::appearance, pnm::bg_color,
			"Background color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.bg_image; },
			[](auto& u) { return u.fallbackTexture2D(); },
			pgr::appearance, pnm::bg_image,
			"Background image"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.bg_shader; },
			[](auto& u) { return u.shaderImage(); },
			pgr::appearance, pnm::bg_shader,
			"Background shader"
	);
}

// -------------------------------------------------------------------------------------------------

Image::Image(BaseComponent& parent) :
	BaseComponent(parent, GenerateName, "image") { }

Image::Image(BaseComponent& parent, std::string name) :
	BaseComponent(parent, std::move(name)) { }

Image::Image(BaseComponent& parent, GenerateName_t, const std::string_view type) :
	BaseComponent(parent, GenerateName, type) { }

Image::~Image() { }

// -------------------------------------------------------------------------------------------------

void Image::doStyle(ContextStyle& ctx) {
	PropertySetterContext<Image> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
}

void Image::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	AccessLayout::lastDynamic(*this) = {libv::vec::cast<float>(property.bg_image()->size()), 0.f};
}

void Image::doRender(ContextRender& context) {
	if (context.changedSize) {
		mesh.clear();
		auto pos = mesh.attribute(attribute_position);
		auto tex = mesh.attribute(attribute_texture0);
		auto index = mesh.index();

		pos(0, 0, 0);
		pos(size().x, 0, 0);
		pos(size().x, size().y, 0);
		pos(0, size().y, 0);

		tex(0, 0);
		tex(1, 0);
		tex(1, 1);
		tex(0, 1);

		index.quad(0, 1, 2, 3);
	}

	const auto guard_m = context.gl.model.push_guard();
	context.gl.model.translate(position());

	context.gl.program(*property.bg_shader());
	context.gl.uniform(property.bg_shader()->uniform_color, property.bg_color());
	context.gl.uniform(property.bg_shader()->uniform_MVPmat, context.gl.mvp());
	context.gl.texture(property.bg_image()->texture(), property.bg_shader()->textureChannel);
	context.gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
