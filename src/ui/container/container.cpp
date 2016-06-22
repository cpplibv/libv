// File: Container.cpp, Created on 2014. január 7. 6:05, Author: Vader

// hpp
#include <libv/ui/container/container.hpp>
// ext
#include <glm/gtx/transform.hpp>
// std
#include <algorithm>
// pro
#include <libv/ui/component/component.hpp>
#include <libv/ui/layout.hpp>
#include <libv/ui/log.hpp>
#include <libv/ui/properties.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void Container::add(const observer_ptr<Component>& component) {
	LIBV_UI_COMPONENT_TRACE("Add component [%s] to container [%s]",
			component->getComponentID(), this->getComponentID());
	components.emplace_back(make_adaptive(component));
	ParentAccessor::setParent(component, make_observer(this));
	invalidate();
}

void Container::add(const shared_ptr<Component>& component) {
	LIBV_UI_COMPONENT_TRACE("Add component [%s] to container [%s]",
			component->getComponentID(), this->getComponentID());
	components.emplace_back(make_adaptive(component));
	ParentAccessor::setParent(make_observer(component), make_observer(this));
	invalidate();
}

void Container::addObserver(const observer_ptr<Component>& component) {
	add(component);
}

void Container::addObserver(Component& component) {
	add(make_observer(&component));
}

void Container::addObserver(Component * const component) {
	add(make_observer(component));
}

void Container::addShared(const shared_ptr<Component>& component) {
	add(component);
}

void Container::addShared(Component * const component) {
	add(shared_ptr<Component>(component));
}

//void Container::remove(const observer_ptr<Component>& component) {
//	components.erase(std::remove(components.begin(), components.end(), component), components.end());
//	ParentAccessor::setParent(component, make_observer(nullptr));
//}
//
//void Container::remove(const shared_ptr<Component>& component) {
//	components.erase(std::remove(components.begin(), components.end(), component), components.end());
//	ParentAccessor::setParent(component, make_observer(nullptr));
//}

// -------------------------------------------------------------------------------------------------

void Container::doBuild(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Build Container [%s]", componentID);
	for (auto& component : components) {
		component.ptr->build(renderer);
	}
}

void Container::doDestroy(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Destroy Container [%s]", componentID);
	for (auto& component : components) {
		component.ptr->destroy(renderer);
	}
}

void Container::doRender(Renderer& gl) {
	for (auto& component : components) {
		gl.pushMatrixModel();

		gl.matrixModel() *= glm::translate(glm::vec3(component.info.offset));
		component.ptr->render(gl);

		gl.popMatrixModel();
	}
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv