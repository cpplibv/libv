// File:   layout_grid.cpp Author: Vader Created on 01 August 2019, 03:58

// hpp
#include <libv/ui/layout/layout_full.hpp>
// libv
#include <libv/utility/approxing.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/min_max.hpp>
// pro
#include <libv/ui/component_base.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/layout/view_layouted.lpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

libv::vec3f LayoutFull::layout1(
		const ContextLayout1& environment,
		libv::span<Child> children,
		const Properties& properties,
		const ComponentBase& parent) {

	(void) environment;
	(void) parent;
	(void) properties;

	const auto resolvePercent = [](const float fix, const float percent, auto& component) {
		if (fix == libv::Approxing(0.f)) {
			return fix;
		} else if (percent == libv::Approxing(100.f)) {
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
							child.properties.size()[dim].pixel + (child.properties.size()[dim].content ? AccessLayout::lastContent(*child.ptr)[dim] : 0.f),
							child.properties.size()[dim].percent, *child.ptr)
			);
		});
	}

	return result;
}

void LayoutFull::layout2(
		const ContextLayout2& environment,
		libv::span<Child> children,
		const Properties& properties,
		const ComponentBase& parent) {

	(void) parent;
	(void) properties;

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
