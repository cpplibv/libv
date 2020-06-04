// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/panel_float.hpp>
// pro
#include <libv/ui/component/basic_panel.lpp>
#include <libv/ui/layout/layout_float.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelFloat : public CoreBasicPanel<LayoutFloat> {
	friend class PanelFloat;

public:
	using CoreBasicPanel<LayoutFloat>::CoreBasicPanel;
};

// -------------------------------------------------------------------------------------------------

PanelFloat::PanelFloat(std::string name) :
	ComponenetHandler<CorePanelFloat, EventHostGeneral<PanelFloat>>(std::move(name)) { }

PanelFloat::PanelFloat(GenerateName_t gen, const std::string_view type) :
	ComponenetHandler<CorePanelFloat, EventHostGeneral<PanelFloat>>(gen, type) { }

PanelFloat::PanelFloat(base_ptr core) noexcept :
	ComponenetHandler<CorePanelFloat, EventHostGeneral<PanelFloat>>(core) { }

// -------------------------------------------------------------------------------------------------

void PanelFloat::snap_to_edge(SnapToEdge value) {
	AccessProperty::manual(self(), self().property.snap_to_edge, value);
}

SnapToEdge PanelFloat::snap_to_edge() const noexcept {
	return self().property.snap_to_edge();
}

void PanelFloat::squish(Squish value) {
	AccessProperty::manual(self(), self().property.squish, value);
}

Squish PanelFloat::squish() const noexcept {
	return self().property.squish();
}

// -------------------------------------------------------------------------------------------------

void PanelFloat::add(Component component) {
	self().add(std::move(component));
}

void PanelFloat::remove(Component& component) {
	self().remove(component);
}

void PanelFloat::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
