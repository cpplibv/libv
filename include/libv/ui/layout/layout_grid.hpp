// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/span.hpp>
// std
#include <memory>
// pro
#include <libv/ui/property.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextLayout1;
class ContextLayout2;
class ContextStyle;
class BaseComponent;

struct LayoutGrid {
public:
	struct Properties {
		PropertyL<Anchor> anchor_content;
		PropertyL<ColumnCount> column_count;
		PropertyL<Orientation2> orientation2;
	};

	template <typename T>
	static void access_properties(T& ctx);
//	static ComponentPropertyDescription description;

	struct ChildProperties {
		PropertyL<Anchor> anchor;
		PropertyL<Size> size;
	};

	template <typename T>
	static void access_child_properties(T& ctx);
//	static ComponentPropertyDescription child_description;

public:
	struct Child {
		ChildProperties property;
		std::shared_ptr<BaseComponent> ptr;

		Child(std::shared_ptr<BaseComponent> ptr) : ptr(std::move(ptr)) {}
	};

public:
	static void style(Properties& properties, ContextStyle& ctx);
	static void style(ChildProperties& properties, ContextStyle& ctx);

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
