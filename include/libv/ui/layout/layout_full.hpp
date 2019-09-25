// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
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

struct LayoutFull {
	struct PS {
		template <typename T>
		void access(T& ctx) {
			(void) ctx;
		}
	};

	struct ChildPS {
		static constexpr Flag_t::value_type L  = (Flag::pendingLayout).value();

		Property<Size, L, pnm::size> size;

		template <typename T>
		void access(T& ctx) {
			ctx(size, "Component size in pixel, percent, ratio and dynamic units");
		}
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
