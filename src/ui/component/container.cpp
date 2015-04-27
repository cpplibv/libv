// File: ProtectedContainer.cpp, Created on 2014. január 7. 6:05, Author: Vader

// hpp
#include "vl/ui/component/container.hpp"
// std
#include <algorithm>
// pro
#include "vl/ui/layout/properties.hpp"
#include "vl/ui/layout/layout_manager.hpp"

namespace vl {
namespace ui {

// -------------------------------------------------------------------------------------------------

void ProtectedContainer::buildComponents() {
	for (auto& component : components) {
		component->build();
	}
}

void ProtectedContainer::destroyComponents() {
	for (auto& component : components) {
		component->destroy();
	}
}

void ProtectedContainer::invalidateComponents() {
	for (auto& component : components) {
		component->invalidate();
	}
}

void ProtectedContainer::renderComponents() {
	for (auto& component : components) {
		component->render();
	}
}

void ProtectedContainer::updateComponents() {
	for (auto& component : components) {
		component->update();
	}
}

// -------------------------------------------------------------------------------------------------

void ProtectedContainer::add(ComponentPtr component) {
	assert(!component->getParent());

	component->setParent(this);
	components.emplace_back(std::move(component));
}

//void ProtectedContainer::remove(ComponentPtr component) {
//	components.erase(std::remove(components.begin(), components.end(), component), components.end());
//}

void ProtectedContainer::setLayout(std::shared_ptr<LayoutManager> manager) {
	this->layoutManager = manager;
}

// -------------------------------------------------------------------------------------------------

ProtectedContainer::const_iterator ProtectedContainer::begin() const {
	return components.begin();
}

ProtectedContainer::const_iterator ProtectedContainer::end() const {
	return components.end();
}

ProtectedContainer::const_reverse_iterator ProtectedContainer::rbegin() const {
	return components.rbegin();
}

ProtectedContainer::const_reverse_iterator ProtectedContainer::rend() const {
	return components.rend();
}

ProtectedContainer::iterator ProtectedContainer::begin() {
	return components.begin();
}

ProtectedContainer::iterator ProtectedContainer::end() {
	return components.end();
}

ProtectedContainer::reverse_iterator ProtectedContainer::rbegin() {
	return components.rbegin();
}

ProtectedContainer::reverse_iterator ProtectedContainer::rend() {
	return components.rend();
}

// -------------------------------------------------------------------------------------------------

void ProtectedContainer::build() {
	buildComponents();
	if (layoutManager != nullptr) {
//		layoutManager->layoutContainer(this);
		layoutManager->layout(begin(), end(), this);
	} //TODO P4: else default layout mrg
}

void ProtectedContainer::destroy() {
	destroyComponents();
}

void ProtectedContainer::invalidate() {
	invalidateComponents();
}

void ProtectedContainer::update() {
	updateComponents();
}

void ProtectedContainer::render() {
	renderComponents();
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace vl

