// File: image.cpp - Created on 2017.10.22. 11:40 - Author: Vader

// hpp
#include <libv/ui/component/image.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/texture_2D.hpp>
#include <libv/ui/shader/shader_image.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

Image::Image() :
	ComponentBase(UnnamedTag{}, "image") {}

void Image::image(std::shared_ptr<Texture2D> image_) {
	this->image_ = std::move(image_);
	// TODO P5: Could be restricted to only invalidate if layout is dependent on content
	//			| But that should be taken care of by the system, and not individual decision on implementation sides
	//			| This also applies for other components
	//			| This might also requires the remove of the lstContext variable, but in that I am not sure
	invalidate(Flag::invalidLayout);
}

void Image::doAttach(ContextUI& context) {
	shader = context.shader<ShaderImage>();
}

void Image::doLayoutPass1(const ContextLayoutPass1& environment) {
	(void) environment;
	if (image_)
		lastContent = {libv::vec::cast<float>(image_->size()), 0.f};
	else
		lastContent = {0, 0, 0};
}

void Image::doRender(ContextRender& context) {
	if (!image_)
		return;

	if (context.changedSize) {
		mesh.clear();
		auto pos = mesh.attribute(attribute_position);
		auto tex = mesh.attribute(attribute_texture0);
		auto index = mesh.index();

		pos(0, 0, 0);
		pos(size.x, 0, 0);
		pos(size.x, size.y, 0);
		pos(0, size.y, 0);

		tex(0, 0);
		tex(1, 0);
		tex(1, 1);
		tex(0, 1);

		index.quad(0, 1, 2, 3);
	}

	const auto guard_m = context.gl.model.push_guard();
	context.gl.model.translate(position);

	context.gl.program(*shader);
	context.gl.uniform(shader->uniform_color, color_);
	context.gl.uniform(shader->uniform_MVPmat, context.gl.mvp());
	context.gl.texture(image_->texture(), shader->textureChannel);
	context.gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
