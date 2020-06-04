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
#include <libv/ui/context_style.hpp>
#include <libv/ui/layout/view_layouted.lpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
void LayoutFloat::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.snap_to_edge; },
			SnapToEdge{false},
			pgr::layout, pnm::snap_to_edge,
			"Snap to edge any child that otherwise would hang out"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.squish; },
			Squish{false},
			pgr::layout, pnm::squish,
			"Squish any child that otherwise would hang out"
	);
}

template <typename T>
void LayoutFloat::access_child_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.anchor_parent; },
			ANCHOR_CENTER_CENTER,
			pgr::layout, pnm::anchor_parent,
			"Parent's floating anchor point"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.anchor_target; },
			ANCHOR_CENTER_CENTER,
			pgr::layout, pnm::anchor_target,
			"Child's floating anchor point"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.size; },
			Size{},
			pgr::layout, pnm::size,
			"Component size in pixel, percent, ratio and dynamic units"
	);
}

// -------------------------------------------------------------------------------------------------

void LayoutFloat::style(Properties& properties, ContextStyle& ctx) {
	PropertyAccessContext<Properties> setter{properties, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
}

void LayoutFloat::style(ChildProperties& properties, ContextStyle& ctx) {
	PropertyAccessContext<ChildProperties> setter{properties, ctx.component, ctx.style, ctx.component.context()};
	access_child_properties(setter);
}

// -------------------------------------------------------------------------------------------------

libv::vec3f LayoutFloat::layout1(
		const ContextLayout1& environment,
		std::span<Child> children,
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

	for (auto& child : children | view_layouted()) {
		AccessLayout::layout1(child.ptr.base(), ContextLayout1{});

		libv::meta::for_constexpr<0, 3>([&](auto i) {
			result[i] = libv::max(
					result[i],
					resolvePercent(
							child.property.size()[i].pixel + (child.property.size()[i].dynamic ? AccessLayout::lastDynamic(child.ptr.base())[i] : 0.f),
							child.property.size()[i].percent, child.ptr.base())
			);
		});
	}

	return result;
}

void LayoutFloat::layout2(
		const ContextLayout2& environment,
		std::span<Child> children,
		const Properties& property,
		const BaseComponent& parent) {

	(void) parent;
	(void) property;

	for (auto& child : children | view_layouted()) {

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
						(child.property.size()[i].dynamic ? AccessLayout::lastDynamic(child.ptr.base())[i] : 0.f);
		});

		// Position ---

		const auto position =
				+ environment.position
				+ child.property.anchor_parent() * environment.size
				- child.property.anchor_target() * size;

		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + size) - roundedPosition;

		AccessLayout::layout2(
				child.ptr.base(),
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
