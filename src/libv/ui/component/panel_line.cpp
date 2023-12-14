// Project: libv.ui, File: src/libv/ui/component/panel_line.cpp

// hpp
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/panel_line_core.hpp>
// libv
#include <libv/container/small_vector.hpp>
// std
#include <algorithm>
// pro
#include <libv/ui/component/base_panel_core.hpp>
#include <libv/ui/component/layout/layout_slc.hpp>
#include <libv/ui/property_system/property_access.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename Access>
void CorePanelLine::access_layout(Access&& access) {
	using ChildNode = SLC_Margin<SLC_Component>;
	libv::small_vector<ChildNode, 32> entries;
	entries.reserve(children.size());
	for (auto& child : children)
		if (child.core().isLayouted())
			entries.emplace_back(SLC_Margin(child.margin_extent(), SLC_Component(child)));

	access(
			SLC_Padding(this->padding_extent(),
				SLC_LineDynamic<ChildNode>(property.spacing(), property.orientation(), property.align_horizontal(), property.align_vertical(), entries)
			)
	);
}

// -------------------------------------------------------------------------------------------------

void CorePanelLine::doStyle(StyleAccess& access) {
	access.self(*this);
}

void CorePanelLine::doStyleChild(StyleAccess& access, ChildID childID) {
	(void) access;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec2f CorePanelLine::doLayout1(const ContextLayout1& layoutEnv) {
	return layoutSLCCorePass1(*this, layoutEnv);
}

void CorePanelLine::doLayout2(const ContextLayout2& layoutEnv) {
	layoutSLCCorePass2(*this, layoutEnv);
}

// =================================================================================================

core_ptr PanelLine::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool PanelLine::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

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

void PanelLine::spacing(Spacing value) {
	AccessProperty::manual(self(), self().property.spacing, value);
}

Spacing PanelLine::spacing() const noexcept {
	return self().property.spacing();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
