// Project: libv.ui, File: src/libv/ui/component/toggle_button.cpp

// hpp
#include <libv/ui/component/toggle_button.hpp>
#include <libv/ui/component/toggle_button_core.hpp>
// pro
#include <libv/ui/component/layout/layout_text.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/property_system/property_access.hpp>
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

libv::vec2f CoreToggleButton::doLayout1(const ContextLayout1& layoutEnv) {
	const auto dynamic_size_text_on = text_on.measure_content_size(
			layoutEnv.minusOneIfUnlimited(layoutEnv.limit - padding_size()),
			CoreButton::property.font(), CoreButton::property.font_size()
		) + padding_size();

	const auto dynamic_size_text_off = text_off.measure_content_size(
			layoutEnv.minusOneIfUnlimited(layoutEnv.limit - padding_size()),
			CoreButton::property.font(), CoreButton::property.font_size()
		) + padding_size();

	return libv::vec::max(dynamic_size_text_on, dynamic_size_text_off);
}

void CoreToggleButton::doLayout2(const ContextLayout2& environment) {
	text_on.invalidateLayout2(environment.size - padding_size());
	text_off.invalidateLayout2(environment.size - padding_size());
}

void CoreToggleButton::doRender(Renderer& r) {
	CoreButton::property.background().render(r, {0, 0}, layout_size(), *this);

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

core_ptr ToggleButton::create_core(std::string name, std::string text) {
	auto p = create_core_ptr<CoreType>(std::move(name));
	p->text_on.string(text);
	p->text_off.string(std::move(text));
	return p;
}

core_ptr ToggleButton::create_core(std::string name, std::string textOn, std::string textOff) {
	auto p = create_core_ptr<CoreType>(std::move(name));
	p->text_on.string(std::move(textOn));
	p->text_off.string(std::move(textOff));
	return p;
}

bool ToggleButton::castable(core_ptr core) noexcept {
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
	if (self().property.selection() == value)
		return;
	style_state(StyleState::select, value);
	AccessProperty::manual(self(), self().property.selection, value);
	// TODO P4: Broadcast select/submit event
}

void ToggleButton::select_silent(bool value) noexcept {
	if (self().property.selection() == value)
		return;
	style_state(StyleState::select, value);
	AccessProperty::manual(self(), self().property.selection, value);
}

bool ToggleButton::select() const noexcept {
	return self().property.selection();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
