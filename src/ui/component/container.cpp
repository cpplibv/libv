// File: ProtectedContainer.cpp, Created on 2014. január 7. 6:05, Author: Vader

// hpp
#include <vl/ui/component/container.hpp>
// std
#include <algorithm>
// pro
#include <vl/ui/layout/properties.hpp>
#include <vl/ui/layout/layout_manager.hpp>

namespace vl {
namespace ui {

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

void ProtectedContainer::buildComponents(Renderer& renderer) {
	for (auto& component : components) {
		component->build(renderer);
	}
}

void ProtectedContainer::destroyComponents(Renderer& renderer) {
	for (auto& component : components) {
		component->destroy(renderer);
	}
}

void ProtectedContainer::invalidateComponents() {
	for (auto& component : components) {
		component->invalidate();
	}
}

void ProtectedContainer::renderComponents(Renderer& renderer) {
	for (auto& component : components) {
		component->render(renderer);
	}
}

void ProtectedContainer::updateComponents() {
	for (auto& component : components) {
		component->update();
	}
}

// -------------------------------------------------------------------------------------------------

//	ivec3 size = v->getDisplaySize();
//	ivec3 pos = v->getDisplayPosition();
//
//	glPushMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glViewport(pos.x, pos.y, size.x, size.y);
//	gluOrtho2D(0, size.x, 0, size.y);
//
//	glBegin(GL_TRIANGLE_STRIP);
//	glColor4f(v->id, 0, v->blue, 1);
//	glVertex3f(0, 0, 0);
//	glVertex3f(size.x, 0, 0);
//	glVertex3f(0, size.y, 0);
//	glVertex3f(size.x, size.y, 0);
//	glEnd();
//	glPopMatrix();

void ProtectedContainer::build(Renderer& renderer) {
	buildComponents(renderer);
	if (layoutManager != nullptr) {
//		layoutManager->layoutContainer(this);
		layoutManager->layout(begin(), end(), this);
	} //TODO P4: else default layout mrg
}

void ProtectedContainer::destroy(Renderer& renderer) {
	destroyComponents(renderer);
}

void ProtectedContainer::invalidate() {
	invalidateComponents();
}

void ProtectedContainer::render(Renderer& renderer) {
	renderComponents(renderer);
}

void ProtectedContainer::update() {
	updateComponents();
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace vl

