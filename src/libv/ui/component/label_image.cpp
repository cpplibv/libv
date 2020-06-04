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
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/string_2D.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreLabelImage : BaseComponent {
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
	libv::glr::Mesh bg_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	String2D text_;

public:
	using BaseComponent::BaseComponent;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(ContextRender& context) override;
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
			[](auto& c, auto v) { c.text_.align(v); },
			[](const auto& c) { return c.text_.align(); },
			AlignHorizontal::Left,
			pgr::appearance, pnm::align_horizontal,
			"Horizontal alignment of the text"
	);
// TODO P1: Implement align_vertical
//	ctx.indirect(
//			[](auto& c) -> auto& { return c.property.align_vertical; },
//			[](auto& c, auto v) { c.text_.align_vertical(v); },
//			[](const auto& c) { return c.text_.align_vertical(); },
//			AlignVertical::Top,
//			pgr::appearance, pnm::align_vertical,
//			"Vertical alignment of the text"
//	);
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
}

void CoreLabelImage::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	const auto contentString = text_.content(-1, -1);
	const auto contentLabelImage = libv::vec::cast<float>(property.bg_image()->size());

	AccessLayout::lastDynamic(*this) = {libv::vec::max(contentString, contentLabelImage), 0.f};
}

void CoreLabelImage::doLayout2(const ContextLayout2& environment) {
	text_.limit(libv::vec::xy(environment.size));
}

void CoreLabelImage::doRender(ContextRender& ctx) {
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
		ctx.gl.texture(text_.font()->texture(), property.font_shader()->textureChannel);
		ctx.gl.uniform(property.font_shader()->uniform_color, property.font_color());
		ctx.gl.uniform(property.font_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(text_.mesh());
	}
}

// -------------------------------------------------------------------------------------------------

LabelImage::LabelImage(std::string name) :
	ComponenetHandler<CoreLabelImage, EventHostGeneral<LabelImage>>(std::move(name)) { }

LabelImage::LabelImage(GenerateName_t gen, const std::string_view type) :
	ComponenetHandler<CoreLabelImage, EventHostGeneral<LabelImage>>(gen, type) { }

LabelImage::LabelImage(base_ptr core) noexcept :
	ComponenetHandler<CoreLabelImage, EventHostGeneral<LabelImage>>(core) { }

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
		self().text_.align(value);
	});
}

AlignHorizontal LabelImage::align_horizontal() const noexcept {
	return self().text_.align();
}

void LabelImage::align_vertical(AlignVertical value) {
	assert(false && "Not implemented yet");
	AccessProperty::setter(self(), self().property.align_vertical, PropertyDriver::manual, [&]() {
		// self().text_.align_vertical(value);
	});
}

AlignVertical LabelImage::align_vertical() const noexcept {
	assert(false && "Not implemented yet");
	// return self().property.align_vertical();
	return AlignVertical::Top;
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
