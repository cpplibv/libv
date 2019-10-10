// File:   layout_grid.cpp Author: Vader Created on 01 August 2019, 03:58

// hpp
#include <libv/ui/layout/layout_full.hpp>
// libv
#include <libv/utility/approx.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/min_max.hpp>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/layout/view_layouted.lpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_access.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
void LayoutFull::access_properties(T& ctx) {
}

template <typename T>
void LayoutFull::access_child_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.size; },
			Size{},
			pgr::layout, pnm::size,
			"Component size in pixel, percent, ratio and dynamic units"
	);
}

// -------------------------------------------------------------------------------------------------

void LayoutFull::style(Properties& properties, ContextStyle& ctx) {
	PropertySetterContext<Properties> setter{properties, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
}

void LayoutFull::style(ChildProperties& properties, ContextStyle& ctx) {
	PropertySetterContext<ChildProperties> setter{properties, ctx.component, ctx.style, ctx.component.context()};
	access_child_properties(setter);
}

// -------------------------------------------------------------------------------------------------

libv::vec3f LayoutFull::layout1(
		const ContextLayout1& environment,
		libv::span<Child> children,
		const Properties& property,
		const BaseComponent& parent) {

	(void) environment;
	(void) parent;
	(void) property;

	const auto resolvePercent = [](const float fix, const float percent, auto& component) {
		if (fix == libv::Approx(0.f)) {
			return fix;
		} else if (percent == libv::Approx(100.f)) {
			log_ui.warn("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
			return fix * 2.f;
		} else {
			return fix / (1.f - percent * 0.01f);
		}
	};

	auto result = libv::vec3f{};

	for (const auto& child : children | view_layouted()) {
		AccessLayout::layout1(*child.ptr, ContextLayout1{});
		libv::meta::for_constexpr<0, 3>([&](auto dim) {
			result[dim] = libv::max(
					result[dim],
					resolvePercent(
							child.property.size()[dim].pixel + (child.property.size()[dim].dynamic ? AccessLayout::lastDynamic(*child.ptr)[dim] : 0.f),
							child.property.size()[dim].percent, *child.ptr)
			);
		});
	}

	return result;
}

void LayoutFull::layout2(
		const ContextLayout2& environment,
		libv::span<Child> children,
		const Properties& property,
		const BaseComponent& parent) {

	(void) parent;
	(void) property;

	for (const auto& child : children | view_layouted()) {
		AccessLayout::layout2(
				*child.ptr,
				ContextLayout2{
					environment.position,
					environment.size,
					MouseOrder{libv::to_value(environment.mouseOrder) + 1}
				}
		);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
