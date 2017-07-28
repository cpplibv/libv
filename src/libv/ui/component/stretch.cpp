// File: stretch.cpp - Created on 2017.10.22. 11:40 - Author: Vader

// hpp
#include <libv/ui/component/stretch.hpp>
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

Stretch::Stretch() :
	ComponentBase(UnnamedTag{}, "stretch") {}

void Stretch::image(std::shared_ptr<Texture2D> image_) {
	this->image_ = std::move(image_);
	// TODO P5: Could be restricted to only invalidate if layout is dependent on content
	//			| But that should be taken care of by the system, and not individual decision on implementation sides
	//			| This also applies for other components
	//			| This might also requires the remove of the lstContext variable, but in that I am not sure
	invalidate(Flag::invalidLayout | Flag::invalidRender);
}

void Stretch::doAttach(ContextUI& context) {
	shader = context.shader<ShaderImage>();
}

void Stretch::doLayoutPass1(const ContextLayoutPass1& environment) {
	(void) environment;
	if (image_)
		lastContent = {libv::vec::cast<float>(image_->size()), 0.f};
	else
		lastContent = {0, 0, 0};
}

void Stretch::doRender(ContextRender& context) {
	if (!image_)
		return;

	if (context.changedSize || flags.match_any(Flag::invalidRender)) {
		mesh.clear();
		auto pos = mesh.attribute(attribute_position);
		auto tex = mesh.attribute(attribute_texture0);
		auto index = mesh.index();

		// y3   12--13--14--15
		//      | / | / | / |
		// y2   8---9---10--11
		//      | / | / | / |
		// y1   4---5---6---7
		//      | / | / | / |
		// y0   0---1---2---3
		//
		//      x0  x1  x2  x3

		const auto borderPos = min(cast<float>(image_->size()), xy(size)) * 0.5f;
		const auto borderTex = min(xy(size) / max(cast<float>(image_->size()), 1.0f) * 0.5f, 0.5f);

		const auto p0 = libv::vec2f{0.0f, 0.0f};
		const auto p1 = borderPos;
		const auto p2 = xy(size) - borderPos;
		const auto p3 = xy(size);

		const auto t0 = libv::vec2f{0.0f, 0.0f};
		const auto t1 = borderTex;
		const auto t2 = 1.0f - borderTex;
		const auto t3 = libv::vec2f{1.0f, 1.0f};

		pos(p0.x, p0.y, 0); tex(t0.x, t0.y);
		pos(p1.x, p0.y, 0); tex(t1.x, t0.y);
		pos(p2.x, p0.y, 0); tex(t2.x, t0.y);
		pos(p3.x, p0.y, 0); tex(t3.x, t0.y);

		pos(p0.x, p1.y, 0); tex(t0.x, t1.y);
		pos(p1.x, p1.y, 0); tex(t1.x, t1.y);
		pos(p2.x, p1.y, 0); tex(t2.x, t1.y);
		pos(p3.x, p1.y, 0); tex(t3.x, t1.y);

		pos(p0.x, p2.y, 0); tex(t0.x, t2.y);
		pos(p1.x, p2.y, 0); tex(t1.x, t2.y);
		pos(p2.x, p2.y, 0); tex(t2.x, t2.y);
		pos(p3.x, p2.y, 0); tex(t3.x, t2.y);

		pos(p0.x, p3.y, 0); tex(t0.x, t3.y);
		pos(p1.x, p3.y, 0); tex(t1.x, t3.y);
		pos(p2.x, p3.y, 0); tex(t2.x, t3.y);
		pos(p3.x, p3.y, 0); tex(t3.x, t3.y);

		index({4, 0, 5, 1, 6, 2, 7, 3}); index({3, 8}); // jump
		index({8, 4, 9, 5, 10, 6, 11, 7}); index({7, 12}); // jump
		index({12, 8, 13, 9, 14, 10, 15, 11});
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
