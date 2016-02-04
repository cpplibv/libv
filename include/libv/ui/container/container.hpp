// File: Container.hpp, Created on 2014. január 6. 15:45, Author: Vader

#pragma once

// libv
#include <libv/memory.hpp>
// std
#include <memory>
#include <vector>
// pro
#include <libv/ui/render/renderer.hpp>

namespace libv {
namespace ui {

class Component;

// -------------------------------------------------------------------------------------------------

class Container {
public:
	using Store = std::vector<adaptive_ptr<Component>>;

protected:
	Store components;

public:
	void add(const observer_ptr<Component>& component);
	void add(const shared_ptr<Component>& component);
//	void remove(const observer_ptr<Component>& component);
//	void remove(const shared_ptr<Component>& component);

public:
	void buildComponents(Renderer& renderer);
	void destroyComponents(Renderer& renderer);
	void renderComponents(Renderer& renderer);

private:
	virtual void doBuildComponents(Renderer& renderer);
	virtual void doDestroyComponents(Renderer& renderer);
	virtual void doRenderComponents(Renderer& renderer);

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