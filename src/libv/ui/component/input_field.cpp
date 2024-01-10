// Project: libv.ui, File: src/libv/ui/component/input_field.cpp

// hpp
#include <libv/ui/component/input_field.hpp>
// libv
#include <libv/sys/clipboard.hpp>
// pro
#include <libv/ui/chrono.hpp>
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component/layout/layout_text.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_resource.hpp> // Only debug actions
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_system/property_access.hpp>
#include <libv/ui/resource/font_2D.hpp>
#include <libv/ui/resource/shader_font.hpp>
#include <libv/ui/resource/shader_quad.hpp>
#include <libv/ui/settings.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreInputField : CoreComponent {
	using base_type = CoreComponent;
	using base_type::base_type;

	// TODO P5: Remove handler()
	[[nodiscard]] inline auto handler() { return InputField{this}; }

public:
	struct Properties {
		PropertyB<FocusSelectPolicy> focus_select_policy;

		PropertyR<Background> background;

		PropertyR<Color> caret_color;
		PropertyR<ShaderQuad_view> caret_shader;

		PropertyR<Color> font_color;
		PropertyR<ShaderFont_view> font_shader;

		PropertyL2<> align_horizontal;
		PropertyL2<> align_vertical;
		PropertyL1L2<> font;
		PropertyL1L2<> font_size;
	} property;

	template <typename T> static void access_properties(T& ctx);

public:
	LayoutText text_;

	time_point caretStartTime;
	libv::vec2f caretPosition;
	uint32_t caret = 0; /// 0 = Before the first character, n = Before the nth character, length() = After the last character

public:
	virtual void onChar(const EventChar& event) override;
	virtual void onKey(const EventKey& event) override;
	virtual void onFocus(const EventFocus& event) override;
	virtual void onMouseButton(const EventMouseButton& event) override;
	virtual void onMouseMovement(const EventMouseMovement& event) override;
	virtual void onMouseScroll(const EventMouseScroll& event) override;

public:
	virtual	libv::vec4f getInnerContentBounds() override;

public:
	virtual void doAttach() override;
	virtual void doStyle(StyleAccess& access) override;
	virtual libv::vec2f doLayout1(const ContextLayout1& environment) override;
	virtual void doLayout2(const ContextLayout2& environment) override;
	virtual void doRender(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreInputField::access_properties(T& ctx) {
//	ctx.property(
//			[](auto& c) -> auto& { return c.property.focus_select_policy; },
//			FocusSelectPolicy::caretAtCursorOrEnd,
//			pgr::behaviour, pnm::focus_select_policy,
//			"Focus select policy"
//	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.background; },
			Background::color({0.9f, 0.9f, 1.f, 1.f}),
			pgr::appearance, pnm::background,
			"Background"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.caret_color; },
			Color(0, 0, 0, 1),
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
			[](auto& c, auto v) { c.text_.align_horizontal(v); },
			[](const auto& c) { return c.text_.align_horizontal(); },
			AlignHorizontal::left,
			pgr::appearance, pnm::align_horizontal,
			"Horizontal alignment of the text"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.align_vertical; },
			[](auto& c, auto v) { c.text_.align_vertical(v); },
			[](const auto& c) { return c.text_.align_vertical(); },
			AlignVertical::top,
			pgr::appearance, pnm::align_vertical,
			"Vertical alignment of the text"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.font_color; },
			Color(0, 0, 0, 1),
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
			[](auto& c, auto v) { c.text_.font(std::move(v)); },
			[](const auto& c) { return c.text_.font(); },
			[](auto& u) { return u.fallbackFont(); },
			pgr::font, pnm::font,
			"Font file"
	);
	ctx.indirect(
			[](auto& c) -> auto& { return c.property.font_size; },
			[](auto& c, auto v) { c.text_.size(v); },
			[](const auto& c) { return c.text_.size(); },
			FontSize{12},
			pgr::font, pnm::font_size,
			"Font size in pixel"
	);
	ctx.synthesize(
			T::handler_setter(&InputField::text),
			T::handler_getter(&InputField::text),
			pgr::behaviour, pnm::text,
			"Displayed text"
	);
	ctx.synthesize(
			[](auto& c, auto v) { c.handler().caret(v); },
			[](const auto& c) { return c.handler().caret(); },
			pgr::behaviour, pnm::caret,
			"Current zero indexed caret position"
	);
}

// -------------------------------------------------------------------------------------------------

void CoreInputField::onChar(const EventChar& event) {
	event.stop_propagation();
	text_.insert(caret, event.unicode);

	caret++;
	caretStartTime = clock::now();
	markInvalidLayout(true, false);
	flagAuto(Flag::pendingRender);
	fire(EventChange{});
	fire(EventCaret{});
}

