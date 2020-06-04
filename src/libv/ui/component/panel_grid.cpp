// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/panel_grid.hpp>
// pro
#include <libv/ui/component/basic_panel.lpp>
#include <libv/ui/layout/layout_grid.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelGrid : public CoreBasicPanel<LayoutGrid> {
	friend class PanelGrid;

public:
	using CoreBasicPanel<LayoutGrid>::CoreBasicPanel;
};

// -------------------------------------------------------------------------------------------------

PanelGrid::PanelGrid(std::string name) :
	ComponenetHandler<CorePanelGrid, EventHostGeneral<PanelGrid>>(std::move(name)) { }

PanelGrid::PanelGrid(GenerateName_t gen, const std::string_view type) :
	ComponenetHandler<CorePanelGrid, EventHostGeneral<PanelGrid>>(gen, type) { }

PanelGrid::PanelGrid(base_ptr core) noexcept :
	ComponenetHandler<CorePanelGrid, EventHostGeneral<PanelGrid>>(core) { }

// -------------------------------------------------------------------------------------------------

void PanelGrid::anchor_content(Anchor value) {
	AccessProperty::manual(self(), self().property.anchor_content, value);
}

Anchor PanelGrid::anchor_content() const noexcept{
	return self().property.anchor_content();
}

void PanelGrid::column_count(ColumnCount value) {
	AccessProperty::manual(self(), self().property.column_count, value);
}

ColumnCount PanelGrid::column_count() const noexcept{
	return self().property.column_count();
}

void PanelGrid::orientation2(Orientation2 value) {
	AccessProperty::manual(self(), self().property.orientation2, value);
}

Orientation2 PanelGrid::orientation2() const noexcept{
	return self().property.orientation2();
}

// -------------------------------------------------------------------------------------------------

void PanelGrid::add(Component component) {
	self().add(std::move(component));
}

void PanelGrid::remove(Component& component) {
	self().remove(component);
}

void PanelGrid::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
