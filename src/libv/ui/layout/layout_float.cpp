// File:   layout_grid.cpp Author: Vader Created on 01 August 2019, 03:58

// hpp
#include <libv/ui/layout/layout_float.hpp>
// libv
#include <libv/utility/approx.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/min_max.hpp>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/context_layout.hpp>
#include <libv/ui/layout/view_layouted.lpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

libv::vec3f LayoutFloat::layout1(
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

		libv::meta::for_constexpr<0, 3>([&](auto i) {
			result[i] = libv::max(
					result[i],
					resolvePercent(
							child.property.size()[i].pixel + (child.property.size()[i].dynamic ? AccessLayout::lastDynamic(*child.ptr)[i] : 0.f),
							child.property.size()[i].percent, *child.ptr)
			);
		});
	}

	return result;
}

void LayoutFloat::layout2(
		const ContextLayout2& environment,
		libv::span<Child> children,
		const Properties& property,
		const BaseComponent& parent) {

	(void) parent;
	(void) property;

	for (const auto& child : children | view_layouted()) {

		// Size ---

		auto size = libv::vec3f{};
		libv::meta::for_constexpr<0, 3>([&](auto i) {
			const auto has_ratio = child.property.size()[i].ratio != 0.f;

			if (has_ratio)
				size[i] = environment.size[i];
			else
				size[i] =
						child.property.size()[i].pixel +
						child.property.size()[i].percent * 0.01f * environment.size[i] +
						(child.property.size()[i].dynamic ? AccessLayout::lastDynamic(*child.ptr)[i] : 0.f);
		});

		// Position ---

		const auto position =
				+ environment.position
				+ child.property.anchor_parent() * environment.size
				- child.property.anchor_target() * size;

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + size) - roundedPosition;

		AccessLayout::layout2(
				*child.ptr,
				ContextLayout2{
					roundedPosition,
					roundedSize,
					MouseOrder{libv::to_value(environment.mouseOrder) + 1}
				}
		);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
