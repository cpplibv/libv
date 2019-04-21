// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/label.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/shader/shader_font.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

Label::Label() :
	ComponentBase(UnnamedTag{}, "label") {}

void Label::doAttach(ContextUI& context) {
	// <<< P3: properites: font, size, color, alignment, shader(?)
//	font = context.font("consola.ttf");
	shader = context.shader<ShaderFont>();
	string.setFont(properties.font, properties.font_size);
	string.setAlign(properties.align);
}

void Label::doLayoutPass1(const ContextLayoutPass1& environment) {
	(void) environment;
	const auto content = string.getContent(-1, -1);
	lastContent = {content, 0.f};
}

void Label::doLayoutPass2(const ContextLayoutPass2& environment) {
	string.setLimit(libv::vec::xy(environment.size));
}

void Label::doRender(ContextRender& context) {
	const auto guard_s = context.gl.state.push_guard();
	const auto guard_m = context.gl.model.push_guard();
	context.gl.model.translate(position);
	context.gl.state.blendSrc_Source1Color();
	context.gl.state.blendDst_One_Minus_Source1Color();

//	if (style->changed()) {
		string.setFont(properties.font, properties.font_size);
		string.setAlign(properties.align);
//	}

	context.gl.program(*shader);
	properties.font->bind(context.gl, shader->textureChannel);
//	context.gl.texture(font->texture, program->textureChannel);
	context.gl.uniform(shader->uniform_color, properties.font_color);
	context.gl.uniform(shader->uniform_MVPmat, context.gl.mvp());
	string.render(context.gl);
//	context.gl.render(string.mesh);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
