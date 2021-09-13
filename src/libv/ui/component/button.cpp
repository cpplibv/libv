// Project: libv.ui, File: src/libv/ui/component/button.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/button_core.hpp>
// pro
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/property_access_context.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/text_layout.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void CoreButton::onFocus(const EventFocus& event) {
	(void) event;
	// TODO P3: listen to hotkey event (ui.select)

	if (event.loss())
		{} // Set style to normal or disabled

	if (event.gain())
		{} // Set style to active if not disabled

	flagAuto(Flag::pendingRender);
}

void CoreButton::onMouseButton(const EventMouseButton& event) {
	if (event.action == libv::input::Action::press)
		focus();

	fire(EventMouseButton{event});

	// TODO P3: use hotkey event (ui.select) (even for mouse)
	if (event.action == libv::input::Action::press && event.button == libv::input::MouseButton::Left)
		fire(EventSubmit{});

	event.stop_propagation();
}

void CoreButton::onMouseMovement(const EventMouseMovement& event) {
//	if (event.enter)
//		set(property.bg_color, property.bg_color() + 0.2f);
//		// TODO P5: Set style to hover if not disabled and updates layout properties in parent
//
//	if (event.leave)
////		reset(property.bg_color);
//		set(property.bg_color, property.bg_color() - 0.2f);
//		// TODO P5: Set style to hover if not disabled and updates layout properties in parent

	fire(EventMouseMovement{event});

	event.stop_propagation();
}

void CoreButton::onMouseScroll(const EventMouseScroll& event) {
	fire(EventMouseScroll{event});

	event.stop_propagation();
}

// -------------------------------------------------------------------------------------------------

libv::vec4f CoreButton::getInnerContentBounds() {
	return {padding_LB() + text_.content_bounding_pos(), text_.content_bounding_size()};
}

// -------------------------------------------------------------------------------------------------

void CoreButton::doAttach() {
	watchFocus(true);
	watchMouse(true);
}

void CoreButton::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreButton> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::doStyle(ctx);
}

libv::vec3f CoreButton::doLayout1(const ContextLayout1& environment) {
	const auto dynamic_size_text = text_.content(xy(environment.size) - padding_size()) + padding_size();
//	const auto dynamic_size_image = property.bg_image()->size().cast<float>();

//	return {libv::vec::max(dynamic_size_text, dynamic_size_image), 0.f};
	return {dynamic_size_text, 0.f};
}

void CoreButton::doLayout2(const ContextLayout2& environment) {
	text_.limit(xy(environment.size) - padding_size());
}

void CoreButton::doRender(Renderer& r) {
	property.background().render(r, {0, 0}, layout_size2(), *this);

	r.text(padding_LB(), text_,
			property.font_color(),
			text_.font(),
			property.font_shader());
}

// =================================================================================================

core_ptr Button::create_core(std::string name) {
	return create_core_ptr<CoreButton>(std::move(name));
}

// -------------------------------------------------------------------------------------------------

void Button::background(Background value) {
	AccessProperty::manual(self(), self().property.background, std::move(value));
}

[[nodiscard]] const Background& Button::background() const noexcept {
	return self().property.background();
}

// -------------------------------------------------------------------------------------------------

void Button::align_horizontal(AlignHorizontal value) {
	AccessProperty::setter(self(), self().property.align_horizontal, PropertyDriver::manual, [&]() {
		self().text_.align_horizontal(value);
	});
}

AlignHorizontal Button::align_horizontal() const noexcept {
	return self().text_.align_horizontal();
}

void Button::align_vertical(AlignVertical value) {
	AccessProperty::setter(self(), self().property.align_vertical, PropertyDriver::manual, [&]() {
		 self().text_.align_vertical(value);
	});
}

AlignVertical Button::align_vertical() const noexcept {
	return self().text_.align_vertical();
}

void Button::font(Font2D_view value) {
	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
		self().text_.font(std::move(value));
	});
}

const Font2D_view& Button::font() const noexcept {
	return self().text_.font();
}

void Button::font_size(FontSize value) {
	AccessProperty::setter(self(), self().property.font, PropertyDriver::manual, [&]() {
		self().text_.size(value);
	});
}

FontSize Button::font_size() const noexcept {
	return self().text_.size();
}

void Button::font_color(Color value) {
	AccessProperty::manual(self(), self().property.font_color, value);
}

const Color& Button::font_color() const noexcept {
	return self().property.font_color();
}

// -------------------------------------------------------------------------------------------------

void Button::text(std::string value) {
	self().text_.string(std::move(value));
	self().markInvalidLayout(true, false);
	self().flagAuto(Flag::pendingRender);
}

const std::string& Button::text() const noexcept {
	return self().text_.string();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