void CoreInputField::onKey(const EventKey& event) {
	// TODO P2: libv.ui: better/simpler access to modifiers
	const auto shift = ui().state.key_pressed(libv::input::Keycode::ShiftLeft) || ui().state.key_pressed(libv::input::Keycode::ShiftRight);
	const auto ctrl = ui().state.key_pressed(libv::input::Keycode::ControlLeft) || ui().state.key_pressed(libv::input::Keycode::ControlRight);
	const auto alt = ui().state.key_pressed(libv::input::Keycode::AltLeft) || ui().state.key_pressed(libv::input::Keycode::AltRight);
	const auto pressOrRepeat = event.action != libv::input::Action::release;

	if (event.keycode == libv::input::Keycode::Backspace && pressOrRepeat) {
		if (caret > 0) {
			text_.erase(caret - 1, 1);
			caret--;
		}
		caretStartTime = clock::now();
		markInvalidLayout(true, false);
		flagAuto(Flag::pendingRender);
		fire(EventChange{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::Delete && pressOrRepeat) {
		if (caret < text_.length())
			text_.erase(caret, 1);

		// On delete caret does not moves
		caretStartTime = clock::now();
		markInvalidLayout(true, false);
		flagAuto(Flag::pendingRender);
		fire(EventChange{});
		return event.stop_propagation();
	}

	if (shift && event.keycode == libv::input::Keycode::Enter && pressOrRepeat) {
		text_.insert(caret, '\n');

		caret++;
		caretStartTime = clock::now();
		markInvalidLayout(true, false);
		flagAuto(Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::Enter && pressOrRepeat) {
		fire(EventSubmit{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::KPEnter && pressOrRepeat) {
		fire(EventSubmit{});
		return event.stop_propagation();
	}

	if (ctrl && event.keycode == libv::input::Keycode::C && pressOrRepeat) {
		libv::sys::clipboard_text(text_.string());

		caretStartTime = clock::now();
		return event.stop_propagation();
	}

	if (ctrl && event.keycode == libv::input::Keycode::V && pressOrRepeat) {
		const auto clip = libv::sys::clipboard_text();
		caret += static_cast<uint32_t>(text_.insert(caret, clip));

		caretStartTime = clock::now();
		markInvalidLayout(true, false);
		flagAuto(Flag::pendingRender);
		fire(EventChange{});
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (ctrl && event.keycode == libv::input::Keycode::X && pressOrRepeat) {
		libv::sys::clipboard_text(text_.string());
		text_.clear();

		caret = 0;
		caretStartTime = clock::now();
		markInvalidLayout(true, false);
		flagAuto(Flag::pendingRender);
		fire(EventChange{});
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::Left && pressOrRepeat) {
		if (caret > 0)
			caret--;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::Right && pressOrRepeat) {
		if (caret < text_.length())
			caret++;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::Home && pressOrRepeat) {
		caret = 0;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::End && pressOrRepeat) {
		caret = static_cast<uint32_t>(text_.length());
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	// Heuristic to detect and absorb Key events that could result in printable charachters while allowing
	// non-text related input propgation. This is not perfect and can never be perfect due to the funamental
	// nature of how charachter input works.
	// If this heuristic fails the only issue it generates that a printable keystroke maybe used by
	// others (like controls hotkey) and it could execute both an unrelated operation and an append of a new
	// character. As a last resort this can be worked around by the user pasting in the charachter and/or
	// remapping the controls.
	//                  | char ctrl alt  shft | Printable
	// A                | 1    0    0    *    | 1
	// F1               | 0    0    0    *    | 0
	// Ctrl F1          | 0    1    0    *    | 0
	// Ctrl S           | 1    1    0    *    | 0
	// Ctrl Alt J       | 1    1    1    0    | 1 (AltGr aka Ctrl+Alt used for alt charachters)
	// Ctrl Shift Alt A | 1    1    1    1    | 0
	if (pressOrRepeat && libv::input::isPrintableCharacterKey(event.keycode) && (!ctrl || (alt && !shift)))
		event.stop_propagation();
}

void CoreInputField::onFocus(const EventFocus& event) {
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
		fire(EventCaret{});
	}
}

void CoreInputField::onMouseButton(const EventMouseButton& event) {
	event.stop_propagation();

	if (!isFocused() && event.action == libv::input::Action::press)
		// NOTE: This focus causes a second EventCaret, could be fixed, but not really important
		focus(FocusMode::active);

	if (event.action == libv::input::Action::press) {
		caret = static_cast<uint32_t>(text_.getClosestCharacterIndexInline(event.local_position - padding_LB()));
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
	}
}

void CoreInputField::onMouseMovement(const EventMouseMovement& event) {
	event.stop_propagation();

	// if (event.enter)
	// 	set(property.bg_color, property.bg_color() + 0.2f);
	// 	// TODO P5: Set style to hover if not disabled and updates layout properties in parent
	//
	// if (event.leave)
	// 	set(property.bg_color, property.bg_color() - 0.2f);
	// 	reset(property.bg_color);
	// 	// TODO P5: Set style to hover if not disabled and updates layout properties in parent

	// // === TEMP ========================================================================================
	// if (ui().state.key_pressed(libv::input::Keycode::F1)) {
	// 	caret = static_cast<uint32_t>(text_.getClosestCharacterIndex(event.local_position - padding_LB()));
	// 	caretStartTime = clock::now();
	// 	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	// 	fire(EventCaret{});
	// }
	// if (ui().state.key_pressed(libv::input::Keycode::F2)) {
	// 	caret = static_cast<uint32_t>(text_.getClosestCharacterIndexInline(event.local_position - padding_LB()));
	// 	caretStartTime = clock::now();
	// 	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	// 	fire(EventCaret{});
	// }
	// // =================================================================================================
}

void CoreInputField::onMouseScroll(const EventMouseScroll& event) {
	event.stop_propagation();
}

// -------------------------------------------------------------------------------------------------

libv::vec4f CoreInputField::getInnerContentBounds() {
	return {padding_LB() + text_.content_bounding_pos(), text_.content_bounding_size()};
}

// -------------------------------------------------------------------------------------------------

void CoreInputField::doAttach() {
	watchChar(true);
	watchKey(true);
	watchFocus(true);
	watchMouse(true);
}

void CoreInputField::doStyle(StyleAccess& access) {
	access.self(*this);
}

libv::vec2f CoreInputField::doLayout1(const ContextLayout1& layoutEnv) {
	const auto dynamic_size_text = text_.content(
				layoutEnv.minusOneIfUnlimited(layoutEnv.limit - padding_size())
			) + padding_size();

	return dynamic_size_text;
}

void CoreInputField::doLayout2(const ContextLayout2& environment) {
	text_.limit(environment.size - padding_size());
	caretPosition = text_.getCharacterPosition(caret);
}

void CoreInputField::doRender(Renderer& r) {
	property.background().render(r, {0, 0}, layout_size(), *this);

	r.text(padding_LB(), text_,
			property.font_color(),
			text_.font(),
			property.font_shader());

	const auto caret_flash_iteration = time_mod(r.time_frame() - caretStartTime, ui().settings.caret_flash_period);

	if (isFocused() && caret_flash_iteration < ui().settings.caret_show_period) {
		const auto lineHeight = text_.font()->getLineAdvance(text_.size());
		const auto max = ui().settings.caret_width_max;
		const auto min = ui().settings.caret_width_min;
		const auto offset = ui().settings.caret_width_offset;
		const auto scale = ui().settings.caret_width_scale;
		const auto caretWidth = std::floor(std::clamp((lineHeight + offset) / scale, min, max));

		r.quad(padding_LB() + caretPosition, {caretWidth, lineHeight},
				property.caret_color(),
				property.caret_shader());
	}
}

// =================================================================================================

core_ptr InputField::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool InputField::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void InputField::background(Background value) {
	AccessProperty::manual(self(), self().property.background, std::move(value));
}

[[nodiscard]] const Background& InputField::background() const noexcept {
	return self().property.background();
}

// -------------------------------------------------------------------------------------------------

void InputField::align_horizontal(AlignHorizontal value) {
	AccessProperty::setter(self(), self().property.align_horizontal, PropertyDriver::manual, [&]() {
		self().text_.align_horizontal(value);
	});
}

AlignHorizontal InputField::align_horizontal() const noexcept {
	return self().text_.align_horizontal();
}

void InputField::align_vertical(AlignVertical value) {
	AccessProperty::setter(self(), self().property.align_vertical, PropertyDriver::manual, [&]() {
		 self().text_.align_vertical(value);
	});
}

AlignVertical InputField::align_vertical() const noexcept {
	return self().text_.align_vertical();
}

void InputField::font(Font2D_view value) {
	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
		self().text_.font(std::move(value));
	});
}

const Font2D_view& InputField::font() const noexcept {
	return self().text_.font();
}

void InputField::font_size(FontSize value) {
	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
		self().text_.size(value);
	});
}

FontSize InputField::font_size() const noexcept {
	return self().text_.size();
}

void InputField::font_color(Color value) {
	AccessProperty::manual(self(), self().property.font_color, value);
}

const Color& InputField::font_color() const noexcept {
	return self().property.font_color();
}

// -------------------------------------------------------------------------------------------------

void InputField::text(std::string value) {
	self().text_.string(std::move(value));
	self().markInvalidLayout(true, false);
	self().flagAuto(Flag::pendingRender);
	self().fire(EventChange{});
	if (self().caret > self().text_.length()) {
		self().caret = static_cast<uint32_t>(self().text_.length());
		self().fire(EventCaret{});
	}
}

const std::string& InputField::text() const noexcept {
	return self().text_.string();
}

void InputField::caret(uint32_t value) {
	self().caret = value;
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
	self().fire(EventCaret{});
}

uint32_t InputField::caret() const noexcept {
	return self().caret;
}

void InputField::focus_select_policy(FocusSelectPolicy value) {
	AccessProperty::manual(self(), self().property.focus_select_policy, std::move(value));
}

FocusSelectPolicy InputField::focus_select_policy() const noexcept {
	return self().property.focus_select_policy();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
