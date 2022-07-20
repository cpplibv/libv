// Project: libv.ui, File: src/libv/ui/component/panel_board.cpp

// hpp
#include <libv/ui/component/panel_board.hpp>
#include <libv/ui/component/panel_board_core.hpp>
// libv
#include <libv/algo/erase_if_stable.hpp>
// std
#include <algorithm>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/component/layout/layout_utility.hxx>
#include <libv/ui/component/layout/view_layouted.hxx>
#include <libv/ui/context/context_focus_traverse.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property.hpp>
#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void CorePanelBoard::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CorePanelBoard> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::doStyle(ctx);
}

void CorePanelBoard::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

void CorePanelBoard::doRender(Renderer& r) {
	property.background().render(r, {0, 0}, layout_size2(), *this);

	for (auto& child : children) {
		Renderer rc = r.enter(child.component);
		AccessParent::render(child.component.core(), rc);
	}
}

void CorePanelBoard::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
	ChildID numRemoved = 0;

	libv::erase_if_stable(children, [&numRemoved, &callback](auto& child) {
		const bool remove = callback(child.component);

		if (remove)
			++numRemoved;
		else
			AccessParent::childID(child.component.core()) -= numRemoved;

		return remove;
	});
}

libv::observer_ptr<CoreComponent> CorePanelBoard::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
	const ChildID dir = context.isForward() ? +1 : -1;
	const ChildID end = context.isForward() ? static_cast<ChildID>(children.size()) : -1;
	ChildID begin = context.isForward() ? 0 : static_cast<ChildID>(children.size() - 1);

	if (current == ChildIDNone) {
		// unrelated component is focused, focus self or iterate every children
		if (AccessParent::isFocusableComponent(*this))
			return libv::make_observer_ptr(this);

	} else if (current == ChildIDSelf) {
		// this component itself is currently focused, iterate every children

	} else {
		// one of the children is currently focused, iterate remaining children
		begin = current + dir;
	}

	if (!AccessParent::isFocusableChild(*this))
		return nullptr;

	for (ChildID i = begin; i != end; i += dir)
		if (auto hit = AccessParent::doFocusTraverse(children[i].component.core(), context, ChildIDNone))
			return hit;

	return nullptr;
}

void CorePanelBoard::doForeachChildren(libv::function_ref<bool(Component&)> callback) {
	for (auto& child : children)
		if (not callback(child.component))
			return;
}

void CorePanelBoard::doForeachChildren(libv::function_ref<void(Component&)> callback) {
	for (auto& child : children)
		callback(child.component);
}

// -------------------------------------------------------------------------------------------------

libv::vec3f CorePanelBoard::doLayout1(const ContextLayout1& layout_env) {
	(void) layout_env;

	libv::vec2f LB{};
	libv::vec2f RT{};

	for (const auto& child : children) {
		LB = libv::min(LB, child.position - child.component.padding_LB());
		RT = libv::max(RT, child.position + child.size + child.component.padding_RT());
	}

	const auto contentSize = RT - LB;

	return libv::vec3f{contentSize + margin_size(), 0.f};
}

void CorePanelBoard::doLayout2(const ContextLayout2& layout_env) {
	for (auto& child : children) {
		const auto position = margin_LB() + child.position;
		const auto roundedPosition = libv::vec::round(position);
		const auto roundedSize = libv::vec::round(position + child.size) - roundedPosition;

		AccessLayout::layout2(
				child.component.core(),
				layout_env.enter(libv::vec3f{roundedPosition, 0.f}, libv::vec3f{roundedSize, 0.f})
		);
	}
}

// =================================================================================================

core_ptr PanelBoard::create_core(std::string name) {
	return create_core_ptr<CorePanelBoard>(std::move(name));
}

// -------------------------------------------------------------------------------------------------

void PanelBoard::background(Background value) {
	AccessProperty::manual(self(), self().property.background, std::move(value));
}

[[nodiscard]] const Background& PanelBoard::background() const noexcept {
	return self().property.background();
}

// -------------------------------------------------------------------------------------------------

void PanelBoard::add(Component component, libv::vec2f position, libv::vec2f size) {
	const auto childID = static_cast<ChildID>(self().children.size());
	AccessParent::childID(component.core()) = childID;

	self().children.emplace_back(std::move(component), position, size);
	self().flagForce(Flag::pendingAttachChild | Flag::pendingLayoutSelf);
}

void PanelBoard::remove(Component& component) {
	const auto id = AccessParent::childID(component.core());
	if (id >= static_cast<ChildID>(self().children.size()) || self().children[id].component != component) {
		log_ui.error("Attempted to remove a non child element: {} from: {}", component.path(), path());
		return;
	}

	component.markRemove();
}

void PanelBoard::remove(std::string_view component_name) {
	int match_count = 0;

	for (auto& child : self().children)
		if (child.component.name() == component_name) {
			child.component.markRemove();
			++match_count;
		}

	log_ui.error_if(match_count == 0, "Attempted to remove a non child element by name: {} from: {}", component_name, path());
}

void PanelBoard::clear() {
	for (auto& child : self().children)
		child.component.markRemove();
}

//void PanelBoard::align_horizontal(AlignHorizontal value) {
//	AccessProperty::manual(self(), self().property.align_horizontal, value);
//}
//
//AlignHorizontal PanelBoard::align_horizontal() const noexcept {
//	return self().property.align_horizontal();
//}
//
//void PanelBoard::align_vertical(AlignVertical value) {
//	AccessProperty::manual(self(), self().property.align_vertical, value);
//}
//
//AlignVertical PanelBoard::align_vertical() const noexcept {
//	return self().property.align_vertical();
//}
//
//void PanelBoard::orientation(Orientation value) {
//	AccessProperty::manual(self(), self().property.orientation, value);
//}
//
//Orientation PanelBoard::orientation() const noexcept {
//	return self().property.orientation();
//}
//
//void PanelBoard::spacing(Spacing value) {
//	AccessProperty::manual(self(), self().property.spacing, value);
//}
//
//Spacing PanelBoard::spacing() const noexcept {
//	return self().property.spacing();
//}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
