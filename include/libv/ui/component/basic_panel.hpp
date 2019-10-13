// File: layout_line.hpp Author: Vader Created on 2017. augusztus 26., 23:33

#pragma once

// libv
#include <libv/algorithm/erase_if_stable.hpp>
// std
#include <memory>
#include <string>
#include <string_view>
#include <vector>
// pro
#include <libv/ui/base_component.hpp>
#include <libv/ui/context_focus_travers.hpp>
#include <libv/ui/context_style.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename Layout>
class BasicPanel : public BaseComponent {
public:
	typename Layout::Properties property;
private:
	std::vector<typename Layout::Child> children;

public:
	explicit BasicPanel(ContextUI& context); // Root only constructor
	explicit BasicPanel(BaseComponent& parent);
	BasicPanel(BaseComponent& parent, std::string name);
	BasicPanel(BaseComponent& parent, GenerateName_t, const std::string_view type);
	virtual ~BasicPanel();

public:
	typename Layout::Child& add(std::shared_ptr<BaseComponent> component);
	void remove(const std::shared_ptr<BaseComponent>& component);
	void clear();

protected:
	virtual void doDetachChildren(libv::function_ref<bool(BaseComponent&)> callback) override;
	virtual void doStyle(ContextStyle& context) override;
	virtual void doStyle(ContextStyle& context, ChildID childID) override;
	virtual libv::observer_ptr<BaseComponent> doFocusTravers(const ContextFocusTravers& context, ChildID current) override;
	virtual void doLayout1(const ContextLayout1& le) override;
	virtual void doLayout2(const ContextLayout2& le) override;
	virtual void doForeachChildren(libv::function_ref<bool(BaseComponent&)> callback) override;
	virtual void doForeachChildren(libv::function_ref<void(BaseComponent&)> callback) override;
};

// -------------------------------------------------------------------------------------------------

template <typename Layout>
BasicPanel<Layout>::BasicPanel(ContextUI& context) :
	BaseComponent(context) { }

template <typename Layout>
BasicPanel<Layout>::BasicPanel(BaseComponent& parent) :
	BaseComponent(parent, GenerateName, "basic-panel") { }

template <typename Layout>
BasicPanel<Layout>::BasicPanel(BaseComponent& parent, std::string name) :
	BaseComponent(parent, std::move(name)) { }

template <typename Layout>
BasicPanel<Layout>::BasicPanel(BaseComponent& parent, GenerateName_t, const std::string_view type) :
	BaseComponent(parent, GenerateName, type) { }

template <typename Layout>
BasicPanel<Layout>::~BasicPanel() {
}

// -------------------------------------------------------------------------------------------------

template <typename Layout>
typename Layout::Child& BasicPanel<Layout>::add(std::shared_ptr<BaseComponent> component) {
	const auto childID = static_cast<ChildID>(children.size());
	auto& child = children.emplace_back(std::move(component));
	flagForce(Flag::pendingAttachChild);
	AccessParent::childID(*child.ptr) = childID;
	return child;
}

template <typename Layout>
void BasicPanel<Layout>::remove(const std::shared_ptr<BaseComponent>& component) {
	if (*(children.begin() + AccessParent::childID(*component)) != component) {
		log_ui.error("Attempted to remove a non child element: {} from: {}", component->path(), path());
		return;
	}

	component->markRemove();
}

template <typename Layout>
void BasicPanel<Layout>::clear() {
	for (const auto& child : children)
		child.ptr->markRemove();
}

// -------------------------------------------------------------------------------------------------

template <typename Layout>
void BasicPanel<Layout>::doDetachChildren(libv::function_ref<bool(BaseComponent&)> callback) {
	ChildID numRemoved = 0;

	libv::erase_if_stable(children, [&numRemoved, &callback](auto& child) {
		const bool remove = callback(*child.ptr);

		if (remove)
			++numRemoved;
		else
			AccessParent::childID(*child.ptr) -= numRemoved;

		return remove;
	});
}

template <typename Layout>
void BasicPanel<Layout>::doStyle(ContextStyle& ctx) {
	Layout::style(property, ctx);
}

template <typename Layout>
void BasicPanel<Layout>::doStyle(ContextStyle& ctx, ChildID childID) {
	Layout::style(children[childID].property, ctx);
}

template <typename Layout>
libv::observer_ptr<BaseComponent> BasicPanel<Layout>::doFocusTravers(const ContextFocusTravers& context, ChildID current) {
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
		if (auto hit = AccessParent::doFocusTravers(*children[i].ptr, context, ChildIDNone))
			return hit;

	return nullptr;
}

template <typename Layout>
void BasicPanel<Layout>::doLayout1(const ContextLayout1& le) {
	AccessLayout::lastDynamic(*this) = Layout::layout1(le, children, property, *this);
}

template <typename Layout>
void BasicPanel<Layout>::doLayout2(const ContextLayout2& le) {
	Layout::layout2(le, children, property, *this);
}

template <typename Layout>
void BasicPanel<Layout>::doForeachChildren(libv::function_ref<bool(BaseComponent&)> callback) {
	for (const auto& child : children)
		if (not callback(*child.ptr))
			return;
}

template <typename Layout>
void BasicPanel<Layout>::doForeachChildren(libv::function_ref<void(BaseComponent&)> callback) {
	for (const auto& child : children)
		callback(*child.ptr);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
