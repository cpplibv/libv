// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/label.hpp>
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
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
void Label::access_properties(T& ctx) {
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
}

// -------------------------------------------------------------------------------------------------

Label::Label() :
	BaseComponent(UnnamedTag, "label") { }

Label::Label(std::string name) :
	BaseComponent(std::move(name)) { }

Label::Label(UnnamedTag_t, const std::string_view type) :
	BaseComponent(UnnamedTag, type) { }

Label::~Label() { }

// -------------------------------------------------------------------------------------------------

void Label::align_horizontal(AlignHorizontal value, PropertyDriver driver) {
	if (AccessProperty::setter(*this, property.align_horizontal, driver))
		return;

	text_.setAlign(value);
}

AlignHorizontal Label::align_horizontal() const noexcept {
	return text_.getAlign();
}

void Label::font(Font2D_view value, PropertyDriver driver) {
	if (AccessProperty::setter(*this, property.font, driver))
		return;

	text_.setFont(std::move(value));
}

const Font2D_view& Label::font() const noexcept {
	return text_.getFont();
}

void Label::font_size(FontSize value, PropertyDriver driver) {
	if (AccessProperty::setter(*this, property.font_size, driver))
		return;

	text_.setSize(value);
}

FontSize Label::font_size() const noexcept {
	return text_.getSize();
}

// -------------------------------------------------------------------------------------------------

void Label::text(std::string value) {
	text_.setString(std::move(value));
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

const std::string& Label::text() const noexcept {
	return text_.getString();
}

// -------------------------------------------------------------------------------------------------

void Label::doStyle(ContextStyle& ctx) {
	PropertySetterContext<Label> setter{*this, ctx.style, context()};
	access_properties(setter);
}

void Label::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	const auto content = text_.getContent(-1, -1);
	AccessLayout::lastDynamic(*this) = {content, 0.f};
}

void Label::doLayout2(const ContextLayout2& environment) {
	text_.setLimit(libv::vec::xy(environment.size));
}

void Label::doRender(ContextRender& ctx) {
	const auto guard_s = ctx.gl.state.push_guard();
	const auto guard_m = ctx.gl.model.push_guard();
	ctx.gl.model.translate(position());
	ctx.gl.state.blendSrc_Source1Color();
	ctx.gl.state.blendDst_One_Minus_Source1Color();

	ctx.gl.program(*property.font_shader());
	ctx.gl.texture(font()->texture(), property.font_shader()->textureChannel);
	ctx.gl.uniform(property.font_shader()->uniform_color, property.font_color());
	ctx.gl.uniform(property.font_shader()->uniform_MVPmat, ctx.gl.mvp());
	ctx.gl.render(text_.mesh());
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
