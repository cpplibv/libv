// Project: libv.ui, File: src/libv/ui/component/button.cpp

// hpp
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/button_core.hpp>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component/layout/layout_text.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_resource.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/property_system/property_access.hpp>
#include <libv/ui/resource/font_2D.hpp>
#include <libv/ui/resource/shader_font.hpp>
#include <libv/ui/resource/shader_image.hpp>
#include <libv/ui/style/style_state.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void CoreButton::routineSubmit() {
	EventSubmit event{};
	fire(event);
	if (!event.submit_rejected())
		onSubmit();
}

void CoreButton::onFocus(const EventFocus& event) {
	(void) event;
	// TODO P3: listen to hotkey event (ui.select) (? aka libv.ctrl enter context)

	style_state(StyleState::focus, event.gain());
}

void CoreButton::onMouseButton(const EventMouseButton& event) {
	if (event.action == libv::input::Action::release && event.button == libv::input::MouseButton::Left)
		style_state(StyleState::active, false);

	if (event.action == libv::input::Action::press)
		focus();

	fire(EventMouseButton{event});

	// TODO P3: use hotkey event (ui.select) (even for mouse)
	if (event.action == libv::input::Action::press && event.button == libv::input::MouseButton::Left) {
		style_state(StyleState::active, true);
		routineSubmit();
	}

	event.stop_propagation();
}

void CoreButton::onMouseMovement(const EventMouseMovement& event) {
	if (event.enter)
		style_state(StyleState::hover, true);

	if (event.leave)
		style_state(StyleState::hover, false);

	// TODO P1: style_state(StyleState::active, false); after the mouse leaves the component area (while it was active): maybe it has to acquire or soft acquire the mouse? so it can track the release (ergo deactivate) event
	//		style_state(StyleState::active, false);

	// TODO P5: Set style to hover if not disabled and updates layout properties in parent
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

void CoreButton::doStyle(StyleAccess& access) {
	access.self(*this);
}

libv::vec3f CoreButton::doLayout1(const ContextLayout1& environment) {
	const auto dynamic_size_text = text_.measure_content_size(
				xy(environment.size) - padding_size(),
				property.font(), property.font_size()
			) + padding_size();

//	const auto dynamic_size_text = text_.content(xy(environment.size) - padding_size()) + padding_size();
//	const auto dynamic_size_image = property.bg_image()->size().cast<float>();

//	return {libv::vec::max(dynamic_size_text, dynamic_size_image), 0.f};
	return {dynamic_size_text, 0.f};
}

void CoreButton::doLayout2(const ContextLayout2& environment) {
	text_.invalidateLayout2(xy(environment.size) - padding_size());
//	text_.layout(xy(environment.size) - padding_size(), property.font(), property.font_size(), property.align_horizontal(), property.align_vertical());
}

void CoreButton::doRender(Renderer& r) {
	property.background().render(r, {0, 0}, layout_size2(), *this);

	r.text(padding_LB(),
//			text_.vertices_data(),
			text_.vertices_data(property.font(), property.font_size(), property.align_horizontal(), property.align_vertical()),
			property.font(),
			property.font_color(),
			property.font_shader());
}

// =================================================================================================

core_ptr Button::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

core_ptr Button::create_core(std::string name, std::string text) {
	auto p = create_core_ptr<CoreType>(std::move(name));
	p->text_.string(std::move(text));
	return p;
}

bool Button::castable(core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
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
	AccessProperty::manual(self(), self().property.align_horizontal, std::move(value));
}

AlignHorizontal Button::align_horizontal() const noexcept {
	return self().property.align_horizontal();
}

void Button::align_vertical(AlignVertical value) {
	AccessProperty::manual(self(), self().property.align_vertical, std::move(value));
}

AlignVertical Button::align_vertical() const noexcept {
	return self().property.align_vertical();
}

void Button::font(Font2D_view value) {
	AccessProperty::manual(self(), self().property.font, std::move(value));
}

const Font2D_view& Button::font() const noexcept {
	return self().property.font();
}

void Button::font_size(FontSize value) {
	AccessProperty::manual(self(), self().property.font_size, std::move(value));
}

FontSize Button::font_size() const noexcept {
	return self().property.font_size();
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
