// File: image.cpp - Created on 2017.10.22. 11:40 - Author: Vader

// hpp
#include <libv/ui/component/image.hpp>
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

struct CoreImage : CoreComponent {
	friend class Image;
	[[nodiscard]] inline auto handler() { return Image{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Color> bg_color;
		PropertyL<Texture2D_view> bg_image;
		PropertyR<ShaderImage_view> bg_shader;
	} property;

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

public:
	using CoreComponent::CoreComponent;

private:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doLayout1(const ContextLayout1& environment) override;
	virtual void doRender(ContextRender& context) override;
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
	CoreComponent::access_properties(setter);
}

void CoreImage::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	AccessLayout::lastDynamic(*this) = {libv::vec::cast<float>(property.bg_image()->size()), 0.f};
}

void CoreImage::doRender(ContextRender& context) {
	if (context.changedSize) {
		mesh.clear();
		auto pos = mesh.attribute(attribute_position);
		auto tex = mesh.attribute(attribute_texture0);
		auto index = mesh.index();

		pos(0, 0, 0);
		pos(layout_size().x, 0, 0);
		pos(layout_size().x, layout_size().y, 0);
		pos(0, layout_size().y, 0);

		tex(0, 0);
		tex(1, 0);
		tex(1, 1);
		tex(0, 1);

		index.quad(0, 1, 2, 3);
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

Image::Image(std::string name) :
	ComponenetHandler<CoreImage, EventHostGeneral<Image>>(std::move(name)) { }

Image::Image(GenerateName_t gen, const std::string_view type) :
	ComponenetHandler<CoreImage, EventHostGeneral<Image>>(gen, type) { }

Image::Image(core_ptr core) noexcept :
	ComponenetHandler<CoreImage, EventHostGeneral<Image>>(core) { }

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
