// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/input_field.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/input/event.hpp>
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
	text.setString(std::move(string_));

	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

const std::string& InputField::getText() const {
	return text.getString();
}

// -------------------------------------------------------------------------------------------------

bool InputField::onChar(const libv::input::EventChar& event) {
	text.push_back(event.utf8.data());

	cursorStartTime = clock::now();
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	return true;
}

bool InputField::onKey(const libv::input::EventKey& event) {
	if (event.key == libv::input::Key::Backspace && event.action != libv::input::Action::release) {
		text.pop_back();

		cursorStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		return true;
	}
	return false;
}

void InputField::onFocus(const EventFocus& event) {
	if (event.loss)
		{} // Set style to normal or disabled

	if (event.gain) {
		{} // Set style to active if not disabled
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
		{} // Set style to normal or disabled

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
	set(property);

	// =================================================================================================

//	set<std::shared_ptr<Font2D>>("font",
//			[](auto& c, auto v) {
//				c.text.setFont(std::move(v), 12);
//				c.cursorPosition = c.text.getCharacterPosition();
//			}, [](auto& c) {
//				return c.text.getFont();
//			}
//	);
//	set(property.font,
//			[](auto& c, auto v) {
//				c.text.setFont(std::move(v), 12);
//				c.cursorPosition = c.text.getCharacterPosition();
//			}, [](auto& c) {
//				return c.text.getFont();
//			}
//	);

//	libv::meta::foreach_member(property, [](auto p) {
//		set(property);
//	});
//
////	set<Font2D_view>("font", &InputField::setFont, &InputField::getFont);
//	set<PropertyFont>("font", &InputField::setFont, &InputField::getFont);
//	set<PropertyFontSize>("font-size", &InputField::setFontSize, &InputField::getFontSize);
//	set<PropertyFontColor>("font-color", &InputField::font_color);

	// =================================================================================================
}

void InputField::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	text.setFont(property.font(), property.font_size());
	text.setAlign(property.align());
	const auto contentString = text.getContent(-1, -1);
	const auto contentImage = libv::vec::cast<float>(property.image()->size());

	AccessLayout::lastDynamic(*this) = {libv::vec::max(contentString, contentImage), 0.f};
}

void InputField::doLayout2(const ContextLayout2& environment) {
	text.setLimit(libv::vec::xy(environment.size));
	cursorPosition = text.getCharacterPosition();
}

void InputField::doRender(ContextRender& ctx) {
	if (ctx.changedSize) {
		quad_mesh.clear();
		auto pos = quad_mesh.attribute(attribute_position);
		auto tex = quad_mesh.attribute(attribute_texture0);
		auto index = quad_mesh.index();

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

	// <<< P3: on Font or on FontSize change would be the correct condition
//	const auto changedFont = property.font.consumeChange();
//	const auto changedFontSize = property.font.consumeChange();
	const auto changedFont = ctx.changedSize;
	const auto changedFontSize = ctx.changedSize;

	if (changedFont || changedFontSize) {
		cursor_mesh.clear();
		auto pos = cursor_mesh.attribute(attribute_position);
		auto index = cursor_mesh.index();

		const auto cursorHeight = property.font()->getLineAdvance(property.font_size());
		const auto max = context().settings.cursor_width_max;
		const auto min = context().settings.cursor_width_min;
		const auto offset = context().settings.cursor_width_offset;
		const auto scale = context().settings.cursor_width_scale;
		const auto cursorWidth = std::floor(std::min((cursorHeight - offset) / scale + min, max));

		// 3-2
		// |/|
		// 0-1
		pos(0, 0, 0);
		pos(cursorWidth, 0, 0);
		pos(cursorWidth, cursorHeight, 0);
		pos(0, cursorHeight, 0);

		index.quad(0, 1, 2, 3);
	}

	const auto guard_m = ctx.gl.model.push_guard();
 	ctx.gl.model.translate(position());

	{
		ctx.gl.program(*property.image_shader());
		ctx.gl.texture(property.image()->texture(), property.image_shader()->textureChannel);
		ctx.gl.uniform(property.image_shader()->uniform_color, property.color());
		ctx.gl.uniform(property.image_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(quad_mesh);
	} {
		const auto guard_s = ctx.gl.state.push_guard();
		ctx.gl.state.blendSrc_Source1Color();
		ctx.gl.state.blendDst_One_Minus_Source1Color();

		text.setFont(property.font(), property.font_size());
		text.setAlign(property.align());

		ctx.gl.program(*property.font_shader());
		ctx.gl.texture(property.font()->texture(), property.font_shader()->textureChannel);
		ctx.gl.uniform(property.font_shader()->uniform_color, property.font_color());
		ctx.gl.uniform(property.font_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(text.mesh());
	}

	const auto cursor_flash_iteration = time_mod(ctx.now - cursorStartTime, context().settings.cursor_flash_period);

	if (isFocused() && cursor_flash_iteration < context().settings.cursor_show_period) {
		ctx.gl.model.translate({cursorPosition, 0});

		ctx.gl.program(*property.cursor_shader());
		ctx.gl.uniform(property.cursor_shader()->uniform_color, property.font_color());
//		context.gl.uniform(property.cursor_shader()->uniform_color, property.cursor_color()); // <<< P2: property name collusion
		ctx.gl.uniform(property.cursor_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(cursor_mesh);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
