// File: stretch.cpp - Created on 2017.10.22. 11:40 - Author: Vader

// hpp
#include <libv/ui/component/stretch.hpp>
// libv
#include <libv/glr/mesh.hpp>
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreStretch : BaseComponent {
	friend class Stretch;
	[[nodiscard]] inline auto handler() { return Stretch{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> bg_color;
		PropertyL<Texture2D_view> bg_image;
		PropertyR<ShaderImage_view> bg_shader;
	} property;

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::TriangleStrip, libv::gl::BufferUsage::StaticDraw};

public:
	using BaseComponent::BaseComponent;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreStretch::access_properties(T& ctx) {
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

void CoreStretch::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreStretch> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	BaseComponent::access_properties(setter);
}

void CoreStretch::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	AccessLayout::lastDynamic(*this) = {libv::vec::cast<float>(property.bg_image()->size()), 0.f};
}

void CoreStretch::doRender(ContextRender& context) {
	if (context.changedSize) {
		mesh.clear();
		auto pos = mesh.attribute(attribute_position);
		auto tex = mesh.attribute(attribute_texture0);
		auto index = mesh.index();

		// y3   12--13--14--15
		//      | / | / | / |
		// y2   8---9---10--11
		//      | / | / | / |
		// y1   4---5---6---7
		//      | / | / | / |
		// y0   0---1---2---3
		//
		//      x0  x1  x2  x3

		const auto borderPos = min(cast<float>(property.bg_image()->size()), layout_size2()) * 0.5f;
		const auto borderTex = min(layout_size2() / max(cast<float>(property.bg_image()->size()), 1.0f) * 0.5f, 0.5f);

		const auto p0 = libv::vec2f{0.0f, 0.0f};
		const auto p1 = borderPos;
		const auto p2 = layout_size2() - borderPos;
		const auto p3 = layout_size2();

		const auto t0 = libv::vec2f{0.0f, 0.0f};
		const auto t1 = borderTex;
		const auto t2 = 1.0f - borderTex;
		const auto t3 = libv::vec2f{1.0f, 1.0f};

		pos(p0.x, p0.y, 0); tex(t0.x, t0.y);
		pos(p1.x, p0.y, 0); tex(t1.x, t0.y);
		pos(p2.x, p0.y, 0); tex(t2.x, t0.y);
		pos(p3.x, p0.y, 0); tex(t3.x, t0.y);

		pos(p0.x, p1.y, 0); tex(t0.x, t1.y);
		pos(p1.x, p1.y, 0); tex(t1.x, t1.y);
		pos(p2.x, p1.y, 0); tex(t2.x, t1.y);
		pos(p3.x, p1.y, 0); tex(t3.x, t1.y);

		pos(p0.x, p2.y, 0); tex(t0.x, t2.y);
		pos(p1.x, p2.y, 0); tex(t1.x, t2.y);
		pos(p2.x, p2.y, 0); tex(t2.x, t2.y);
		pos(p3.x, p2.y, 0); tex(t3.x, t2.y);

		pos(p0.x, p3.y, 0); tex(t0.x, t3.y);
		pos(p1.x, p3.y, 0); tex(t1.x, t3.y);
		pos(p2.x, p3.y, 0); tex(t2.x, t3.y);
		pos(p3.x, p3.y, 0); tex(t3.x, t3.y);

		index({4, 0, 5, 1, 6, 2, 7, 3}); index({3, 8}); // jump
		index({8, 4, 9, 5, 10, 6, 11, 7}); index({7, 12}); // jump
		index({12, 8, 13, 9, 14, 10, 15, 11});
	}

	const auto guard_m = context.gl.model.push_guard();
	context.gl.model.translate(layout_position());

	context.gl.program(*property.bg_shader());
	context.gl.uniform(property.bg_shader()->uniform_color, property.bg_color());
	context.gl.uniform(property.bg_shader()->uniform_MVPmat, context.gl.mvp());
	context.gl.texture(property.bg_image()->texture(), property.bg_shader()->textureChannel);
	context.gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

Stretch::Stretch(std::string name) :
	ComponenetHandler<CoreStretch, EventHostGeneral<Stretch>>(std::move(name)) { }

Stretch::Stretch(GenerateName_t gen, const std::string_view type) :
	ComponenetHandler<CoreStretch, EventHostGeneral<Stretch>>(gen, type) { }

Stretch::Stretch(base_ptr core) noexcept :
	ComponenetHandler<CoreStretch, EventHostGeneral<Stretch>>(core) { }

// -------------------------------------------------------------------------------------------------

void Stretch::color(Color value) {
	AccessProperty::manual(self(), self().property.bg_color, value);
}

const Color& Stretch::color() const noexcept {
	return self().property.bg_color();
}

void Stretch::image(Texture2D_view value) {
	AccessProperty::manual(self(), self().property.bg_image, std::move(value));
}

const Texture2D_view& Stretch::image() const noexcept {
	return self().property.bg_image();
}

void Stretch::shader(ShaderImage_view value) {
	AccessProperty::manual(self(), self().property.bg_shader, std::move(value));
}

const ShaderImage_view& Stretch::shader() const noexcept {
	return self().property.bg_shader();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
