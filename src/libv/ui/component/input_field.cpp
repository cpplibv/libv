// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/input_field.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/input/event.hpp>
#include <libv/meta/identity.hpp>
// pro
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/log.hpp>
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

void InputField::align_horizontal(AlignHorizontal value) {
	text_.setAlign(value);
	flagAuto(Flag::pendingRender);
}

AlignHorizontal InputField::align_horizontal() const noexcept {
	return text_.getAlign();
}

void InputField::font(Font2D_view value) {
	text_.setFont(std::move(value));
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

const Font2D_view& InputField::font() const noexcept {
	return text_.getFont();
}

void InputField::font_size(FontSize value) {
	text_.setSize(value);
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

FontSize InputField::font_size() const noexcept {
	return text_.getSize();
}

void InputField::text(std::string value) {
	text_.setString(std::move(value));
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
}

const std::string& InputField::text() const noexcept {
	return text_.getString();
}

// -------------------------------------------------------------------------------------------------

bool InputField::onChar(const libv::input::EventChar& event) {
	text_.push_back(event.utf8.data());

	cursorStartTime = clock::now();
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	return true;
}

bool InputField::onKey(const libv::input::EventKey& event) {
	if (event.key == libv::input::Key::Backspace && event.action != libv::input::Action::release) {
		text_.pop_back();

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
		// Set style to hover if not disabled
		set(property.bg_color, property.bg_color() + 0.2f);

	if (event.isMovement() && event.movement().leave)
		// Set style to hover if not disabled
		reset(property.bg_color);

	return true;
}

// -------------------------------------------------------------------------------------------------

void InputField::doAttach() {
	watchChar(true);
	watchKey(true);
	watchFocus(true);
	watchMouse(Flag::watchMouseButton | Flag::watchMouseEnter);
}

void InputField::doStyle(ContextStyle& ctx) {
	property.access(ctx);
}

void InputField::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	const auto contentString = text_.getContent(-1, -1);
	const auto contentImage = libv::vec::cast<float>(property.bg_image()->size());

	AccessLayout::lastDynamic(*this) = {libv::vec::max(contentString, contentImage), 0.f};
}

void InputField::doLayout2(const ContextLayout2& environment) {
	text_.setLimit(libv::vec::xy(environment.size));
	cursorPosition = text_.getCharacterPosition();
}

void InputField::doRender(ContextRender& ctx) {
	if (ctx.changedSize) {
		bg_mesh.clear();
		auto pos = bg_mesh.attribute(attribute_position);
		auto tex = bg_mesh.attribute(attribute_texture0);
		auto index = bg_mesh.index();

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

	const auto changedFont = property.font.consumeChange();
	const auto changedFontSize = property.font_size.consumeChange();

	if (changedFont || changedFontSize) {
		cursor_mesh.clear();
		auto pos = cursor_mesh.attribute(attribute_position);
		auto index = cursor_mesh.index();

		const auto cursorHeight = font()->getLineAdvance(font_size());
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
		ctx.gl.program(*property.bg_shader());
		ctx.gl.texture(property.bg_image()->texture(), property.bg_shader()->textureChannel);
		ctx.gl.uniform(property.bg_shader()->uniform_color, property.bg_color());
		ctx.gl.uniform(property.bg_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(bg_mesh);
	} {
		const auto guard_s = ctx.gl.state.push_guard();
		ctx.gl.state.blendSrc_Source1Color();
		ctx.gl.state.blendDst_One_Minus_Source1Color();

		ctx.gl.program(*property.font_shader());
		ctx.gl.texture(font()->texture(), property.font_shader()->textureChannel);
		ctx.gl.uniform(property.font_shader()->uniform_color, property.font_color());
		ctx.gl.uniform(property.font_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(text_.mesh());
	}

	const auto cursor_flash_iteration = time_mod(ctx.now - cursorStartTime, context().settings.cursor_flash_period);

	if (isFocused() && cursor_flash_iteration < context().settings.cursor_show_period) {
		ctx.gl.model.translate({cursorPosition, 0});

		ctx.gl.program(*property.cursor_shader());
		ctx.gl.uniform(property.cursor_shader()->uniform_color, property.cursor_color());
		ctx.gl.uniform(property.cursor_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(cursor_mesh);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
