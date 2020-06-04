// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/panel_full.hpp>
// pro
#include <libv/ui/component/basic_panel.lpp>
#include <libv/ui/layout/layout_full.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelFull : public CoreBasicPanel<LayoutFull> {
	friend class PanelFull;

public:
	using CoreBasicPanel<LayoutFull>::CoreBasicPanel;
};

// -------------------------------------------------------------------------------------------------

PanelFull::PanelFull(std::string name) :
	ComponenetHandler<CorePanelFull, EventHostGeneral<PanelFull>>(std::move(name)) { }

PanelFull::PanelFull(GenerateName_t gen, const std::string_view type) :
	ComponenetHandler<CorePanelFull, EventHostGeneral<PanelFull>>(gen, type) { }

PanelFull::PanelFull(base_ptr core) noexcept :
	ComponenetHandler<CorePanelFull, EventHostGeneral<PanelFull>>(core) { }

// -------------------------------------------------------------------------------------------------

void PanelFull::add(Component component) {
	self().add(std::move(component));
}

void PanelFull::remove(Component& component) {
	self().remove(component);
}

void PanelFull::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
