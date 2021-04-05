// Project: libv.ui, File: src/libv/ui/component/input_field.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/input_field.hpp>
// pro
#include <libv/ui/chrono.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/core_component.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/shader/shader_quad.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/text_layout.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreInputField : public CoreComponent {
	friend class InputField;
	[[nodiscard]] inline auto handler() { return InputField{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyB<FocusSelectPolicy> focus_select_policy;

		PropertyR<Color> bg_color;
		PropertyL<Texture2D_view> bg_image;
		PropertyR<ShaderImage_view> bg_shader;

		PropertyR<Color> caret_color;
		PropertyR<ShaderQuad_view> caret_shader;

		PropertyR<Color> font_color;
		PropertyR<ShaderFont_view> font_shader;

		PropertyL<> align_horizontal;
		PropertyL<> align_vertical;
		PropertyL<> font;
		PropertyL<> font_size;
	} property;

private:
	TextLayout text_;

private:
	time_point caretStartTime;
	libv::vec2f caretPosition;
	uint32_t caret = 0; /// 0 = Before the first character, n = Before the nth character, length() = After the last character

public:
	using CoreComponent::CoreComponent;

private:
	virtual void onChar(const EventChar& event) override;
	virtual void onKey(const EventKey& event) override;
	virtual void onFocus(const EventFocus& event) override;
	virtual void onMouseButton(const EventMouseButton& event) override;
	virtual void onMouseMovement(const EventMouseMovement& event) override;
	virtual void onMouseScroll(const EventMouseScroll& event) override;

private:
	virtual void doAttach() override;
	virtual void doStyle(ContextStyle& context) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
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
	ctx.synthetize(
			[](auto& c, auto v) { c.handler().text(std::move(v)); },
			[](const auto& c) { return c.handler().text(); },
			pgr::behaviour, pnm::text,
			"Displayed text"
	);
	ctx.synthetize(
			[](auto& c, auto v) { c.handler().caret(v); },
			[](const auto& c) { return c.handler().caret(); },
			pgr::behaviour, pnm::caret,
			"Current zero indexed caret position"
	);
}

// -------------------------------------------------------------------------------------------------

void CoreInputField::onChar(const EventChar& event) {
	text_.insert(caret, event.unicode);

	caret++;
	caretStartTime = clock::now();
	flagAuto(Flag::pendingLayout | Flag::pendingRender);
	fire(EventChange{});
	fire(EventCaret{});
	event.stop_propagation();
}

