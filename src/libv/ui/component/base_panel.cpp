// File: label.cpp - Created on 2017.11.05. 07:52 - Author: Vader

// hpp
#include <libv/ui/component/base_panel.lpp>
// libv
#include <libv/algorithm/erase_if_stable.hpp>
// pro
#include <libv/ui/context_focus_traverse.hpp>
#include <libv/ui/log.hpp>
//#include <libv/ui/context_style.hpp>
//#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void CoreBasePanel::add(Component component) {
	const auto childID = static_cast<ChildID>(children.size());
	AccessParent::childID(component.core()) = childID;

	children.emplace_back(std::move(component));
	flagForce(Flag::pendingAttachChild);
}

void CoreBasePanel::remove(Component& component) {
	if (children[AccessParent::childID(component.core())] != component) {
		log_ui.error("Attempted to remove a non child element: {} from: {}", component.path(), path());
		return;
	}

	component.markRemove();
}

void CoreBasePanel::clear() {
	for (auto& child : children)
		child.markRemove();
}

// -------------------------------------------------------------------------------------------------

void CoreBasePanel::doDetachChildren(libv::function_ref<bool(CoreComponent&)> callback) {
	ChildID numRemoved = 0;

	libv::erase_if_stable(children, [&numRemoved, &callback](auto& child) {
		const bool remove = callback(child.core());

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
			return libv::make_observer(this);

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

void CoreBasePanel::doForeachChildren(libv::function_ref<bool(CoreComponent&)> callback) {
	for (auto& child : children)
		if (not callback(child.core()))
			return;
}

void CoreBasePanel::doForeachChildren(libv::function_ref<void(CoreComponent&)> callback) {
	for (auto& child : children)
		callback(child.core());
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
