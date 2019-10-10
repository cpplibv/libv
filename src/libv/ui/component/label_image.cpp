// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/label_image.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_access.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
void LabelImage::access_properties(T& ctx) {
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
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.align_horizontal; },
			[](auto& c, auto v) { c.align_horizontal(v, PropertyDriver::style); },
			[](const auto& c) { return c.align_horizontal(); },
			AlignHorizontal::Left,
			pgr::appearance, pnm::align_horizontal,
			"Horizontal alignment of the text"
	);
//	ctx.indirect(
//			[](auto& c) -> auto& { return c.property.align_vertical; },
//			[](auto& c, auto v) { c.align_vertical(v, PropertyDriver::style); },
//			[](const auto& c) { return c.align_vertical(); },
//			AlignVertical::Top,
//			pgr::appearance, pnm::align_vertical,
//			"Vertical alignment of the text"
//	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_color; },
			Color(1, 1, 1, 1),
			pgr::appearance, pnm::font_color,
			"Font color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_shader; },
			[](auto& u) { return u.shaderFont(); },
			pgr::appearance, pnm::font_shader,
			"Font shader"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.font; },
			[](auto& c, auto v) { c.font(std::move(v), PropertyDriver::style); },
			[](const auto& c) { return c.font(); },
			[](auto& u) { return u.fallbackFont(); },
			pgr::font, pnm::font,
			"Font file"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.font_size; },
			[](auto& c, auto v) { c.font_size(v, PropertyDriver::style); },
			[](const auto& c) { return c.font_size(); },
			FontSize{12},
			pgr::font, pnm::font_size,
			"Font size in pixel"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.text(std::move(v)); },
			[](const auto& c) { return c.text(); },
			pgr::behaviour, pnm::text,
			"Displayed text"
	);
}

// -------------------------------------------------------------------------------------------------

LabelImage::LabelImage() :
	BaseComponent(UnnamedTag, "label-image") { }

LabelImage::LabelImage(std::string name) :
	BaseComponent(std::move(name)) { }

LabelImage::LabelImage(UnnamedTag_t, const std::string_view type) :
	BaseComponent(UnnamedTag, type) { }

LabelImage::~LabelImage() { }

// -------------------------------------------------------------------------------------------------

void LabelImage::align_horizontal(AlignHorizontal value, PropertyDriver driver) {
	if (AccessProperty::setter(*this, property.align_horizontal, driver))
		return;

	text_.setAlign(value);
}

AlignHorizontal LabelImage::align_horizontal() const noexcept {
	return text_.getAlign();
}

void LabelImage::font(Font2D_view value, PropertyDriver driver) {
	if (AccessProperty::setter(*this, property.font, driver))
		return;

	text_.setFont(std::move(value));
}

const Font2D_view& LabelImage::font() const noexcept {
	return text_.getFont();
}

void LabelImage::font_size(FontSize value, PropertyDriver driver) {
	if (AccessProperty::setter(*this, property.font_size, driver))
		return;

	text_.setSize(value);
}

FontSize LabelImage::font_size() const noexcept {
	return text_.getSize();
}

// -------------------------------------------------------------------------------------------------

void LabelImage::text(std::string value) {
	text_.setString(std::move(value));
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

const std::string& LabelImage::text() const noexcept {
	return text_.getString();
}

// -------------------------------------------------------------------------------------------------

void LabelImage::doStyle(ContextStyle& ctx) {
	PropertySetterContext<LabelImage> setter{*this, ctx.style, context()};
	access_properties(setter);
}

void LabelImage::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	const auto contentString = text_.getContent(-1, -1);
	const auto contentImage = libv::vec::cast<float>(property.bg_image()->size());

	AccessLayout::lastDynamic(*this) = {libv::vec::max(contentString, contentImage), 0.f};
}

void LabelImage::doLayout2(const ContextLayout2& environment) {
	text_.setLimit(libv::vec::xy(environment.size));
}

void LabelImage::doRender(ContextRender& ctx) {
	if (ctx.changedSize) {
		bg_mesh.clear();
		auto pos = bg_mesh.attribute(attribute_position);
		auto tex = bg_mesh.attribute(attribute_texture0);
		auto index = bg_mesh.index();

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

	const auto guard_m = ctx.gl.model.push_guard();
 	ctx.gl.model.translate(position());

	{
		ctx.gl.program(*property.bg_shader());
		ctx.gl.texture(property.bg_image()->texture(), property.bg_shader()->textureChannel);
		ctx.gl.uniform(property.bg_shader()->uniform_color, property.bg_color());
		ctx.gl.uniform(property.bg_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(bg_mesh);
	} {
		const auto guard_s = ctx.gl.state.push_guard();
		ctx.gl.state.blendSrc_Source1Color();
		ctx.gl.state.blendDst_One_Minus_Source1Color();

		ctx.gl.program(*property.font_shader());
		ctx.gl.texture(font()->texture(), property.font_shader()->textureChannel);
		ctx.gl.uniform(property.font_shader()->uniform_color, property.font_color());
		ctx.gl.uniform(property.font_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(text_.mesh());
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
