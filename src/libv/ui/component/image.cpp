// Project: libv.ui, File: src/libv/ui/component/image.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/image.hpp>
// pro
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreImage : CoreComponent {
	friend class Image;
	[[nodiscard]] inline auto handler() { return Image{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> bg_color;
		PropertyL1<Texture2D_view> bg_image;
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
void CoreImage::access_properties(T& ctx) {
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

void CoreImage::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreImage> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::doStyle(ctx);
}

libv::vec3f CoreImage::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	const auto dynamic_size_image = property.bg_image()->size().cast<float>() + padding_size();

	return {dynamic_size_image, 0.f};
}

void CoreImage::doRender(Renderer& r) {
	r.texture_2D(padding_LB(), layout_size2() - padding_size(), {0, 0}, {1, 1},
			property.bg_color(),
			property.bg_image(),
			property.bg_shader());
}

// =================================================================================================

core_ptr Image::create_core(std::string name) {
	return create_core_ptr<CoreImage>(std::move(name));
}

// -------------------------------------------------------------------------------------------------

void Image::color(Color value) {
	AccessProperty::manual(self(), self().property.bg_color, value);
}

const Color& Image::color() const noexcept {
	return self().property.bg_color();
}

void Image::image(Texture2D_view value) {
	AccessProperty::manual(self(), self().property.bg_image, std::move(value));
}

const Texture2D_view& Image::image() const noexcept {
	return self().property.bg_image();
}

void Image::shader(ShaderImage_view value) {
	AccessProperty::manual(self(), self().property.bg_shader, std::move(value));
}

const ShaderImage_view& Image::shader() const noexcept {
	return self().property.bg_shader();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
