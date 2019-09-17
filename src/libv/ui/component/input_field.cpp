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
#include <libv/ui/shader/shader_quad.hpp>
#include <libv/ui/style.hpp>
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
	if (event.loss)
		displayCursor = false;

	if (event.gain) {
		displayCursor = true;
		cursorStartTime = clock::now();
	}

	flagAuto(Flag::pendingRender);
}

bool InputField::onMouse(const EventMouse& event) {
	if (event.isButton() && event.button().action == libv::input::Action::press)
		focus();

	if (event.isMovement() && event.movement().enter)
		{} // Set style to hover if not disabled

	if (event.isMovement() && event.movement().leave)
		{} // Set style to normal or disable

	return true;
}

// -------------------------------------------------------------------------------------------------

void InputField::doAttach() {
	watchChar(true);
	watchKey(true);
	watchFocus(true);
	watchMouse(Flag::watchMouseButton);
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

//	if (true) { // TODO P5: on Font or on FontSize change would be the correct condition
	if (context.changedSize) {
		cursor_mesh.clear();
		auto pos = cursor_mesh.attribute(attribute_position);
		auto index = cursor_mesh.index();

		const auto cursorHeight = properties.font()->getLineAdvance(properties.font_size());
		// <<< P5: ui.settings for cursorWidth
		const auto cursorWidth = std::floor(std::min((cursorHeight - 12.f) / 24.f + 2.f, 5.f));

		// 3-2
		// |/|
		// 0-1
		pos(0, 0, 0);
		pos(cursorWidth, 0, 0);
		pos(cursorWidth, cursorHeight, 0);
		pos(0, cursorHeight, 0);

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

	// <<< P3: ui.settings for cursor periods
	constexpr auto cursor_show_period = std::chrono::milliseconds(350);
	constexpr auto cursor_hide_period = std::chrono::milliseconds(350);

	constexpr auto cursor_flash_period = cursor_show_period + cursor_hide_period;
	const auto cursor_flash_iteration = time_mod(context.now - cursorStartTime, cursor_flash_period);

	if (displayCursor && cursor_flash_iteration < cursor_show_period) {
//		context.gl.model.translate(string.getPen());

		context.gl.program(*properties.cursor_shader());
		context.gl.uniform(properties.cursor_shader()->uniform_color, properties.font_color());
//		context.gl.uniform(properties.cursor_shader()->uniform_color, properties.cursor_color()); // <<< P4: property name collusion
		context.gl.uniform(properties.cursor_shader()->uniform_MVPmat, context.gl.mvp());
		context.gl.render(cursor_mesh);
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
