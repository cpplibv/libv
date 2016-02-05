// File: Container.cpp, Created on 2014. január 7. 6:05, Author: Vader

// hpp
#include <libv/ui/container/container.hpp>
// ext
#include <glm/gtx/transform.hpp>
// std
#include <algorithm>
// pro
#include <libv/ui/component/component.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/properties.hpp>

// TODO P4: default layout mrg (dummy fix size max overlapping? or flow?)
// TODO P4: invalidate in not virtual!
// TODO P4: build could be eliminated? do your stuff in render?
// TODO P4: more definite size management (mostly for frame! and its event) (sorry about this task)

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void Container::add(const observer_ptr<Component>& component) {
	components.emplace_back(component);
}

void Container::add(const shared_ptr<Component>& component) {
	components.emplace_back(component);
}

//void Container::remove(const observer_ptr<Component>& component) {
//	components.erase(std::remove(components.begin(), components.end(), component), components.end());
//}
//
//void Container::remove(const shared_ptr<Component>& component) {
//	components.erase(std::remove(components.begin(), components.end(), component), components.end());
//}

// -------------------------------------------------------------------------------------------------

void Container::buildComponents(Renderer& renderer) {
	doBuildComponents(renderer);
}

void Container::destroyComponents(Renderer& renderer) {
	doDestroyComponents(renderer);
}

void Container::renderComponents(Renderer& renderer) {
	doRenderComponents(renderer);
}

// -------------------------------------------------------------------------------------------------

void Container::doBuildComponents(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Build Container");
	for (auto& component : components) {
		component->build(renderer);
	}
}

void Container::doDestroyComponents(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Destroy Container");
	for (auto& component : components) {
		component->destroy(renderer);
	}
}

void Container::doRenderComponents(Renderer& gl) {
	for (auto& component : components) {
		component->render(gl);
	}
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv