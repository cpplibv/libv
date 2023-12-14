// Project: libv.ui, File: src/libv/ui/component/label.cpp

// hpp
#include <libv/ui/component/label.hpp>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component/layout/layout_text.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_resource.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/property_system/property_access.hpp>
#include <libv/ui/resource/font_2D.hpp>
#include <libv/ui/resource/shader_font.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreLabel : CoreComponent {
	using base_type = CoreComponent;
	using base_type::base_type;

public:
	struct Properties {
		PropertyR<Background> background;

		PropertyR<Color> font_color;
		PropertyR<ShaderFont_view> font_shader;

		PropertyL2<> align_horizontal;
		PropertyL2<> align_vertical;
		PropertyL1L2<> font;
		PropertyL1L2<> font_size;
	} property;

	template <typename T> static void access_properties(T& ctx);

public:
	LayoutText text_;

public:
	virtual	libv::vec4f getInnerContentBounds() override;

public:
	virtual void doStyle(StyleAccess& access) override;
	virtual libv::vec2f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreLabel::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.background; },
			Background::none(),
			pgr::appearance, pnm::background, "Background"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.align_horizontal; },
			[](auto& c, auto v) { c.text_.align_horizontal(v); },
			[](const auto& c) { return c.text_.align_horizontal(); },
			AlignHorizontal::left,
			pgr::appearance, pnm::align_horizontal, "Horizontal alignment of the text"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.align_vertical; },
			[](auto& c, auto v) { c.text_.align_vertical(v); },
			[](const auto& c) { return c.text_.align_vertical(); },
			AlignVertical::top,
			pgr::appearance, pnm::align_vertical, "Vertical alignment of the text"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_color; },
			Color(0, 0, 0, 1),
			pgr::appearance, pnm::font_color, "Font color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_shader; },
			[](auto& u) { return u.shaderFont(); },
			pgr::appearance, pnm::font_shader, "Font shader"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.font; },
			[](auto& c, auto v) { c.text_.font(std::move(v)); },
			[](const auto& c) { return c.text_.font(); },
			[](auto& u) { return u.fallbackFont(); },
			pgr::font, pnm::font, "Font file"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.font_size; },
			[](auto& c, auto v) { c.text_.size(v); },
			[](const auto& c) { return c.text_.size(); },
			FontSize{12},
			pgr::font, pnm::font_size, "Font size in pixel"
	);
	ctx.synthesize(
			T::handler_setter(&Label::text),
			T::handler_getter(&Label::text),
			pgr::behaviour, pnm::text, "Displayed text"
	);
}

// -------------------------------------------------------------------------------------------------

libv::vec4f CoreLabel::getInnerContentBounds() {
	return {padding_LB() + text_.content_bounding_pos(), text_.content_bounding_size()};
}

// -------------------------------------------------------------------------------------------------

void CoreLabel::doStyle(StyleAccess& access) {
	access.self(*this);
}

libv::vec2f CoreLabel::doLayout1(const ContextLayout1& layoutEnv) {
	const auto dynamic_size_text = text_.content(
			layoutEnv.minusOneIfUnlimited(layoutEnv.limit - padding_size())
		) + padding_size();

	return dynamic_size_text;
}

void CoreLabel::doLayout2(const ContextLayout2& layoutEnv) {
	text_.limit(layoutEnv.size - padding_size());
}

void CoreLabel::doRender(Renderer& r) {
	property.background().render(r, {0, 0}, layout_size(), *this);

	if (!r.cull_test({0, 0}, layout_size()))
		return;
	r.text(padding_LB(), text_,
			property.font_color(),
			text_.font(),
			property.font_shader());
}

// =================================================================================================

core_ptr Label::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

core_ptr Label::create_core(std::string name, std::string text) {
	auto p = create_core_ptr<CoreType>(std::move(name));
	p->text_.string(std::move(text));
	return p;
}

bool Label::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void Label::background(Background value) {
	AccessProperty::manual(self(), self().property.background, std::move(value));
}

[[nodiscard]] const Background& Label::background() const noexcept {
	return self().property.background();
}

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

void Label::font_color(Color value) {
	AccessProperty::manual(self(), self().property.font_color, value);
}

const Color& Label::font_color() const noexcept {
	return self().property.font_color();
}

// -------------------------------------------------------------------------------------------------

void Label::text(std::string value) {
	if (value == self().text_.string())
		return;

	self().text_.string(std::move(value));
	self().markInvalidLayout(true, false);
	self().flagAuto(Flag::pendingRender);
}

const std::string& Label::text() const noexcept {
	return self().text_.string();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
