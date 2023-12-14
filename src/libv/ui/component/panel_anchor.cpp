// Project: libv.ui, File: src/libv/ui/component/panel_anchor.cpp

// hpp
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_anchor_core.hpp>
// pro
#include <libv/ui/component/base_panel_core.hpp>
#include <libv/ui/component/layout/layout_utility.hpp>
#include <libv/ui/component/layout/view_layouted.hxx>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_system/property_access.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void CorePanelAnchor::doStyle(StyleAccess& access) {
	access.self(*this);
}

void CorePanelAnchor::doStyleChild(StyleAccess& access, ChildID childID) {
	(void) access;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec2f CorePanelAnchor::doLayout1(const ContextLayout1& layoutEnv) {
	const auto contentLimit = layoutEnv.limit - padding_size();
	const auto contentParent = layoutEnv.parent - padding_size();
	auto result = libv::vec2f{};

	for (auto& child : children | view_layouted()) {
		const auto childMargin = child.core().margin_size();
		const auto childLimit = contentLimit - childMargin;
		const auto childParent = contentParent - childMargin;
		const auto childUnlimited = layoutEnv.unlimited && child.size().dynamic2();
		const auto childEnv = ContextLayout1{childUnlimited, childLimit, childParent};
		const auto childDynamic = child.size().has_dynamic() ? AccessLayout::layout1(child.core(), childEnv) : libv::vec2f{};
		const auto childEvalSize = child.size().eval(childDynamic, childParent) + childMargin;

		result = libv::max(result, childEvalSize);
	}

	return result + padding_size();
}

void CorePanelAnchor::doLayout2(const ContextLayout2& layoutEnv) {
	const auto contentAreaSize = layoutEnv.size - padding_size();
	const auto contentLimit = contentAreaSize;
	const auto contentParent = contentAreaSize;

	for (auto& child : children | view_layouted()) {

		// Size ---

		const auto childMargin = child.core().margin_size();
		const auto childLimit = contentLimit - childMargin;
		const auto childParent = contentParent - childMargin;
		const auto childEnv = ContextLayout1{{false, false}, childLimit, childParent};
		const auto childDynamic = child.size().has_dynamic() ? AccessLayout::layout1(child.core(), childEnv) : libv::vec2f{};
		const auto childEvalSize = child.size().eval(childDynamic, childParent);
		const auto childRatioShare = child.size().ratio_mask2() * (childParent - childEvalSize);

		const auto childSize = childEvalSize + childRatioShare;

		// Position ---

		const auto position =
				+ padding_LB()
				+ child.core().margin_LB()
				+ info(child.anchor()).rate() * contentAreaSize
				- info(child.anchor()).rate() * (childSize + childMargin);

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + childSize) - roundedPosition;

		AccessLayout::layout2(
				child.core(),
				layoutEnv.enter(roundedPosition, roundedSize)
		);
	}
}

// =================================================================================================

core_ptr PanelAnchor::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool PanelAnchor::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void PanelAnchor::snap_to_edge(SnapToEdge value) {
	AccessProperty::manual(self(), self().property.snap_to_edge, value);
}

SnapToEdge PanelAnchor::snap_to_edge() const noexcept {
	return self().property.snap_to_edge();
}

void PanelAnchor::squish(Squish value) {
	AccessProperty::manual(self(), self().property.squish, value);
}

Squish PanelAnchor::squish() const noexcept {
	return self().property.squish();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
