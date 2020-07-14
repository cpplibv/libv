// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/label.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/string_2D.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreLabel : CoreComponent {
	friend class Label;
	[[nodiscard]] inline auto handler() { return Label{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> font_color;
		PropertyR<ShaderFont_view> font_shader;

		PropertyL<> align_horizontal;
		PropertyL<> align_vertical;
		PropertyL<> font;
		PropertyL<> font_size;
	} property;

private:
	String2D text_;

public:
	using CoreComponent::CoreComponent;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreLabel::access_properties(T& ctx) {
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.align_horizontal; },
			[](auto& c, auto v) { c.text_.align_horizontal(v); },
			[](const auto& c) { return c.text_.align_horizontal(); },
			AlignHorizontal::Left,
			pgr::appearance, pnm::align_horizontal,
			"Horizontal alignment of the text"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.align_vertical; },
			[](auto& c, auto v) { c.text_.align_vertical(v); },
			[](const auto& c) { return c.text_.align_vertical(); },
			AlignVertical::Top,
			pgr::appearance, pnm::align_vertical,
			"Vertical alignment of the text"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_color; },
			Color(0, 0, 0, 1),
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
			[](auto& c, auto v) { c.text_.font(std::move(v)); },
			[](const auto& c) { return c.text_.font(); },
			[](auto& u) { return u.fallbackFont(); },
			pgr::font, pnm::font,
			"Font file"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.font_size; },
			[](auto& c, auto v) { c.text_.size(v); },
			[](const auto& c) { return c.text_.size(); },
			FontSize{12},
			pgr::font, pnm::font_size,
			"Font size in pixel"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.handler().text(std::move(v)); },
			[](const auto& c) { return c.handler().text(); },
			pgr::behaviour, pnm::text,
			"Displayed text"
	);
}

// -------------------------------------------------------------------------------------------------

void CoreLabel::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreLabel> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

libv::vec3f CoreLabel::doLayout1(const ContextLayout1& environment) {
	const auto dynamic_size_text = text_.content(xy(environment.size));

	return {dynamic_size_text, 0.f};
}

void CoreLabel::doLayout2(const ContextLayout2& environment) {
	text_.limit(libv::vec::xy(environment.size));
}

void CoreLabel::doRender(ContextRender& ctx) {
	const auto guard_s = ctx.gl.state.push_guard();
	const auto guard_m = ctx.gl.model.push_guard();
	ctx.gl.model.translate(layout_position());
	ctx.gl.state.blendSrc_Source1Color();
	ctx.gl.state.blendDst_One_Minus_Source1Color();

	ctx.gl.program(*property.font_shader());
	ctx.gl.texture(text_.font()->texture(), property.font_shader()->textureChannel);
	ctx.gl.uniform(property.font_shader()->uniform_color, property.font_color());
	ctx.gl.uniform(property.font_shader()->uniform_MVPmat, ctx.gl.mvp());
	ctx.gl.render(text_.mesh());
}

// -------------------------------------------------------------------------------------------------

Label::Label(std::string name) :
	ComponentHandler<CoreLabel, EventHostGeneral<Label>>(std::move(name)) { }

Label::Label(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CoreLabel, EventHostGeneral<Label>>(gen, type) { }

Label::Label(core_ptr core) noexcept :
	ComponentHandler<CoreLabel, EventHostGeneral<Label>>(core) { }

// -------------------------------------------------------------------------------------------------

void Label::align_horizontal(AlignHorizontal value) {
	AccessProperty::setter(self(), self().property.align_horizontal, PropertyDriver::manual, [&]() {
		self().text_.align_horizontal(value);
	});
}

AlignHorizontal Label::align_horizontal() const noexcept {
	return self().text_.align_horizontal();
}

void Label::align_vertical(AlignVertical value) {
	AccessProperty::setter(self(), self().property.align_vertical, PropertyDriver::manual, [&]() {
		self().text_.align_vertical(value);
	});
}

AlignVertical Label::align_vertical() const noexcept {
	return self().text_.align_vertical();
}

void Label::font(Font2D_view value) {
	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
		self().text_.font(std::move(value));
	});
}

const Font2D_view& Label::font() const noexcept {
	return self().text_.font();
}

void Label::font_size(FontSize value) {
	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
		self().text_.size(value);
	});
}

FontSize Label::font_size() const noexcept {
	return self().text_.size();
}

// -------------------------------------------------------------------------------------------------

void Label::text(std::string value) {
	self().text_.string(std::move(value));
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

const std::string& Label::text() const noexcept {
	return self().text_.string();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