void CoreInputField::onKey(const EventKey& event) {
	if (event.keycode == libv::input::Keycode::Backspace && event.action != libv::input::Action::release) {
		if (caret > 0) {
			text_.erase(caret - 1, 1);
			caret--;
		}
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventChange{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::Delete && event.action != libv::input::Action::release) {
		if (caret < text_.length())
			text_.erase(caret, 1);

		// On delete caret does not moves
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventChange{});
		return event.stop_propagation();
	}

	// TODO P2: libv.ui: better access to modifiers
	const auto shift = context().state.key_pressed(libv::input::Keycode::ShiftLeft) || context().state.key_pressed(libv::input::Keycode::ShiftRight);
	const auto ctrl = context().state.key_pressed(libv::input::Keycode::ControlLeft) || context().state.key_pressed(libv::input::Keycode::ControlRight);

	// === TEMP ========================================================================================

	if (shift && event.keycode == libv::input::Keycode::Enter && event.action != libv::input::Action::release) {
		text_.insert(caret, '\n');

		caret++;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (ctrl && event.keycode == libv::input::Keycode::Num1 && event.action == libv::input::Action::press)
		return handler().align_horizontal(AlignHorizontal::left), event.stop_propagation();
	if (ctrl && event.keycode == libv::input::Keycode::Num2 && event.action == libv::input::Action::press)
		return handler().align_horizontal(AlignHorizontal::center), event.stop_propagation();
	if (ctrl && event.keycode == libv::input::Keycode::Num3 && event.action == libv::input::Action::press)
		return handler().align_horizontal(AlignHorizontal::right), event.stop_propagation();
	if (ctrl && event.keycode == libv::input::Keycode::Num4 && event.action == libv::input::Action::press)
		return handler().align_horizontal(AlignHorizontal::justify), event.stop_propagation();
	if (ctrl && event.keycode == libv::input::Keycode::Num5 && event.action == libv::input::Action::press)
		return handler().align_horizontal(AlignHorizontal::justify_all), event.stop_propagation();

	if (ctrl && event.keycode == libv::input::Keycode::Num6 && event.action == libv::input::Action::press)
		return handler().font(context().font("Achafexp.ttf")), event.stop_propagation();
	if (ctrl && event.keycode == libv::input::Keycode::Num7 && event.action == libv::input::Action::press)
		return handler().font(context().font("consola.ttf")), event.stop_propagation();

	if (ctrl && event.keycode == libv::input::Keycode::Num8 && event.action == libv::input::Action::press)
		return handler().font_size(libv::ui::FontSize(libv::to_value(handler().font_size()) + 3)), event.stop_propagation();
	if (ctrl && event.keycode == libv::input::Keycode::Num9 && event.action == libv::input::Action::press)
		return handler().font_size(libv::ui::FontSize(libv::to_value(handler().font_size()) - 3)), event.stop_propagation();

	if (event.keycode == libv::input::Keycode::F1 && event.action == libv::input::Action::press) {
		const auto mouse_coord = context().state.mouse_position() - libv::vec::xy(layout_position());
		caret = static_cast<uint32_t>(text_.getClosestCharacterIndex(mouse_coord));
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}
	if (event.keycode == libv::input::Keycode::F2 && event.action == libv::input::Action::press) {
		const auto mouse_coord = context().state.mouse_position() - libv::vec::xy(layout_position());
		caret = static_cast<uint32_t>(text_.getClosestCharacterIndexInline(mouse_coord));
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	// =================================================================================================

	if (event.keycode == libv::input::Keycode::Enter && event.action != libv::input::Action::release) {
		fire(EventSubmit{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::KPEnter && event.action != libv::input::Action::release) {
		fire(EventSubmit{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::C && event.action != libv::input::Action::release && ctrl) {
		context().clipboardText(text_.string());

		caretStartTime = clock::now();
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::V && event.action != libv::input::Action::release && ctrl) {
		const auto clip = context().clipboardText();
		caret += static_cast<uint32_t>(text_.insert(caret, clip));

		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventChange{});
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::X && event.action != libv::input::Action::release && ctrl) {
		context().clipboardText(text_.string());
		text_.clear();

		caret = 0;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventChange{});
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::Left && event.action != libv::input::Action::release) {
		if (caret > 0)
			caret--;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::Right&& event.action != libv::input::Action::release) {
		if (caret < text_.length())
			caret++;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::Home&& event.action != libv::input::Action::release) {
		caret = 0;
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}

	if (event.keycode == libv::input::Keycode::End&& event.action != libv::input::Action::release) {
		caret = static_cast<uint32_t>(text_.length());
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
		return event.stop_propagation();
	}
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
		focus();

	if (event.action == libv::input::Action::press) {
		caret = static_cast<uint32_t>(text_.getClosestCharacterIndexInline(event.local_position));
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
	}
}

void CoreInputField::onMouseMovement(const EventMouseMovement& event) {
	if (event.enter)
		set(property.bg_color, property.bg_color() + 0.2f);
		// TODO P5: Set style to hover if not disabled and updates layout properties in parent

	if (event.leave)
		set(property.bg_color, property.bg_color() - 0.2f);
//		reset(property.bg_color);
		// TODO P5: Set style to hover if not disabled and updates layout properties in parent

	// === TEMP ========================================================================================
	if (context().state.key_pressed(libv::input::Keycode::F1)) {
		caret = static_cast<uint32_t>(text_.getClosestCharacterIndex(event.local_position));
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
	}
	if (context().state.key_pressed(libv::input::Keycode::F2)) {
		caret = static_cast<uint32_t>(text_.getClosestCharacterIndexInline(event.local_position));
		caretStartTime = clock::now();
		flagAuto(Flag::pendingLayout | Flag::pendingRender);
		fire(EventCaret{});
	}
	// =================================================================================================

	event.stop_propagation();
}

void CoreInputField::onMouseScroll(const EventMouseScroll& event) {
	event.stop_propagation();
}

// -------------------------------------------------------------------------------------------------

void CoreInputField::doAttach() {
	watchChar(true);
	watchKey(true);
	watchFocus(true);
	watchMouse(true);
}

void CoreInputField::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreInputField> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

libv::vec3f CoreInputField::doLayout1(const ContextLayout1& environment) {
	const auto dynamic_size_text = text_.content(xy(environment.size) - padding_size()) + padding_size();
//	const auto dynamic_size_image = property.bg_image()->size().cast<float>();

//	return {libv::vec::max(dynamic_size_text, dynamic_size_image), 0.f};
	return {dynamic_size_text, 0.f};
}

void CoreInputField::doLayout2(const ContextLayout2& environment) {
	text_.limit(xy(environment.size) - padding_size());
	caretPosition = text_.getCharacterPosition(caret);
}

void CoreInputField::doRender(Renderer& r) {
	r.texture_2D({0, 0}, layout_size2(), {0, 0}, {1, 1},
			property.bg_color(),
			property.bg_image(),
			property.bg_shader());

	r.text(padding_LB(), text_,
			property.font_color(),
			text_.font(),
			property.font_shader());

	const auto caret_flash_iteration = time_mod(r.time_frame() - caretStartTime, context().settings.caret_flash_period);

	if (isFocused() && caret_flash_iteration < context().settings.caret_show_period) {
		const auto lineHeight = text_.font()->getLineAdvance(text_.size());
		const auto max = context().settings.caret_width_max;
		const auto min = context().settings.caret_width_min;
		const auto offset = context().settings.caret_width_offset;
		const auto scale = context().settings.caret_width_scale;
		const auto caretWidth = std::floor(std::clamp((lineHeight + offset) / scale, min, max));

		r.quad(caretPosition, {caretWidth, lineHeight},
				property.caret_color(),
				property.caret_shader());
	}
}

// -------------------------------------------------------------------------------------------------

InputField::InputField(std::string name) :
	ComponentHandler<CoreInputField, EventHostEditable<InputField>>(std::move(name)) { }

InputField::InputField(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CoreInputField, EventHostEditable<InputField>>(gen, type) { }

InputField::InputField(core_ptr core) noexcept :
	ComponentHandler<CoreInputField, EventHostEditable<InputField>>(core) { }

// -------------------------------------------------------------------------------------------------

void InputField::color(Color value) {
	AccessProperty::manual(self(), self().property.bg_color, value);
}

const Color& InputField::color() const noexcept {
	return self().property.bg_color();
}

void InputField::image(Texture2D_view value) {
	AccessProperty::manual(self(), self().property.bg_image, std::move(value));
}

const Texture2D_view& InputField::image() const noexcept {
	return self().property.bg_image();
}

void InputField::shader(ShaderImage_view value) {
	AccessProperty::manual(self(), self().property.bg_shader, std::move(value));
}

const ShaderImage_view& InputField::shader() const noexcept {
	return self().property.bg_shader();
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
	self().flagAuto(Flag::pendingLayout | Flag::pendingRender);
	self().fire(EventChange{});
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
