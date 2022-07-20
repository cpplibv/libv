// Project: libv.ui, File: src/libv/ui/component/base_panel.cpp

// hpp
#include <libv/ui/component/base_panel.hpp>
#include <libv/ui/component/base_panel_core.hpp>
// libv
#include <libv/algo/erase_if_stable.hpp>
// pro
#include <libv/ui/context/context_focus_traverse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void CoreBasePanel::add(Component component) {
	const auto childID = static_cast<ChildID>(children.size());
	AccessParent::childID(component.core()) = childID;

	children.emplace_back(std::move(component));
	// NOTE: LayoutSelf is necessary to make container layout the new child into the correct place
	flagForce(Flag::pendingAttachChild | Flag::pendingLayoutSelf);
}

void CoreBasePanel::add_front(Component component) {
	children.emplace(children.begin(), std::move(component));

	// Reassign IDs
	int32_t id = 0;
	for (auto& child : children)
		AccessParent::childID(child.core()) = ChildID{id++};

	// NOTE: LayoutSelf is necessary to make container layout the new child into the correct place
	flagForce(Flag::pendingAttachChild | Flag::pendingLayoutSelf);
}

void CoreBasePanel::remove(Component& component) {
	const auto id = AccessParent::childID(component.core());
	if (id >= static_cast<ChildID>(children.size()) || children[id] != component) {
		log_ui.error("Attempted to remove a non child element: {} from: {}", component.path(), path());
		return;
	}

	component.markRemove();
}

void CoreBasePanel::remove(std::string_view component_name) {
	int match_count = 0;

	for (auto& child : children)
		if (child.name() == component_name) {
			child.markRemove();
			++match_count;
		}

	log_ui.error_if(match_count == 0, "Attempted to remove a non child element by name: {} from: {}", component_name, path());
}

void CoreBasePanel::clear() {
	for (auto& child : children)
		child.markRemove();
}

// -------------------------------------------------------------------------------------------------

void CoreBasePanel::doStyle(ContextStyle& ctx) {
	PropertyAccessContext<CoreBasePanel> setter{*this, ctx.component, ctx.style, context()};
	access_properties(setter);
	CoreComponent::doStyle(ctx);
}

void CoreBasePanel::doRender(Renderer& r) {
	property.background().render(r, {0, 0}, layout_size2(), *this);

	for (auto& child : children) {
		Renderer rc = r.enter(child);
		AccessParent::render(child.core(), rc);
	}
}

void CoreBasePanel::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
	ChildID numRemoved = 0;

	libv::erase_if_stable(children, [&numRemoved, &callback](auto& child) {
		const bool remove = callback(child);

		if (remove)
			++numRemoved;
		else
			AccessParent::childID(child.core()) -= numRemoved;

		return remove;
	});
}

libv::observer_ptr<CoreComponent> CoreBasePanel::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
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
		if (auto hit = AccessParent::doFocusTraverse(children[i].core(), context, ChildIDNone))
			return hit;

	return nullptr;
}

void CoreBasePanel::doForeachChildren(libv::function_ref<bool(Component&)> callback) {
	for (auto& child : children)
		if (not callback(child))
			return;
}

void CoreBasePanel::doForeachChildren(libv::function_ref<void(Component&)> callback) {
	for (auto& child : children)
		callback(child);
}

// =================================================================================================

core_ptr BasePanel::create_core(std::string name) {
	return create_core_ptr<CoreBasePanel>(std::move(name));
}

// -------------------------------------------------------------------------------------------------

void BasePanel::background(Background value) {
	AccessProperty::manual(self(), self().property.background, std::move(value));
}

[[nodiscard]] const Background& BasePanel::background() const noexcept {
	return self().property.background();
}

// -------------------------------------------------------------------------------------------------

void BasePanel::add(Component component) {
	self().add(std::move(component));
}

void BasePanel::add_front(Component component) {
	self().add_front(std::move(component));
}

void BasePanel::remove(Component& component) {
	self().remove(component);
}

void BasePanel::remove(std::string_view component_name) {
	self().remove(component_name);
}

void BasePanel::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
