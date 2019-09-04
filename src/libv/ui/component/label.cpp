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
	BaseComponent(UnnamedTag{}, "label") { }

Label::Label(std::string name) :
	BaseComponent(std::move(name)) { }

Label::Label(UnnamedTag, const std::string_view type) :
	BaseComponent(UnnamedTag{}, type) { }

Label::~Label() { }

// -------------------------------------------------------------------------------------------------

void Label::setText(std::string string_) {
	string.setString(std::move(string_));
	flagAuto(Flag::pendingLayout);
}

const std::string& Label::getText() const {
	return string.getString();
}

// -------------------------------------------------------------------------------------------------

void Label::doStyle() {
	set(properties);
}

void Label::doRender(ContextRender& context) {
	const auto guard_s = context.gl.state.push_guard();
	const auto guard_m = context.gl.model.push_guard();
	context.gl.model.translate(position());
	context.gl.state.blendSrc_Source1Color();
	context.gl.state.blendDst_One_Minus_Source1Color();

	string.setFont(properties.font(), properties.font_size());
	string.setAlign(properties.align());

	context.gl.program(*properties.font_shader());
	properties.font()->bind(context.gl, properties.font_shader()->textureChannel);
//	context.gl.texture(font->texture, program->textureChannel);
	context.gl.uniform(properties.font_shader()->uniform_color, properties.font_color());
	context.gl.uniform(properties.font_shader()->uniform_MVPmat, context.gl.mvp());
	string.render(context.gl);
//	context.gl.render(string.mesh);
}

void Label::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	string.setFont(properties.font(), properties.font_size());
	string.setAlign(properties.align());
	const auto content = string.getContent(-1, -1);
	AccessLayout::lastDynamic(*this) = {content, 0.f};
}

void Label::doLayout2(const ContextLayout2& environment) {
	string.setLimit(libv::vec::xy(environment.size));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
