// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/utility/span.hpp>
// std
#include <memory>
// pro
#include <libv/ui/property.hpp>
#include <libv/ui/property_set.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextLayout1;
class ContextLayout2;
class BaseComponent;

struct LayoutFloat {
	struct PS {
		// TODO P5: libv.ui: Implement Snap in float layout
		// TODO P5: libv.ui: Implement Squish in float layout
		PropertySnapToEdge snapToEdge;
		PropertySquish squish;

		LIBV_REFLECTION_ACCESS(snapToEdge);
		LIBV_REFLECTION_ACCESS(squish);
	};

	struct ChildPS {
		PropertySize size;
		PropertyAnchorParent anchor_parent;
		PropertyAnchorTarget anchor_target;

		LIBV_REFLECTION_ACCESS(size);
		LIBV_REFLECTION_ACCESS(anchor_parent);
		LIBV_REFLECTION_ACCESS(anchor_target);
	};

	struct Child {
		libv::ui::PropertySet<ChildPS> property;
		std::shared_ptr<BaseComponent> ptr;

		Child(std::shared_ptr<BaseComponent> ptr) : ptr(std::move(ptr)) {}
	};

	struct Properties : libv::ui::PropertySet<PS> {};

public:
	static libv::vec3f layout1(
			const ContextLayout1& environment,
			libv::span<Child> children,
			const Properties& property,
			const BaseComponent& parent);
	static void layout2(
			const ContextLayout2& environment,
			libv::span<Child> children,
			const Properties& property,
			const BaseComponent& parent);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
