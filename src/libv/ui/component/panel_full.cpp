// Project: libv.ui, File: src/libv/ui/component/panel_full.cpp

// hpp
#include <libv/ui/component/panel_full.hpp>
#include <libv/ui/component/panel_full_core.hpp>
// libv
#include <libv/meta/for_constexpr.hpp>
#include <libv/utility/min_max.hpp>
#include <libv/utility/to_underlying.hpp>
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

void CorePanelFull::doStyle(StyleAccess& access) {
	access.self(*this);
}

void CorePanelFull::doStyleChild(StyleAccess& access, ChildID childID) {
	(void) access;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec2f CorePanelFull::doLayout1(const ContextLayout1& layoutEnv) {
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

void CorePanelFull::doLayout2(const ContextLayout2& layoutEnv) {
	for (auto& child : children | view_layouted()) {
		const auto childSize = layoutEnv.size - padding_size() - child.core().margin_size();
		const auto position = padding_LB() + child.core().margin_LB();

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + childSize) - roundedPosition;

		AccessLayout::layout2(
				child.core(),
				layoutEnv.enter(roundedPosition, roundedSize)
		);
	}
}

// =================================================================================================

core_ptr PanelFull::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool PanelFull::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
