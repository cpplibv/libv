// File: image.cpp - Created on 2017.10.22. 11:40 - Author: Vader

// hpp
#include <libv/ui/component/image.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

Image::Image() :
	BaseComponent(UnnamedTag{}, "image") { }

Image::Image(std::string name) :
	BaseComponent(std::move(name)) { }

Image::Image(UnnamedTag, const std::string_view type) :
	BaseComponent(UnnamedTag{}, type) { }

Image::~Image() { }

// -------------------------------------------------------------------------------------------------

void Image::doStyle() {
	this->set(properties);
}

void Image::doRender(ContextRender& context) {
	if (context.changedSize) {
		mesh.clear();
		auto pos = mesh.attribute(attribute_position);
		auto tex = mesh.attribute(attribute_texture0);
		auto index = mesh.index();

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

	const auto guard_m = context.gl.model.push_guard();
	context.gl.model.translate(position());

	context.gl.program(*properties.image_shader());
	context.gl.uniform(properties.image_shader()->uniform_color, properties.color());
	context.gl.uniform(properties.image_shader()->uniform_MVPmat, context.gl.mvp());
	context.gl.texture(properties.image()->texture(), properties.image_shader()->textureChannel);
	context.gl.render(mesh);
}

void Image::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	AccessLayout::lastDynamic(*this) = {libv::vec::cast<float>(properties.image()->size()), 0.f};
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
