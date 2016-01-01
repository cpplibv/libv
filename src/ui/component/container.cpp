// File: ProtectedContainer.cpp, Created on 2014. január 7. 6:05, Author: Vader

// hpp
#include <libv/ui/component/container.hpp>
// ext
#include <glm/gtc/matrix_transform.hpp>
// std
#include <algorithm>
// pro
#include <libv/ui/log.hpp>
#include <libv/ui/layout/properties.hpp>
#include <libv/ui/layout/layout_manager.hpp>

//TODO P4: default layout mrg
//TODO P4: invalidate in not virtual!

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void ProtectedContainer::add(const observer_ptr<Component>& component) {
	assert(!component->getParent());

	component->setParent(this);
	components.emplace_back(component);
}

void ProtectedContainer::add(const shared_ptr<Component>& component) {
	assert(!component->getParent());

	component->setParent(this);
	components.emplace_back(component);
}

//void ProtectedContainer::remove(adaptive_ptr<Component> component) {
//	components.erase(std::remove(components.begin(), components.end(), component), components.end());
//}

void ProtectedContainer::setLayout(const observer_ptr<LayoutManager>& manager) {
	this->layoutManager = manager;
}
void ProtectedContainer::setLayout(const shared_ptr<LayoutManager>& manager) {
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
	LIBV_UI_COMPONENT_TRACE("Build Container");
	buildComponents(renderer);

	if (layoutManager) {
		layoutManager->layout(begin(), end(), this);
	}
	Component::validate();
}

void ProtectedContainer::destroy(Renderer& renderer) {
	LIBV_UI_COMPONENT_TRACE("Destroy Container");
	destroyComponents(renderer);
}

void ProtectedContainer::invalidate() {
	LIBV_UI_COMPONENT_TRACE("Invalidate Container");
	Component::invalidate();
	invalidateComponents();
}

void ProtectedContainer::render(Renderer& renderer) {
	glClearColor(0.236f, 0.311f, 0.311f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(pos.x, pos.y, size.x, size.y);

//	LIBV_UI_COMPONENT_TRACE("%d %d %d %d", pos.x, pos.y, size.x, size.y);

	renderer.pushMatrixView(glm::ortho(0, size.x, 0, size.y, 1000, -1000));
	renderComponents(renderer);
}

void ProtectedContainer::update() {
	updateComponents();
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv

