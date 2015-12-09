// File: Container.hpp, Created on 2014. január 6. 15:45, Author: Vader

#pragma once

// std
#include <memory>
#include <vector>
// pro
#include <vl/ui/component/component.hpp>

namespace vl {
namespace ui {

class LayoutManager;

// -------------------------------------------------------------------------------------------------

/**
 * ProtectedContainer is a base container which only maintains Component's public interface making
 * it ideal for use as a base for those component which containerness preferred to stay hidden.
 */
class ProtectedContainer : public Component {
private:
	using Store = std::vector<adaptive_ptr<Component>>;
protected:
	using const_iterator = Store::const_iterator;
	using const_reverse_iterator = Store::const_reverse_iterator;
	using iterator = Store::iterator;
	using reverse_iterator = Store::reverse_iterator;

private:
	std::vector<adaptive_ptr<Component>> components;
	adaptive_ptr<LayoutManager> layoutManager;

protected:
	void add(const observer_ptr<Component>& component);
	void add(const shared_ptr<Component>& component);
	void remove(const observer_ptr<Component>& component);
	void remove(const shared_ptr<Component>& component);
	void setLayout(const observer_ptr<LayoutManager>& manager);
	void setLayout(const shared_ptr<LayoutManager>& manager);

	const_iterator begin() const;
	const_iterator end() const;
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;
	iterator begin();
	iterator end();
	reverse_iterator rbegin();
	reverse_iterator rend();

protected:
	void buildComponents(Renderer& renderer);
	void destroyComponents(Renderer& renderer);
	void invalidateComponents();
	void renderComponents(Renderer& renderer);
	void updateComponents();

public:
	virtual void build(Renderer& renderer) override;
	virtual void destroy(Renderer& renderer) override;
	virtual void invalidate() override;
	virtual void render(Renderer& renderer) override;
	virtual void update() override;

public:
	ProtectedContainer() = default;
	virtual ~ProtectedContainer() = default;
};

struct Container : public ProtectedContainer {
	using ProtectedContainer::const_iterator;
	using ProtectedContainer::const_reverse_iterator;
	using ProtectedContainer::iterator;
	using ProtectedContainer::reverse_iterator;

	using ProtectedContainer::add;
	using ProtectedContainer::remove;
	using ProtectedContainer::setLayout;

	using ProtectedContainer::begin;
	using ProtectedContainer::end;
	using ProtectedContainer::rbegin;
	using ProtectedContainer::rend;
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace vl

//	std::begin
//	std::end
//	std::vector<adaptive_ptr<Component>> findComponentAt(int, int);
//	void setFocusPolicy(...);
//	adaptive_ptr<Component> getComponent(int);
//	std::vector<adaptive_ptr<Component>> getComponentAt(int, int);
//	int getComponentCount();
//	adaptive_ptr<Component> getFocusOwner();