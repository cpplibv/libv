// Project: libv.ui, File: src/libv/ui/component/toggle_button.cpp

// hpp
#include <libv/ui/component/toggle_button.hpp>
#include <libv/ui/component/toggle_button_core.hpp>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component/layout/layout_text.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
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

void CoreToggleButton::onSubmit() {
//	if (CoreButton::onSubmit())

	CoreButton::onSubmit();
	handler().toggle();
	//	ToggleButton{this}.toggle();
	// TODO P5: ToggleButton{this} = the old now missing handler(), need a better solution
}

// -------------------------------------------------------------------------------------------------

libv::vec4f CoreToggleButton::getInnerContentBounds() {
	return property.selection() ?
		libv::vec4f{padding_LB() + text_on.content_bounding_pos(), text_on.content_bounding_size()} :
		libv::vec4f{padding_LB() + text_off.content_bounding_pos(), text_off.content_bounding_size()};
}

// -------------------------------------------------------------------------------------------------

void CoreToggleButton::doStyle(StyleAccess& access) {
	access.self(*this);
}

libv::vec3f CoreToggleButton::doLayout1(const ContextLayout1& environment) {
	const auto dynamic_size_text_on = text_on.measure_content_size(
				xy(environment.size) - padding_size(),
				CoreButton::property.font(), CoreButton::property.font_size()
			) + padding_size();

	const auto dynamic_size_text_off = text_off.measure_content_size(
				xy(environment.size) - padding_size(),
				CoreButton::property.font(), CoreButton::property.font_size()
			) + padding_size();

	const auto dynamic_size_text = libv::vec::max(dynamic_size_text_on, dynamic_size_text_off);

	return {dynamic_size_text, 0.f};
}

void CoreToggleButton::doLayout2(const ContextLayout2& environment) {
	text_on.invalidateLayout2(xy(environment.size) - padding_size());
	text_off.invalidateLayout2(xy(environment.size) - padding_size());
}

void CoreToggleButton::doRender(Renderer& r) {
	CoreButton::property.background().render(r, {0, 0}, layout_size2(), *this);

	r.text(padding_LB(),
			property.selection() ?
				text_on.vertices_data(CoreButton::property.font(), CoreButton::property.font_size(), CoreButton::property.align_horizontal(), CoreButton::property.align_vertical()) :
				text_off.vertices_data(CoreButton::property.font(), CoreButton::property.font_size(), CoreButton::property.align_horizontal(), CoreButton::property.align_vertical()),
			CoreButton::property.font(),
			CoreButton::property.font_color(),
			CoreButton::property.font_shader());
}

// =================================================================================================

core_ptr ToggleButton::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool ToggleButton::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void ToggleButton::text_on(std::string value) {
	self().text_on.string(std::move(value));
	self().markInvalidLayout(true, false);
	self().flagAuto(Flag::pendingRender);
}

const std::string& ToggleButton::text_on() const noexcept {
	return self().text_on.string();
}

void ToggleButton::text_off(std::string value) {
	self().text_off.string(std::move(value));
	self().markInvalidLayout(true, false);
	self().flagAuto(Flag::pendingRender);
}

const std::string& ToggleButton::text_off() const noexcept {
	return self().text_off.string();
}

void ToggleButton::select(bool value) noexcept {
	style_state(libv::ui::StyleState::select, value);
	AccessProperty::manual(self(), self().property.selection, value);
}

bool ToggleButton::select() const noexcept {
	return self().property.selection();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
