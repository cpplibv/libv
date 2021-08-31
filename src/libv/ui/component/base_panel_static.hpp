// Project: libv.ui, File: src/libv/ui/component/base_panel_static.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <vector>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/component/detail/component.hpp>



// hpp
#include <libv/ui/component/base_panel.lpp>
// libv
#include <libv/algo/erase_if_stable.hpp>
// pro
#include <libv/ui/context/context_focus_traverse.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/log.hpp>
//#include <libv/ui/context/context_style.hpp>
//#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------
// =================================================================================================

struct CoreTemplate {
	template <typename T>
	void access_properties(T& ctx);
	template <typename T>
	void access_components(T& ctx);
};

// =================================================================================================
// -------------------------------------------------------------------------------------------------

template <typename CRTP>
class CoreBasePanelStatic : public CoreComponent {
protected:
	std::vector<Component> children;

public:
	using CoreComponent::CoreComponent;
	virtual ~CoreBasePanelStatic() noexcept = default;

public:
	void add(Component component);
	void remove(Component& component);
	void clear();

private:
	virtual void doRender(Renderer& r) override;
	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual libv::observer_ptr<CoreComponent> doFocusTraverse(const ContextFocusTraverse& context, ChildID current) override;
	virtual void doForeachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doForeachChildren(libv::function_ref<void(Component&)> callback) override;

//	virtual void doStyle(ContextStyle& ctx) override;
//	virtual void doStyle(ContextStyle& ctx, ChildID childID) override;

//	void doStyle(ContextStyle& ctx) {
//		PropertyAccessContext<Properties> setter{property, ctx.component, ctx.style, ctx.component.context()};
//		access_properties(setter);
//		CoreComponent::doStyle(ctx);
//	}
//
//void CorePanelStatusLine::doStyle(ContextStyle& ctx, ChildID childID) {
//	(void) ctx;
//	(void) childID;
//}
};

// -------------------------------------------------------------------------------------------------

template <typename CRTP>
void CoreBasePanelStatic<CRTP>::add(Component component) {
	const auto childID = static_cast<ChildID>(children.size());
	AccessParent::childID(component.core()) = childID;

	children.emplace_back(std::move(component));
	// NOTE: LayoutSelf is necessary to make container layout the new child into the correct place
	flagForce(Flag::pendingAttachChild | Flag::pendingLayoutSelf);
}

template <typename CRTP>
void CoreBasePanelStatic<CRTP>::remove(Component& component) {
	if (children[AccessParent::childID(component.core())] != component) {
		log_ui.error("Attempted to remove a non child element: {} from: {}", component.path(), path());
		return;
	}

	component.markRemove();
}

template <typename CRTP>
void CoreBasePanelStatic<CRTP>::clear() {
	for (auto& child : children)
		child.markRemove();
}

// -------------------------------------------------------------------------------------------------

template <typename CRTP>
void CoreBasePanelStatic<CRTP>::doRender(Renderer& r) {
	for (auto& child : children) {
		Renderer rc = r.enter(child);
		AccessParent::render(child.core(), rc);
	}
}

template <typename CRTP>
void CoreBasePanelStatic<CRTP>::doDetachChildren(libv::function_ref<bool(Component&)> callback) {
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

template <typename CRTP>
libv::observer_ptr<CoreComponent> CoreBasePanelStatic<CRTP>::doFocusTraverse(const ContextFocusTraverse& context, ChildID current) {
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

template <typename CRTP>
void CoreBasePanelStatic<CRTP>::doForeachChildren(libv::function_ref<bool(Component&)> callback) {
	for (auto& child : children)
		if (not callback(child))
			return;
}

template <typename CRTP>
void CoreBasePanelStatic<CRTP>::doForeachChildren(libv::function_ref<void(Component&)> callback) {
	for (auto& child : children)
		callback(child);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
