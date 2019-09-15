// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/input_field.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/input/event.hpp>
#include <libv/utility/overload.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

InputField::InputField() :
	BaseComponent(UnnamedTag, "input-field") { }

InputField::InputField(std::string name) :
	BaseComponent(std::move(name)) { }

InputField::InputField(UnnamedTag_t, const std::string_view type) :
	BaseComponent(UnnamedTag, type) { }

InputField::~InputField() { }

// -------------------------------------------------------------------------------------------------

void InputField::setText(std::string string_) {
	string.setString(std::move(string_));
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

const std::string& InputField::getText() const {
	return string.getString();
}

// -------------------------------------------------------------------------------------------------

bool InputField::onChar(const libv::input::EventChar& event) {
	string.push_back(event.utf8.data());
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	return true;
}

bool InputField::onKey(const libv::input::EventKey& event) {
	if (event.key == libv::input::Key::Backspace && event.action != libv::input::Action::release) {
		string.pop_back();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		return true;
	}
	return false;
}

void InputField::onFocus(const EventFocus& event) {
	(void) event;

	if (event.loss)
		{} // Disable cursor
	if (event.gain)
		{} // Enable cursor
}

bool InputField::onMouse(const EventMouse& event) {
	if (event.isButton())
		focus();

	return true;
}

// -------------------------------------------------------------------------------------------------

void InputField::doAttach() {
	flagAuto(Flag::focusable | Flag::watchMouseButton);
}

void InputField::doStyle() {
	set(properties);
}

void InputField::doRender(ContextRender& context) {
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

	{
		context.gl.program(*properties.image_shader());
		context.gl.uniform(properties.image_shader()->uniform_color, properties.color());
		context.gl.uniform(properties.image_shader()->uniform_MVPmat, context.gl.mvp());
		context.gl.texture(properties.image()->texture(), properties.image_shader()->textureChannel);
		context.gl.render(mesh);
	} {
		const auto guard_s = context.gl.state.push_guard();
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
}

void InputField::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	string.setFont(properties.font(), properties.font_size());
	string.setAlign(properties.align());
	const auto contentString = string.getContent(-1, -1);
	const auto contentImage = libv::vec::cast<float>(properties.image()->size());

	AccessLayout::lastDynamic(*this) = {libv::vec::max(contentString, contentImage), 0.f};
}

void InputField::doLayout2(const ContextLayout2& environment) {
	string.setLimit(libv::vec::xy(environment.size));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv