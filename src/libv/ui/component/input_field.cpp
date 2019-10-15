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
#include <libv/ui/context_state.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_access.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/shader/shader_quad.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/texture_2D.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
void InputField::access_properties(T& ctx) {
//	ctx.property(
//			[](auto& c) -> auto& { return c.property.focus_select_policy; },
//			FocusSelectPolicy::caretAtCursorOrEnd,
//			pgr::behaviour, pnm::focus_select_policy,
//			"Focus select policy"
//	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.bg_color; },
			Color(1, 1, 1, 1),
			pgr::appearance, pnm::bg_color,
			"Background color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.bg_image; },
			[](auto& u) { return u.fallbackTexture2D(); },
			pgr::appearance, pnm::bg_image,
			"Background image"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.bg_shader; },
			[](auto& u) { return u.shaderImage(); },
			pgr::appearance, pnm::bg_shader,
			"Background shader"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.caret_color; },
			Color(1, 1, 1, 1),
			pgr::appearance, pnm::caret_color,
			"Cursor color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.caret_shader; },
			[](auto& u) { return u.shaderQuad(); },
			pgr::appearance, pnm::caret_shader,
			"Cursor shader"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.align_horizontal; },
			[](auto& c, auto v) { c.align_horizontal(v, PropertyDriver::style); },
			[](const auto& c) { return c.align_horizontal(); },
			AlignHorizontal::Left,
			pgr::appearance, pnm::align_horizontal,
			"Horizontal alignment of the text"
	);
//	ctx.indirect(
//			[](auto& c) -> auto& { return c.property.align_vertical; },
//			[](auto& c, auto v) { c.align_vertical(v, PropertyDriver::style); },
//			[](const auto& c) { return c.align_vertical(); },
//			AlignVertical::Top,
//			pgr::appearance, pnm::align_vertical,
//			"Vertical alignment of the text"
//	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_color; },
			Color(1, 1, 1, 1),
			pgr::appearance, pnm::font_color,
			"Font color"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_shader; },
			[](auto& u) { return u.shaderFont(); },
			pgr::appearance, pnm::font_shader,
			"Font shader"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.font; },
			[](auto& c, auto v) { c.font(std::move(v), PropertyDriver::style); },
			[](const auto& c) { return c.font(); },
			[](auto& u) { return u.fallbackFont(); },
			pgr::font, pnm::font,
			"Font file"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.font_size; },
			[](auto& c, auto v) { c.font_size(v, PropertyDriver::style); },
			[](const auto& c) { return c.font_size(); },
			FontSize{12},
			pgr::font, pnm::font_size,
			"Font size in pixel"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.text(std::move(v)); },
			[](const auto& c) { return c.text(); },
			pgr::behaviour, pnm::text,
			"Displayed text"
	);
}

//ComponentPropertyDescription InputField::description;
//namespace { PropertyAutoRegistration<InputField> registration; } // namespace

// -------------------------------------------------------------------------------------------------

InputField::InputField(BaseComponent& parent) :
	BaseComponent(parent, GenerateName, "input-field") { }

InputField::InputField(BaseComponent& parent, std::string name) :
	BaseComponent(parent, std::move(name)) { }

InputField::InputField(BaseComponent& parent, GenerateName_t, const std::string_view type) :
	BaseComponent(parent, GenerateName, type) { }

InputField::~InputField() { }

// -------------------------------------------------------------------------------------------------

void InputField::align_horizontal(AlignHorizontal value, PropertyDriver driver) {
	if (AccessProperty::setter(*this, property.align_horizontal, driver))
		return;

	text_.align(value);
}

AlignHorizontal InputField::align_horizontal() const noexcept {
	return text_.align();
}

void InputField::font(Font2D_view value, PropertyDriver driver) {
	if (AccessProperty::setter(*this, property.font, driver))
		return;

	text_.font(std::move(value));
}

const Font2D_view& InputField::font() const noexcept {
	return text_.font();
}

void InputField::font_size(FontSize value, PropertyDriver driver) {
	if (AccessProperty::setter(*this, property.font_size, driver))
		return;

	text_.size(value);
}

FontSize InputField::font_size() const noexcept {
	return text_.size();
}

// -------------------------------------------------------------------------------------------------

void InputField::text(std::string value) {
	text_.string(std::move(value));
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	fire(EventChange{*this});
}

const std::string& InputField::text() const noexcept {
	return text_.string();
}

// -------------------------------------------------------------------------------------------------

bool InputField::onChar(const libv::input::EventChar& event) {
	text_.insert(caret, event.unicode);
//	text_.insert(caret, event.utf8.data());
//	text_.push_back(event.utf8.data());

	caret++;
	caretStartTime = clock::now();
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	fire(EventChange{*this});
	return true;
}

bool InputField::onKey(const libv::input::EventKey& event) {
	if (event.key == libv::input::Key::Backspace && event.action != libv::input::Action::release) {
		if (caret > 0) {
			text_.erase(caret - 1, 1);
			caret--;
		}
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventChange{*this});
		return true;
	}

	if (event.key == libv::input::Key::Delete && event.action != libv::input::Action::release) {
		if (caret < text_.length())
			text_.erase(caret, 1);

		// On delete caret does not moves
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventChange{*this});
		return true;
	}

	// === TEMP ========================================================================================

	if (event.key == libv::input::Key::Enter && event.action != libv::input::Action::release && (event.mods & libv::input::KeyModifier::shift) != libv::input::KeyModifier::none) {
		text_.insert(caret, '\n');

		caret++;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		return true;
	}

	if (event.key == libv::input::Key::Num1 && event.action == libv::input::Action::press)
		return align_horizontal(AlignHorizontal::Left), true;
	if (event.key == libv::input::Key::Num2 && event.action == libv::input::Action::press)
		return align_horizontal(AlignHorizontal::Center), true;
	if (event.key == libv::input::Key::Num3 && event.action == libv::input::Action::press)
		return align_horizontal(AlignHorizontal::Right), true;
	if (event.key == libv::input::Key::Num4 && event.action == libv::input::Action::press)
		return align_horizontal(AlignHorizontal::Justify), true;
	if (event.key == libv::input::Key::Num5 && event.action == libv::input::Action::press)
		return align_horizontal(AlignHorizontal::JustifyAll), true;

	if (event.key == libv::input::Key::Num6 && event.action == libv::input::Action::press)
		return font(context().font("Achafexp.ttf")), true;
	if (event.key == libv::input::Key::Num7 && event.action == libv::input::Action::press)
		return font(context().font("consola.ttf")), true;

	if (event.key == libv::input::Key::Num8 && event.action == libv::input::Action::press)
		return font_size(libv::ui::FontSize(libv::to_value(font_size()) + 3)), true;
	if (event.key == libv::input::Key::Num9 && event.action == libv::input::Action::press)
		return font_size(libv::ui::FontSize(libv::to_value(font_size()) - 3)), true;

	// =================================================================================================

	if (event.key == libv::input::Key::Enter && event.action != libv::input::Action::release) {
		fire(EventSubmit{*this});
		return true;
	}

	if (event.key == libv::input::Key::KPEnter && event.action != libv::input::Action::release) {
		fire(EventSubmit{*this});
		return true;
	}

	if (event.key == libv::input::Key::C && event.action != libv::input::Action::release && (event.mods & libv::input::KeyModifier::control) != libv::input::KeyModifier::none) {
		context().clipboardText(text_.string());

		caretStartTime = clock::now();
		return true;
	}

	if (event.key == libv::input::Key::V && event.action != libv::input::Action::release && (event.mods & libv::input::KeyModifier::control) != libv::input::KeyModifier::none) {
		const auto clip = context().clipboardText();
		caret += static_cast<uint32_t>(text_.insert(caret, clip));

		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventChange{*this});
		return true;
	}

	if (event.key == libv::input::Key::X && event.action != libv::input::Action::release && (event.mods & libv::input::KeyModifier::control) != libv::input::KeyModifier::none) {
		context().clipboardText(text_.string());
		text_.clear();

		caret = 0;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventChange{*this});
		return true;
	}

	if (event.key == libv::input::Key::Left && event.action != libv::input::Action::release) {
		if (caret > 0)
			caret--;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		return true;
	}

	if (event.key == libv::input::Key::Right&& event.action != libv::input::Action::release) {
		if (caret < text_.length())
			caret++;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		return true;
	}

	if (event.key == libv::input::Key::Home&& event.action != libv::input::Action::release) {
		caret = 0;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		return true;
	}

	if (event.key == libv::input::Key::End&& event.action != libv::input::Action::release) {
		caret = static_cast<uint32_t>(text_.length());
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		return true;
	}

	return false;
}

