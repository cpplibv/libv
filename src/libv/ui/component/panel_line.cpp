// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/panel_line.hpp>
// pro
#include <libv/ui/component/basic_panel.lpp>
#include <libv/ui/layout/layout_line.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelLine : public CoreBasicPanel<LayoutLine> {
	friend class PanelLine;

public:
	using CoreBasicPanel<LayoutLine>::CoreBasicPanel;
};

// -------------------------------------------------------------------------------------------------

PanelLine::PanelLine(std::string name) :
	ComponenetHandler<CorePanelLine, EventHostGeneral<PanelLine>>(std::move(name)) { }

PanelLine::PanelLine(GenerateName_t gen, const std::string_view type) :
	ComponenetHandler<CorePanelLine, EventHostGeneral<PanelLine>>(gen, type) { }

PanelLine::PanelLine(base_ptr core) noexcept :
	ComponenetHandler<CorePanelLine, EventHostGeneral<PanelLine>>(core) { }

// -------------------------------------------------------------------------------------------------

void PanelLine::align_horizontal(AlignHorizontal value) {
	AccessProperty::manual(self(), self().property.align_horizontal, value);
}

AlignHorizontal PanelLine::align_horizontal() const noexcept {
	return self().property.align_horizontal();
}

void PanelLine::align_vertical(AlignVertical value) {
	AccessProperty::manual(self(), self().property.align_vertical, value);
}

AlignVertical PanelLine::align_vertical() const noexcept {
	return self().property.align_vertical();
}

void PanelLine::orientation(Orientation value) {
	AccessProperty::manual(self(), self().property.orientation, value);
}

Orientation PanelLine::orientation() const noexcept {
	return self().property.orientation();
}

// -------------------------------------------------------------------------------------------------

void PanelLine::add(Component component) {
	self().add(std::move(component));
}

void PanelLine::remove(Component& component) {
	self().remove(component);
}

void PanelLine::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
