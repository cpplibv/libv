// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/label_image.hpp>
// pro
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/core_component.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/text_layout.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreLabelImage : CoreComponent {
	friend class LabelImage;
	[[nodiscard]] inline auto handler() { return LabelImage{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> bg_color;
		PropertyL<Texture2D_view> bg_image;
		PropertyR<ShaderImage_view> bg_shader;

		PropertyR<Color> font_color;
		PropertyR<ShaderFont_view> font_shader;

		PropertyL<> align_horizontal;
		PropertyL<> align_vertical;
		PropertyL<> font;
		PropertyL<> font_size;
	} property;

private:
	TextLayout text_;

public:
	using CoreComponent::CoreComponent;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreLabelImage::access_properties(T& ctx) {
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
			[](auto& c, auto v) { c.text_.align_horizontal(v); },
			[](const auto& c) { return c.text_.align_horizontal(); },
			AlignHorizontal::left,
			pgr::appearance, pnm::align_horizontal,
			"Horizontal alignment of the text"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.align_vertical; },
			[](auto& c, auto v) { c.text_.align_vertical(v); },
			[](const auto& c) { return c.text_.align_vertical(); },
			AlignVertical::top,
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

void CoreLabelImage::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreLabelImage> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

libv::vec3f CoreLabelImage::doLayout1(const ContextLayout1& environment) {
	const auto dynamic_size_text = text_.content(xy(environment.size) - padding_size()) + padding_size();
	const auto dynamic_size_image = property.bg_image()->size().cast<float>();

	return {libv::vec::max(dynamic_size_text, dynamic_size_image), 0.f};
}

void CoreLabelImage::doLayout2(const ContextLayout2& environment) {
	text_.limit(xy(environment.size) - padding_size());
}

void CoreLabelImage::doRender(Renderer& r) {
	r.texture_2D({0, 0}, layout_size2(), {0, 0}, {1, 1},
			property.bg_color(),
			property.bg_image(),
			property.bg_shader());

	r.text(padding_LB(), text_,
			property.font_color(),
			text_.font(),
			property.font_shader());
}

// -------------------------------------------------------------------------------------------------

LabelImage::LabelImage(std::string name) :
	ComponentHandler<CoreLabelImage, EventHostGeneral<LabelImage>>(std::move(name)) { }

LabelImage::LabelImage(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CoreLabelImage, EventHostGeneral<LabelImage>>(gen, type) { }

LabelImage::LabelImage(core_ptr core) noexcept :
	ComponentHandler<CoreLabelImage, EventHostGeneral<LabelImage>>(core) { }

// -------------------------------------------------------------------------------------------------

void LabelImage::color(Color value) {
	AccessProperty::manual(self(), self().property.bg_color, value);
}

const Color& LabelImage::color() const noexcept {
	return self().property.bg_color();
}

void LabelImage::image(Texture2D_view value) {
	AccessProperty::manual(self(), self().property.bg_image, std::move(value));
}

const Texture2D_view& LabelImage::image() const noexcept {
	return self().property.bg_image();
}

void LabelImage::shader(ShaderImage_view value) {
	AccessProperty::manual(self(), self().property.bg_shader, std::move(value));
}

const ShaderImage_view& LabelImage::shader() const noexcept {
	return self().property.bg_shader();
}

// -------------------------------------------------------------------------------------------------

void LabelImage::align_horizontal(AlignHorizontal value) {
	AccessProperty::setter(self(), self().property.align_horizontal, PropertyDriver::manual, [&]() {
		self().text_.align_horizontal(value);
	});
}

AlignHorizontal LabelImage::align_horizontal() const noexcept {
	return self().text_.align_horizontal();
}

void LabelImage::align_vertical(AlignVertical value) {
	AccessProperty::setter(self(), self().property.align_vertical, PropertyDriver::manual, [&]() {
		self().text_.align_vertical(value);
	});
}

AlignVertical LabelImage::align_vertical() const noexcept {
	return self().text_.align_vertical();
}

void LabelImage::font(Font2D_view value) {
	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
		self().text_.font(std::move(value));
	});
}

const Font2D_view& LabelImage::font() const noexcept {
	return self().text_.font();
}

void LabelImage::font_size(FontSize value) {
	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
		self().text_.size(value);
	});
}

FontSize LabelImage::font_size() const noexcept {
	return self().text_.size();
}

// -------------------------------------------------------------------------------------------------

void LabelImage::text(std::string value) {
	self().text_.string(std::move(value));
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

const std::string& LabelImage::text() const noexcept {
	return self().text_.string();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
