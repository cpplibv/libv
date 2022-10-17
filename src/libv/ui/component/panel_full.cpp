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
#include <libv/ui/component/layout/layout_utility.hxx>
#include <libv/ui/component/layout/view_layouted.hxx>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void CorePanelFull::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CorePanelFull> setter{*this, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
	CoreBasePanel::doStyle(ctx);
}

void CorePanelFull::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec3f CorePanelFull::doLayout1(const ContextLayout1& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	auto result = libv::vec3f{};

	for (auto& child : children | view_layouted()) {
		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child.core(), ContextLayout1{env_size}) :
				libv::vec3f{};

		libv::meta::for_constexpr<0, 3>([&](auto i) {
			result[i] = libv::max(
					result[i],
					resolvePercent(
							child.size()[i].pixel + (child.size()[i].dynamic ? child_dynamic[i] : 0.f),
							child.size()[i].percent, child.core())
						+ child.core().margin_size3()[i]
			);
		});
	}

	return result + padding_size3();
}

void CorePanelFull::doLayout2(const ContextLayout2& layout_env) {
	for (auto& child : children | view_layouted()) {
		const auto env_size = layout_env.size - padding_size3() - child.core().margin_size3();
		const auto position = padding_LB3() + child.core().margin_LB3();

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + env_size) - roundedPosition;

		AccessLayout::layout2(
				child.core(),
				layout_env.enter(roundedPosition, roundedSize)
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
