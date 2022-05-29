// Project: libv.ui, File: src/libv/ui/component/panel_anchor.cpp

// hpp
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_anchor_core.hpp>
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

void CorePanelAnchor::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<Properties> setter{property, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
	CoreBasePanel::doStyle(ctx);
}

void CorePanelAnchor::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec3f CorePanelAnchor::doLayout1(const ContextLayout1& layout_env) {
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

void CorePanelAnchor::doLayout2(const ContextLayout2& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	for (auto& child : children | view_layouted()) {

		// Size ---

		auto size = libv::vec3f{};

		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child.core(), ContextLayout1{env_size}) :
				libv::vec3f{};

		libv::meta::for_constexpr<0, 3>([&](auto i) {
			const auto has_ratio = child.size()[i].ratio != 0.f;

			if (has_ratio)
				size[i] = env_size[i] - child.margin_size3()[i];
			else
				size[i] =
						child.size()[i].pixel +
						child.size()[i].percent * 0.01f * env_size[i] +
						(child.size()[i].dynamic ? child_dynamic[i] : 0.f);
		});

		// Position ---

		const auto position =
				+ padding_LB3()
				+ child.core().margin_LB3()
				+ info(child.anchor()).rate() * env_size
				- info(child.anchor()).rate() * (size + child.core().margin_size3());

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + size) - roundedPosition;

		AccessLayout::layout2(
				child.core(),
				layout_env.enter(roundedPosition, roundedSize)
		);
	}
}

// =================================================================================================

core_ptr PanelAnchor::create_core(std::string name) {
	return create_core_ptr<CorePanelAnchor>(std::move(name));
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