void InputField::onFocus(const EventFocus& event) {
	if (event.loss()) {
		// TODO P5: Set style to normal or disabled
		flagAuto(Flag::pendingRender);
	}

	if (event.gain()) {
		// TODO P5: Set style to active if not disabled

		// TODO P5: Implement FocusSelectPolicy property, half is in onFocus, half is onMouseButton
//		property.focus_select_policy = FocusSelectPolicy::caretAtCursorOrEnd;
//		if (property.focus_select_policy == FocusSelectPolicy::caretAtCursorOrEnd)
//		caret = 0; // Begin
//		caret = caret; // History
		caret = static_cast<uint32_t>(text_.length()); // End
		// caret = do not change if focus was initiated by cursor to retain caret placement by the cursor | NOPE, focus event should happen from inside mouse button, so after focus() call simple caret set and we done

		caretStartTime = clock::now();
		flagAuto(Flag::pendingRender | Flag::pendingLayout);
	}
}

bool InputField::onMouseButton(const EventMouseButton& event) {
	if (event.action == libv::input::Action::press)
		focus();

//	caret = text_.getClosestCharacterIndex(position() - event.universe.mousePosition);

	return true;
}

bool InputField::onMouseMovement(const EventMouseMovement& event) {
	if (event.enter)
		set(property.bg_color, property.bg_color() + 0.2f);
		// TODO P5: Set style to hover if not disabled and updates layout properties in parent

	if (event.leave)
		reset(property.bg_color);
		// TODO P5: Set style to hover if not disabled and updates layout properties in parent

	return true;
}

bool InputField::onMouseScroll(const EventMouseScroll& event) {
	(void) event;
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
	PropertySetterContext<InputField> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
}

void InputField::doLayout1(const ContextLayout1& environment) {
	(void) environment;

	const auto contentString = text_.content(-1, -1);
	const auto contentImage = libv::vec::cast<float>(property.bg_image()->size());

	AccessLayout::lastDynamic(*this) = {libv::vec::max(contentString, contentImage), 0.f};
}

void InputField::doLayout2(const ContextLayout2& environment) {
	text_.limit(libv::vec::xy(environment.size));
	caretPosition = text_.getCharacterPosition(caret);
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
		caret_mesh.clear();
		auto pos = caret_mesh.attribute(attribute_position);
		auto index = caret_mesh.index();

		const auto caretHeight = font()->getLineAdvance(font_size());
		const auto max = context().settings.caret_width_max;
		const auto min = context().settings.caret_width_min;
		const auto offset = context().settings.caret_width_offset;
		const auto scale = context().settings.caret_width_scale;
		const auto caretWidth = std::floor(std::min((caretHeight - offset) / scale + min, max));

		// 3-2
		// |/|
		// 0-1
		pos(0, 0, 0);
		pos(caretWidth, 0, 0);
		pos(caretWidth, caretHeight, 0);
		pos(0, caretHeight, 0);

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

	const auto caret_flash_iteration = time_mod(ctx.now - caretStartTime, context().settings.caret_flash_period);

	if (isFocused() && caret_flash_iteration < context().settings.caret_show_period) {
		ctx.gl.model.translate({caretPosition, 0});

		ctx.gl.program(*property.caret_shader());
		ctx.gl.uniform(property.caret_shader()->uniform_color, property.caret_color());
		ctx.gl.uniform(property.caret_shader()->uniform_MVPmat, ctx.gl.mvp());
		ctx.gl.render(caret_mesh);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
