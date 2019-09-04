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
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename Layout>
class BasicPanel : public BaseComponent {
public:
	typename Layout::Properties properties;
private:
	std::vector<typename Layout::Child> children;

public:
	BasicPanel();
	BasicPanel(std::string name);
	BasicPanel(UnnamedTag, const std::string_view type);
	virtual ~BasicPanel();

public:
	typename Layout::Child& add(std::shared_ptr<BaseComponent> component);
	void remove(const std::shared_ptr<BaseComponent>& component);
	void clear();

protected:
	virtual void doDetachChildren(libv::function_ref<bool(BaseComponent&)> callback) override;
	virtual void doStyle() override;
	virtual void doStyle(uint32_t childID) override;
	virtual void doLayout1(const ContextLayout1& le) override;
	virtual void doLayout2(const ContextLayout2& le) override;
	virtual void doForeachChildren(libv::function_ref<void(BaseComponent&)> callback) override;
};

// -------------------------------------------------------------------------------------------------

template <typename Layout>
BasicPanel<Layout>::BasicPanel() :
	BaseComponent(UnnamedTag{}, "basic-panel") { }

template <typename Layout>
BasicPanel<Layout>::BasicPanel(std::string name) :
	BaseComponent(std::move(name)) { }

template <typename Layout>
BasicPanel<Layout>::BasicPanel(UnnamedTag, const std::string_view type) :
	BaseComponent(UnnamedTag{}, type) { }

template <typename Layout>
BasicPanel<Layout>::~BasicPanel() {
}

// -------------------------------------------------------------------------------------------------

template <typename Layout>
typename Layout::Child& BasicPanel<Layout>::add(std::shared_ptr<BaseComponent> component) {
	const auto childID = static_cast<uint32_t>(children.size());
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
	uint32_t numRemoved = 0;

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
void BasicPanel<Layout>::doStyle() {
	set(properties);
}

template <typename Layout>
void BasicPanel<Layout>::doStyle(uint32_t childID) {
	children[childID].ptr->set(children[childID].properties);
}

template <typename Layout>
void BasicPanel<Layout>::doLayout1(const ContextLayout1& le) {
	AccessLayout::lastDynamic(*this) = Layout::layout1(le, children, properties, *this);
}

template <typename Layout>
void BasicPanel<Layout>::doLayout2(const ContextLayout2& le) {
	Layout::layout2(le, children, properties, *this);
}

template <typename Layout>
void BasicPanel<Layout>::doForeachChildren(libv::function_ref<void(BaseComponent&)> callback) {
	for (const auto& child : children)
		callback(*child.ptr);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
