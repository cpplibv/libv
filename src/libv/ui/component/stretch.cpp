// Project: libv.ui, File: src/libv/ui/component/stretch.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/stretch.hpp>
// pro
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/core_component.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreStretch : CoreComponent {
	friend class Stretch;
	[[nodiscard]] inline auto handler() { return Stretch{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> bg_color;
		PropertyL<Texture2D_view> bg_image;
		PropertyR<ShaderImage_view> bg_shader;
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
	CoreComponent::access_properties(setter);
}

libv::vec3f CoreStretch::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	const auto dynamic_size_image = property.bg_image()->size().cast<float>() + padding_size();

	return {dynamic_size_image, 0.f};
}

void CoreStretch::doRender(Renderer& r) {
	// y3   12--13--14--15
	//      | / | / | / |
	// y2   8---9---10--11
	//      | / | / | / |
	// y1   4---5---6---7
	//      | / | / | / |
	// y0   0---1---2---3
	//
	//      x0  x1  x2  x3

//	const auto borderPos = min(cast<float>(property.bg_image()->size()), layout_size2()) * 0.5f;
//	const auto borderTex = min(layout_size2() / max(cast<float>(property.bg_image()->size()), 1.0f) * 0.5f, 0.5f);
//
//	const auto p0 = libv::vec2f{0.0f, 0.0f};
//	const auto p1 = borderPos;
//	const auto p2 = layout_size2() - borderPos;
//	const auto p3 = layout_size2();
//
//	const auto t0 = libv::vec2f{0.0f, 0.0f};
//	const auto t1 = borderTex;
//	const auto t2 = 1.0f - borderTex;
//	const auto t3 = libv::vec2f{1.0f, 1.0f};
//
//	const auto color = property.bg_color();
//
//	r.begin_triangles(property.bg_image(), property.bg_shader());
//
//	r.vertex({p0.x, p0.y, 0}, {t0.x, t0.y}, color);
//	r.vertex({p1.x, p0.y, 0}, {t1.x, t0.y}, color);
//	r.vertex({p2.x, p0.y, 0}, {t2.x, t0.y}, color);
//	r.vertex({p3.x, p0.y, 0}, {t3.x, t0.y}, color);
//
//	r.vertex({p0.x, p1.y, 0}, {t0.x, t1.y}, color);
//	r.vertex({p1.x, p1.y, 0}, {t1.x, t1.y}, color);
//	r.vertex({p2.x, p1.y, 0}, {t2.x, t1.y}, color);
//	r.vertex({p3.x, p1.y, 0}, {t3.x, t1.y}, color);
//
//	r.vertex({p0.x, p2.y, 0}, {t0.x, t2.y}, color);
//	r.vertex({p1.x, p2.y, 0}, {t1.x, t2.y}, color);
//	r.vertex({p2.x, p2.y, 0}, {t2.x, t2.y}, color);
//	r.vertex({p3.x, p2.y, 0}, {t3.x, t2.y}, color);
//
//	r.vertex({p0.x, p3.y, 0}, {t0.x, t3.y}, color);
//	r.vertex({p1.x, p3.y, 0}, {t1.x, t3.y}, color);
//	r.vertex({p2.x, p3.y, 0}, {t2.x, t3.y}, color);
//	r.vertex({p3.x, p3.y, 0}, {t3.x, t3.y}, color);
//
//	r.index_strip({4, 0, 5, 1, 6, 2, 7, 3});
//	r.index_strip({3, 8}); // jump
//	r.index_strip({8, 4, 9, 5, 10, 6, 11, 7});
//	r.index_strip({7, 12}); // jump
//	r.index_strip({12, 8, 13, 9, 14, 10, 15, 11});
//
//	r.end();
}

// -------------------------------------------------------------------------------------------------

Stretch::Stretch(std::string name) :
	ComponentHandler<CoreStretch, EventHostGeneral<Stretch>>(std::move(name)) { }

Stretch::Stretch(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CoreStretch, EventHostGeneral<Stretch>>(gen, type) { }

Stretch::Stretch(core_ptr core) noexcept :
	ComponentHandler<CoreStretch, EventHostGeneral<Stretch>>(core) { }

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
