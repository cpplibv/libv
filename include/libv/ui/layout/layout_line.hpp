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

struct LayoutLine {
	struct PS {
		PropertyAlignHorizontal alignHorizontal;
		PropertyAlignVertical alignVertical;
		PropertyOrientation orientation;

		LIBV_REFLECTION_ACCESS(alignHorizontal);
		LIBV_REFLECTION_ACCESS(alignVertical);
		LIBV_REFLECTION_ACCESS(orientation);
	};

	struct ChildPS {
		PropertySize size;

		LIBV_REFLECTION_ACCESS(size);
	};

	struct Child {
		libv::ui::PropertySet<ChildPS> properties;
		std::shared_ptr<BaseComponent> ptr;

		Child(std::shared_ptr<BaseComponent> ptr) : ptr(std::move(ptr)) {}
	};

	struct Properties : libv::ui::PropertySet<PS> {};

public:
	static libv::vec3f layout1(
			const ContextLayout1& environment,
			libv::span<Child> children,
			const Properties& properties,
			const BaseComponent& parent);
	static void layout2(
			const ContextLayout2& environment,
			libv::span<Child> children,
			const Properties& properties,
			const BaseComponent& parent);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv