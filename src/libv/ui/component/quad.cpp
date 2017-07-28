// File: quad.cpp - Created on 2017.10.22. 11:40 - Author: Vader

// hpp
#include <libv/ui/component/quad.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/shader/shader_quad.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

Quad::Quad() :
	ComponentBase(UnnamedTag{}, "quad") {
}

void Quad::doAttach(ContextUI& context) {
	shader = context.shader<ShaderQuad>();
}

void Quad::doRender(ContextRender& context) {
	if (context.changedSize) {
		mesh.clear();
		auto pos = mesh.attribute(attribute_position);
		auto index = mesh.index();

		// 3-2
		// |/|
		// 0-1
		pos(0, 0, 0);
		pos(size.x, 0, 0);
		pos(size.x, size.y, 0);
		pos(0, size.y, 0);

		index.quad(0, 1, 2, 3);
	}

	const auto guard_m = context.gl.model.push_guard();
	context.gl.model.translate(position);

	context.gl.program(*shader);
	context.gl.uniform(shader->uniform_color, color_);
	context.gl.uniform(shader->uniform_MVPmat, context.gl.mvp());
	context.gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
