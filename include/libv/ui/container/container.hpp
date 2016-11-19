// File: Container.hpp, Created on 2014. január 6. 15:45, Author: Vader

#pragma once

// libv
#include <libv/memory.hpp>
// std
#include <memory>
#include <vector>
// pro
#include <libv/ui/component/component.hpp>

namespace libv {
namespace ui {

class Context;

// -------------------------------------------------------------------------------------------------

class Container : public Component {
public:
	struct LayoutResult {
		vec3 size;
		vec3 offset;
	};
	struct ContainedComponent {
		LayoutResult info;
		adaptive_ptr<Component> ptr;

		ContainedComponent(adaptive_ptr<Component> component) : ptr(component) { }
	};

public:
	using Store = std::vector<ContainedComponent>;

protected:
	Store components;

public:
	void add(const observer_ptr<Component>& component);
	void add(const shared_ptr<Component>& component);
	void addObserver(const observer_ptr<Component>& component);
	//void addObserver(const shared_ptr<Component>& component); // Hazardous
	void addObserver(Component& component);
	void addObserver(Component* const component);
	//void addShared(const observer_ptr<Component>& component); // Hazardous
	void addShared(const shared_ptr<Component>& component);
	//void addShared(Component& component); // Hazardous
	void addShared(Component* const component);

//	void remove(const observer_ptr<Component>& component);
//	void remove(const shared_ptr<Component>& component);

private:
	virtual LayoutInfo doLayout(const LayoutInfo& parentLayout) override = 0;
	virtual void doBuild(Context& context) override;
	virtual void doDestroy(Context& context) override;
	virtual void doRender(Context& context) override;

public:
	Container() = default;
	virtual ~Container() = default;
};

//	void setFocusPolicy(...);
//	std::vector<adaptive_ptr<Component>> findComponentAt(int, int);
//	std::vector<adaptive_ptr<Component>> getComponentAt(int, int);

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv