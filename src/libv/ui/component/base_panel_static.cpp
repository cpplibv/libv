// Project: libv.ui, File: src/libv/ui/component/base_panel_static.cpp

// hpp
#include <libv/ui/component/base_panel_static.hpp>
// libv
//#include <libv/algo/erase_if_stable.hpp>
// pro
//#include <libv/ui/context/context_focus_traverse.hpp>
//#include <libv/ui/context/context_render.hpp>
//#include <libv/ui/log.hpp>
//#include <libv/ui/context/context_style.hpp>
//#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//void CoreBasePanelStatic::add(Component component) {
//	const auto childID = static_cast<ChildID>(children.size());
//	AccessParent::childID(component.core()) = childID;
//
//	children.emplace_back(std::move(component));
//	// NOTE: LayoutSelf is necessary to make container layout the new child into the correct place
//	flagForce(Flag::pendingAttachChild | Flag::pendingLayoutSelf);
//}
//
//void CoreBasePanelStatic::remove(Component& component) {
//	if (children[AccessParent::childID(component.core())] != component) {
//		log_ui.error("Attempted to remove a non child element: {} from: {}", component.path(), path());
//		return;
//	}
//
//	component.markRemove();
//}
//
//void CoreBasePanelStatic::clear() {
//	for (auto& child : children)
//		child.markRemove();
//}
//
//// -------------------------------------------------------------------------------------------------
//
//void CoreBasePanelStatic::doRender(Renderer& r) {
//	for (auto& child : children) {
//		Renderer rc = r.enter(child);
//		AccessParent::render(child.core(), rc);
//	}
//}
//
//void CoreBasePanelStatic::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
//	ChildID numRemoved = 0;
//
//	libv::erase_if_stable(children, [&numRemoved, &callback](auto& child) {
//		const bool remove = callback(child);
//
//		if (remove)
//			++numRemoved;
//		else
//			AccessParent::childID(child.core()) -= numRemoved;
//
//		return remove;
//	});
//}
//
//libv::observer_ptr<CoreComponent> CoreBasePanelStatic::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
//	const ChildID dir = context.isForward() ? +1 : -1;
//	const ChildID end = context.isForward() ? static_cast<ChildID>(children.size()) : -1;
//	ChildID begin = context.isForward() ? 0 : static_cast<ChildID>(children.size() - 1);
//
//	if (current == ChildIDNone) {
//		// unrelated component is focused, focus self or iterate every children
//		if (AccessParent::isFocusableComponent(*this))
//			return libv::make_observer_ptr(this);
//
//	} else if (current == ChildIDSelf) {
//		// this component itself is currently focused, iterate every children
//
//	} else {
//		// one of the children is currently focused, iterate remaining children
//		begin = current + dir;
//	}
//
//	if (!AccessParent::isFocusableChild(*this))
//		return nullptr;
//
//	for (ChildID i = begin; i != end; i += dir)
//		if (auto hit = AccessParent::doFocusTraverse(children[i].core(), context, ChildIDNone))
//			return hit;
//
//	return nullptr;
//}
//
//void CoreBasePanelStatic::doForeachChildren(libv::function_ref<bool(Component&)> callback) {
//	for (auto& child : children)
//		if (not callback(child))
//			return;
//}
//
//void CoreBasePanelStatic::doForeachChildren(libv::function_ref<void(Component&)> callback) {
//	for (auto& child : children)
//		callback(child);
//}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
